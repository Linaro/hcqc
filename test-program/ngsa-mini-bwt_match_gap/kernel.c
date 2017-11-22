#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define STATE_M 0
#define STATE_I 1
#define STATE_D 2

#define aln_score(m,o,e,p) ((m)*(p)->s_mm + (o)*(p)->s_gapo + (e)*(p)->s_gape)

#define BWA_MODE_GAPE       0x01
#define BWA_MODE_LOGGAP     0x04
#define BWA_MODE_NONSTOP    0x10

#define OCC_INTERVAL 0x80

typedef unsigned char ubyte_t;

typedef uint32_t bwtint_t;

typedef struct {
	bwtint_t primary; // S^{-1}(0), or the primary index of BWT
	bwtint_t L2[5]; // C(), cumulative count
	bwtint_t seq_len; // sequence length
	bwtint_t bwt_size; // size of bwt, about seq_len/4
	uint32_t *bwt; // BWT
	// occurance array, separated to two parts
	uint32_t cnt_table[256];
	// suffix array
	int sa_intv;
	bwtint_t n_sa;
	bwtint_t *sa;
} bwt_t;

typedef struct {
	bwtint_t w;
	int bid;
} bwt_width_t;

typedef struct {
	uint32_t n_mm:8, n_gapo:8, n_gape:8, a:1;
	bwtint_t k, l;
	int score;
} bwt_aln1_t;

typedef struct {
	int s_mm, s_gapo, s_gape;
	int mode;
	int indel_end_skip, max_del_occ, max_entries;
	float fnr;
	int max_diff, max_gapo, max_gape;
	int max_seed_diff, seed_len;
	int n_threads;
	int max_top2;
	int trim_qual;
} gap_opt_t;

typedef struct {
	u_int32_t info;
	u_int32_t n_mm:8, n_gapo:8, n_gape:8, state:2, n_seed_mm:6;
	bwtint_t k, l;
	int last_diff_pos;
} gap_entry_t;

typedef struct {
	int n_entries, m_entries;
	gap_entry_t *stack;
} gap_stack1_t;

typedef struct {
	int n_stacks, best, n_entries;
	gap_stack1_t *stacks;
} gap_stack_t;

#define bwt_occ_intv(b, k) ((b)->bwt + (k)/OCC_INTERVAL*12)

#define __occ_aux4(bwt, b)											\
	((bwt)->cnt_table[(b)&0xff] + (bwt)->cnt_table[(b)>>8&0xff]		\
	 + (bwt)->cnt_table[(b)>>16&0xff] + (bwt)->cnt_table[(b)>>24])

static inline void bwt_occ4(const bwt_t *bwt, bwtint_t k, bwtint_t cnt[4])
{
	bwtint_t l, j, x;
	uint32_t *p;
	if (k == (bwtint_t)(-1)) {
		memset(cnt, 0, 4 * sizeof(bwtint_t));
		return;
	}
	if (k >= bwt->primary) --k; // because $ is not in bwt
	p = bwt_occ_intv(bwt, k);
	memcpy(cnt, p, 16);
	p += 4;
	j = k >> 4 << 4;
	for (l = k / OCC_INTERVAL * OCC_INTERVAL, x = 0; l < j; l += 16, ++p)
		x += __occ_aux4(bwt, *p);
	x += __occ_aux4(bwt, *p & ~((1U<<((~k&15)<<1)) - 1)) - (~k&15);
	cnt[0] += x&0xff; cnt[1] += x>>8&0xff; cnt[2] += x>>16&0xff; cnt[3] += x>>24;
}

