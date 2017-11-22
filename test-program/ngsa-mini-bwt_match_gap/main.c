#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>

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

int bwt_match_exact_alt(const bwt_t *bwt, int len, const ubyte_t *str, bwtint_t *k0, bwtint_t *l0)
{
  /* TODO: */
  return 0;
}

void gap_reset_stack(gap_stack_t *stack)
{
  /* TODO: */
}

int main ()
{
#if 0
  /* TODO: */
  bwt_match_gap(bwts,
		len,
		seq,
		w,
		seed_w,
		opt,
		_n_aln,
		stack);
#endif
  exit (0);
}