// an analogy to bwt_occ4() but more efficient, requiring k <= l
static inline void bwt_2occ4(const bwt_t *bwt, bwtint_t k, bwtint_t l, bwtint_t cntk[4], bwtint_t cntl[4])
{
	bwtint_t _k, _l;
	if (k == l) {
		bwt_occ4(bwt, k, cntk);
		memcpy(cntl, cntk, 4 * sizeof(bwtint_t));
		return;
	}
	_k = (k >= bwt->primary)? k-1 : k;
	_l = (l >= bwt->primary)? l-1 : l;
	if (_l/OCC_INTERVAL != _k/OCC_INTERVAL || k == (bwtint_t)(-1) || l == (bwtint_t)(-1)) {
		bwt_occ4(bwt, k, cntk);
		bwt_occ4(bwt, l, cntl);
	} else {
		bwtint_t i, j, x, y;
		uint32_t *p;
		int cl[4];
		if (k >= bwt->primary) --k; // because $ is not in bwt
		if (l >= bwt->primary) --l;
		cl[0] = cl[1] = cl[2] = cl[3] = 0;
		p = bwt_occ_intv(bwt, k);
		memcpy(cntk, p, 4 * sizeof(bwtint_t));
		p += 4;
		// prepare cntk[]
		j = k >> 4 << 4;
		for (i = k / OCC_INTERVAL * OCC_INTERVAL, x = 0; i < j; i += 16, ++p)
			x += __occ_aux4(bwt, *p);
		y = x;
		x += __occ_aux4(bwt, *p & ~((1U<<((~k&15)<<1)) - 1)) - (~k&15);
		// calculate cntl[] and finalize cntk[]
		j = l >> 4 << 4;
		for (; i < j; i += 16, ++p) y += __occ_aux4(bwt, *p);
		y += __occ_aux4(bwt, *p & ~((1U<<((~l&15)<<1)) - 1)) - (~l&15);
		memcpy(cntl, cntk, 16);
		cntk[0] += x&0xff; cntk[1] += x>>8&0xff; cntk[2] += x>>16&0xff; cntk[3] += x>>24;
		cntl[0] += y&0xff; cntl[1] += y>>8&0xff; cntl[2] += y>>16&0xff; cntl[3] += y>>24;
	}
}

extern int bwt_match_exact_alt(const bwt_t *bwt, int len, const ubyte_t *str, bwtint_t *k0, bwtint_t *l0);
extern void gap_reset_stack(gap_stack_t *stack);

static inline void gap_push(gap_stack_t *stack, int a, int i, bwtint_t k, bwtint_t l, int n_mm, int n_gapo, int n_gape,
							int state, int is_diff, const gap_opt_t *opt)
{
	int score;
	gap_entry_t *p;
	gap_stack1_t *q;
	score = aln_score(n_mm, n_gapo, n_gape, opt);
	q = stack->stacks + score;
	if (q->n_entries == q->m_entries) {
		q->m_entries <<= 1;
		q->stack = (gap_entry_t*)realloc(q->stack, sizeof(gap_entry_t) * q->m_entries);
	}
	p = q->stack + q->n_entries;
	p->info = (u_int32_t)score<<21 | a<<20 | i; p->k = k; p->l = l;
	p->n_mm = n_mm; p->n_gapo = n_gapo; p->n_gape = n_gape; p->state = state;
	if (is_diff) p->last_diff_pos = i;
	++(q->n_entries);
	++(stack->n_entries);
	if (stack->best > score) stack->best = score;
}

static inline void gap_pop(gap_stack_t *stack, gap_entry_t *e)
{
	gap_stack1_t *q;
	q = stack->stacks + stack->best;
	*e = q->stack[q->n_entries - 1];
	--(q->n_entries);
	--(stack->n_entries);
	if (q->n_entries == 0 && stack->n_entries) { // reset best
		int i;
		for (i = stack->best + 1; i < stack->n_stacks; ++i)
			if (stack->stacks[i].n_entries != 0) break;
		stack->best = i;
	} else if (stack->n_entries == 0) stack->best = stack->n_stacks;
}

static inline void gap_shadow(int x, int len, bwtint_t max, int last_diff_pos, bwt_width_t *w)
{
	int i, j;
	for (i = j = 0; i < last_diff_pos; ++i) {
		if (w[i].w > x) w[i].w -= x;
		else if (w[i].w == x) {
			w[i].bid = 1;
			w[i].w = max - (++j);
		} // else should not happen
	}
}

static inline int int_log2(uint32_t v)
{
	int c = 0;
	if (v & 0xffff0000u) { v >>= 16; c |= 16; }
	if (v & 0xff00) { v >>= 8; c |= 8; }
	if (v & 0xf0) { v >>= 4; c |= 4; }
	if (v & 0xc) { v >>= 2; c |= 2; }
	if (v & 0x2) c |= 1;
	return c;
}

bwt_aln1_t *bwt_match_gap(bwt_t *const bwts[2], int len, const ubyte_t *seq[2], bwt_width_t *w[2],
						  bwt_width_t *seed_w[2], const gap_opt_t *opt, int *_n_aln, gap_stack_t *stack)
{
	int best_score = aln_score(opt->max_diff+1, opt->max_gapo+1, opt->max_gape+1, opt);
	int best_diff = opt->max_diff + 1, max_diff = opt->max_diff;
	int best_cnt = 0;
	int max_entries = 0, j, _j, n_aln, m_aln;
	bwt_aln1_t *aln;

	m_aln = 4; n_aln = 0;
	aln = (bwt_aln1_t*)calloc(m_aln, sizeof(bwt_aln1_t));

	// check whether there are too many N
	for (j = _j = 0; j < len; ++j)
		if (seq[0][j] > 3) ++_j;
	if (_j > max_diff) {
		*_n_aln = n_aln;
		return aln;
	}

	//for (j = 0; j != len; ++j) printf("#0 %d: [%d,%u]\t[%d,%u]\n", j, w[0][j].bid, w[0][j].w, w[1][j].bid, w[1][j].w);
	gap_reset_stack(stack); // reset stack
	gap_push(stack, 0, len, 0, bwts[0]->seq_len, 0, 0, 0, 0, 0, opt);
	gap_push(stack, 1, len, 0, bwts[0]->seq_len, 0, 0, 0, 0, 0, opt);

	while (stack->n_entries) {
		gap_entry_t e;
		int a, i, m, m_seed = 0, hit_found, allow_diff, allow_M, tmp;
		bwtint_t k, l, cnt_k[4], cnt_l[4], occ;
		const bwt_t *bwt;
		const ubyte_t *str;
		const bwt_width_t *seed_width = 0;
		bwt_width_t *width;

		if (max_entries < stack->n_entries) max_entries = stack->n_entries;
		if (stack->n_entries > opt->max_entries) break;
		gap_pop(stack, &e); // get the best entry
		k = e.k; l = e.l; // SA interval
		a = e.info>>20&1; i = e.info&0xffff; // strand, length
		if (!(opt->mode & BWA_MODE_NONSTOP) && e.info>>21 > best_score + opt->s_mm) break; // no need to proceed

		m = max_diff - (e.n_mm + e.n_gapo);
		if (opt->mode & BWA_MODE_GAPE) m -= e.n_gape;
		if (m < 0) continue;
		bwt = bwts[1-a]; str = seq[a]; width = w[a];
		if (seed_w) { // apply seeding
			seed_width = seed_w[a];
			m_seed = opt->max_seed_diff - (e.n_mm + e.n_gapo);
			if (opt->mode & BWA_MODE_GAPE) m_seed -= e.n_gape;
		}
		//printf("#1\t[%d,%d,%d,%c]\t[%d,%d,%d]\t[%u,%u]\t[%u,%u]\t%d\n", stack->n_entries, a, i, "MID"[e.state], e.n_mm, e.n_gapo, e.n_gape, width[i-1].bid, width[i-1].w, k, l, e.last_diff_pos);
		if (i > 0 && m < width[i-1].bid) continue;

		// check whether a hit is found
		hit_found = 0;
		if (i == 0) hit_found = 1;
		else if (m == 0 && (e.state == STATE_M || (opt->mode&BWA_MODE_GAPE) || e.n_gape == opt->max_gape)) { // no diff allowed
			if (bwt_match_exact_alt(bwt, i, str, &k, &l)) hit_found = 1;
			else continue; // no hit, skip
		}

		if (hit_found) { // action for found hits
			int score = aln_score(e.n_mm, e.n_gapo, e.n_gape, opt);
			int do_add = 1;
			//printf("#2 hits found: %d:(%u,%u)\n", e.n_mm+e.n_gapo, k, l);
			if (n_aln == 0) {
				best_score = score;
				best_diff = e.n_mm + e.n_gapo;
				if (opt->mode & BWA_MODE_GAPE) best_diff += e.n_gape;
				if (!(opt->mode & BWA_MODE_NONSTOP))
					max_diff = (best_diff + 1 > opt->max_diff)? opt->max_diff : best_diff + 1; // top2 behaviour
			}
			if (score == best_score) best_cnt += l - k + 1;
			else if (best_cnt > opt->max_top2) break; // top2b behaviour
			if (e.n_gapo) { // check whether the hit has been found. this may happen when a gap occurs in a tandem repeat
				for (j = 0; j != n_aln; ++j)
					if (aln[j].k == k && aln[j].l == l) break;
				if (j < n_aln) do_add = 0;
			}
			if (do_add) { // append
				bwt_aln1_t *p;
				gap_shadow(l - k + 1, len, bwt->seq_len, e.last_diff_pos, width);
				if (n_aln == m_aln) {
					m_aln <<= 1;
					aln = (bwt_aln1_t*)realloc(aln, m_aln * sizeof(bwt_aln1_t));
					memset(aln + m_aln/2, 0, m_aln/2*sizeof(bwt_aln1_t));
				}
				p = aln + n_aln;
				p->n_mm = e.n_mm; p->n_gapo = e.n_gapo; p->n_gape = e.n_gape; p->a = a;
				p->k = k; p->l = l;
				p->score = score;
				++n_aln;
			}
			continue;
		}

		--i;
		bwt_2occ4(bwt, k - 1, l, cnt_k, cnt_l); // retrieve Occ values
		occ = l - k + 1;
		// test whether diff is allowed
		allow_diff = allow_M = 1;
		if (i > 0) {
			int ii = i - (len - opt->seed_len);
			if (width[i-1].bid > m-1) allow_diff = 0;
			else if (width[i-1].bid == m-1 && width[i].bid == m-1 && width[i-1].w == width[i].w) allow_M = 0;
			if (seed_w && ii > 0) {
				if (seed_width[ii-1].bid > m_seed-1) allow_diff = 0;
				else if (seed_width[ii-1].bid == m_seed-1 && seed_width[ii].bid == m_seed-1
						 && seed_width[ii-1].w == seed_width[ii].w) allow_M = 0;
			}
		}
		// indels
		tmp = (opt->mode & BWA_MODE_LOGGAP)? int_log2(e.n_gape + e.n_gapo)/2+1 : e.n_gapo + e.n_gape;
		if (allow_diff && i >= opt->indel_end_skip + tmp && len - i >= opt->indel_end_skip + tmp) {
			if (e.state == STATE_M) { // gap open
				if (e.n_gapo < opt->max_gapo) { // gap open is allowed
					// insertion
					gap_push(stack, a, i, k, l, e.n_mm, e.n_gapo + 1, e.n_gape, STATE_I, 1, opt);
					// deletion
					for (j = 0; j != 4; ++j) {
						k = bwt->L2[j] + cnt_k[j] + 1;
						l = bwt->L2[j] + cnt_l[j];
						if (k <= l) gap_push(stack, a, i + 1, k, l, e.n_mm, e.n_gapo + 1, e.n_gape, STATE_D, 1, opt);
					}
				}
			} else if (e.state == STATE_I) { // extention of an insertion
				if (e.n_gape < opt->max_gape) // gap extention is allowed
					gap_push(stack, a, i, k, l, e.n_mm, e.n_gapo, e.n_gape + 1, STATE_I, 1, opt);
			} else if (e.state == STATE_D) { // extention of a deletion
				if (e.n_gape < opt->max_gape) { // gap extention is allowed
					if (e.n_gape + e.n_gapo < max_diff || occ < opt->max_del_occ) {
						for (j = 0; j != 4; ++j) {
							k = bwt->L2[j] + cnt_k[j] + 1;
							l = bwt->L2[j] + cnt_l[j];
							if (k <= l) gap_push(stack, a, i + 1, k, l, e.n_mm, e.n_gapo, e.n_gape + 1, STATE_D, 1, opt);
						}
					}
				}
			}
		}
		// mismatches
		if (allow_diff && allow_M) { // mismatch is allowed
			for (j = 1; j <= 4; ++j) {
				int c = (str[i] + j) & 3;
				int is_mm = (j != 4 || str[i] > 3);
				k = bwt->L2[c] + cnt_k[c] + 1;
				l = bwt->L2[c] + cnt_l[c];
				if (k <= l) gap_push(stack, a, i, k, l, e.n_mm + is_mm, e.n_gapo, e.n_gape, STATE_M, is_mm, opt);
			}
		} else if (str[i] < 4) { // try exact match only
			int c = str[i] & 3;
			k = bwt->L2[c] + cnt_k[c] + 1;
			l = bwt->L2[c] + cnt_l[c];
			if (k <= l) gap_push(stack, a, i, k, l, e.n_mm, e.n_gapo, e.n_gape, STATE_M, 0, opt);
		}
	}

	*_n_aln = n_aln;
	//fprintf(stderr, "max_entries = %d\n", max_entries);
	return aln;
}
