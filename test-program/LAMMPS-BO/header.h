typedef long int int64_t;
#define SQR(x)        ((x)*(x))

namespace LAMMPS_NS {
typedef int tagint;
typedef int64_t bigint;
}
typedef int ivec[3];
typedef double rvec[3];
typedef double rtensor[3][3];
typedef double rvec2[2];
typedef double rvec4[4];

typedef LAMMPS_NS::tagint rc_tagint;
typedef LAMMPS_NS::bigint rc_bigint;

typedef struct
{
  int n_global;
  double* l;
  int vdw_type;
} global_parameters;

typedef struct
{

  char name[15];

  double r_s;
  double valency;
  double mass;
  double r_vdw;
  double epsilon;
  double gamma;
  double r_pi;
  double valency_e;
  double nlp_opt;


  double alpha;
  double gamma_w;
  double valency_boc;
  double p_ovun5;
  double chi;
  double eta;
  int p_hbond;


  double r_pi_pi;
  double p_lp2;
  double b_o_131;
  double b_o_132;
  double b_o_133;


  double p_ovun2;
  double p_val3;
  double valency_val;
  double p_val5;
  double rcore2;
  double ecore2;
  double acore2;


  double lgcij;
  double lgre;

} single_body_parameters;

typedef struct {

  double p_bo1,p_bo2,p_bo3,p_bo4,p_bo5,p_bo6;
  double r_s, r_p, r_pp;
  double p_boc3, p_boc4, p_boc5;


  double p_be1, p_be2;
  double De_s, De_p, De_pp;


  double p_ovun1;


  double D;
  double alpha;
  double r_vdW;
  double gamma_w;
  double rcore, ecore, acore;
  double lgcij, lgre;


  double gamma;

  double v13cor, ovc;
} two_body_parameters;

typedef struct {

  double theta_00;
  double p_val1, p_val2, p_val4, p_val7;


  double p_pen1;


  double p_coa1;
} three_body_parameters;


typedef struct{
  int cnt;
  three_body_parameters prm[5];
} three_body_header;


typedef struct{
  double r0_hb, p_hb1, p_hb2, p_hb3;
} hbond_parameters;

typedef struct {
  double V1, V2, V3;


  double p_tor1;


  double p_cot1;
} four_body_parameters;

typedef struct
{
  int cnt;
  four_body_parameters prm[5];
} four_body_header;

typedef struct
{
  int num_atom_types;
  global_parameters gp;
  single_body_parameters *sbp;
  two_body_parameters **tbp;
  three_body_header ***thbp;
  hbond_parameters ***hbp;
  four_body_header ****fbp;
} reax_interaction;


struct _reax_atom
{
  rc_tagint orig_id;
  int imprt_id;
  int type;
  char name[8];

  rvec x;
  rvec v;
  rvec f;
  rvec f_old;

  double q;
  rvec4 s;
  rvec4 t;

  int Hindex;
  int num_bonds;
  int num_hbonds;
  int renumber;

  int numbonds;
  int nbr_id[20];
  double nbr_bo[20];
  double sum_bo, no_lp;
};


typedef _reax_atom reax_atom;

typedef struct
{
  double V;
  rvec min, max, box_norms;

  rtensor box, box_inv;
  rtensor trans, trans_inv;
  rtensor g;
} simulation_box;


struct grid_cell
{
  double cutoff;
  rvec min, max;
  ivec rel_box;

  int mark;
  int type;
  int str;
  int end;
  int top;
  int* atoms;
  struct grid_cell** nbrs;
  ivec* nbrs_x;
  rvec* nbrs_cp;
};


typedef struct grid_cell grid_cell;


typedef struct
{
  int total, max_atoms, max_nbrs;
  ivec ncells;
  rvec cell_len;
  rvec inv_len;

  ivec bond_span;
  ivec nonb_span;
  ivec vlist_span;

  ivec native_cells;
  ivec native_str;
  ivec native_end;

  double ghost_cut;
  ivec ghost_span;
  ivec ghost_nonb_span;
  ivec ghost_hbond_span;
  ivec ghost_bond_span;

  grid_cell*** cells;
  ivec *order;
} grid;


typedef struct
{
  int rank;
  int est_send, est_recv;
  int atoms_str, atoms_cnt;
  ivec rltv, prdc;
  rvec bndry_min, bndry_max;

  int send_type;
  int recv_type;
  ivec str_send;
  ivec end_send;
  ivec str_recv;
  ivec end_recv;
} neighbor_proc;

typedef struct
{
  double ghost_nonb;
  double ghost_hbond;
  double ghost_bond;
  double ghost_cutoff;
} boundary_cutoff;

struct _reax_system
{
  reax_interaction reax_param;

  rc_bigint bigN;
  int n, N, numH;
  int local_cap, total_cap, gcell_cap, Hcap;
  int est_recv, est_trans, max_recved;
  int wsize, my_rank, num_nbrs;
  ivec my_coords;
  neighbor_proc my_nbrs[6];
  int *global_offset;
  simulation_box big_box, my_box, my_ext_box;
  grid my_grid;
  boundary_cutoff bndry_cuts;
  reax_atom *my_atoms;

  class Pair *pair_ptr;
  int my_bonds;
  int mincap;
  double safezone, saferzone;

  int omp_active;
};
typedef _reax_system reax_system;

typedef struct
{
  char sim_name[1024];
  int nprocs;
  int nthreads;
  ivec procs_by_dim;







  int ensemble;
  int nsteps;
  double dt;
  int geo_format;
  int restart;

  int restrict_bonds;
  int remove_CoM_vel;
  int random_vel;
  int reposition_atoms;

  int reneighbor;
  double vlist_cut;
  double bond_cut;
  double nonb_cut, nonb_low;
  double hbond_cut;
  double user_ghost_cut;

  double bg_cut;
  double bo_cut;
  double thb_cut;
  double thb_cutsq;

  int tabulate;

  int qeq_freq;
  double q_err;
  int refactor;
  double droptol;

  double T_init, T_final, T;
  double Tau_T;
  int T_mode;
  double T_rate, T_freq;

  int virial;
  rvec P, Tau_P, Tau_PT;
  int press_mode;
  double compressibility;

  int molecular_analysis;
  int num_ignored;
  int ignore[25];

  int dipole_anal;
  int freq_dipole_anal;
  int diffusion_coef;
  int freq_diffusion_coef;
  int restrict_type;

  int lgflag;
  int enobondsflag;

} control_params;


typedef struct
{
  double T;
  double xi;
  double v_xi;
  double v_xi_old;
  double G_xi;

} thermostat;


typedef struct
{
  double P;
  double eps;
  double v_eps;
  double v_eps_old;
  double a_eps;

} isotropic_barostat;


typedef struct
{
  rtensor P;
  double P_scalar;

  double eps;
  double v_eps;
  double v_eps_old;
  double a_eps;

  rtensor h0;
  rtensor v_g0;
  rtensor v_g0_old;
  rtensor a_g0;

} flexible_barostat;


typedef struct
{
  double start;
  double end;
  double elapsed;

  double total;
  double comm;
  double nbrs;
  double init_forces;
  double bonded;
  double nonb;
  double qEq;
  int s_matvecs;
  int t_matvecs;
} reax_timing;



typedef struct
{
  double e_tot;
  double e_kin;
  double e_pot;

  double e_bond;
  double e_ov;
  double e_un;
  double e_lp;
  double e_ang;
  double e_pen;
  double e_coa;
  double e_hb;
  double e_tor;
  double e_con;
  double e_vdW;
  double e_ele;
  double e_pol;
} energy_data;

typedef struct
{
  int step;
  int prev_steps;
  double time;

  double M;
  double inv_M;

  rvec xcm;
  rvec vcm;
  rvec fcm;
  rvec amcm;
  rvec avcm;
  double etran_cm;
  double erot_cm;

  rtensor kinetic;
  rtensor virial;

  energy_data my_en;
  energy_data sys_en;

  double N_f;
  rvec t_scale;
  rtensor p_scale;
  thermostat therm;
  isotropic_barostat iso_bar;
  flexible_barostat flex_bar;
  double inv_W;

  double kin_press;
  rvec int_press;
  rvec my_ext_press;
  rvec ext_press;
  rvec tot_press;

  reax_timing timing;
} simulation_data;

typedef struct{
  int thb;
  int pthb;
  double theta, cos_theta;
  rvec dcos_di, dcos_dj, dcos_dk;
} three_body_interaction_data;


typedef struct {
  int nbr;
  ivec rel_box;
  double d;
  rvec dvec;
} far_neighbor_data;


typedef struct {
  int nbr;
  int scl;
  far_neighbor_data *ptr;
} hbond_data;



typedef struct{
  int wrt;
  rvec dVal;
} dDelta_data;


typedef struct{
  int wrt;
  rvec dBO, dBOpi, dBOpi2;
} dbond_data;

typedef struct{
  double BO, BO_s, BO_pi, BO_pi2;
  double Cdbo, Cdbopi, Cdbopi2;
  double C1dbo, C2dbo, C3dbo;
  double C1dbopi, C2dbopi, C3dbopi, C4dbopi;
  double C1dbopi2, C2dbopi2, C3dbopi2, C4dbopi2;
  rvec dBOp, dln_BOp_s, dln_BOp_pi, dln_BOp_pi2;
  double *CdboReduction;
} bond_order_data;


typedef struct {
  int nbr;
  int sym_index;
  int dbond_index;
  ivec rel_box;

  double d;
  rvec dvec;
  bond_order_data bo_data;
} bond_data;


typedef struct {
  int j;
  double val;
} sparse_matrix_entry;

typedef struct {
  int cap, n, m;
  int *start, *end;
  sparse_matrix_entry *entries;
} sparse_matrix;


typedef struct {
  int num_far;
  int H, Htop;
  int hbonds, num_hbonds;
  int bonds, num_bonds;
  int num_3body;
  int gcell_atoms;
} reallocate_data;


typedef struct
{
  int allocated;


  double *tmp_dbl[6];
  rvec *tmp_rvec[6];
  rvec2 *tmp_rvec2[6];
  int *within_bond_box;


  double *total_bond_order;
  double *Deltap, *Deltap_boc;
  double *Delta, *Delta_lp, *Delta_lp_temp, *Delta_e, *Delta_boc, *Delta_val;
  double *dDelta_lp, *dDelta_lp_temp;
  double *nlp, *nlp_temp, *Clp, *vlpex;
  rvec *dDeltap_self;
  int *bond_mark, *done_after;


  sparse_matrix *H, *L, *U;
  double *Hdia_inv, *b_s, *b_t, *b_prc, *b_prm, *s, *t;
  double *droptol;
  rvec2 *b, *x;


  double *y, *z, *g;
  double *hc, *hs;
  double **h, **v;

  double *r, *d, *q, *p;
  rvec2 *r2, *d2, *q2, *p2;

  double Tap[8];


  int *mark, *old_mark;
  rvec *x_old;


  int *restricted;
  int **restricted_list;


  rvec *v_const;


  double *CdDelta;
  rvec *f;


  rvec *forceReduction;
  rvec *my_ext_pressReduction;
  double *CdDeltaReduction;
  int *valence_angle_atom_myoffset;

  reallocate_data realloc;
} storage;


typedef union
{
  void *v;
  three_body_interaction_data *three_body_list;
  bond_data *bond_list;
  dbond_data *dbo_list;
  dDelta_data *dDelta_list;
  far_neighbor_data *far_nbr_list;
  hbond_data *hbond_list;
} list_type;


struct _reax_list
{
  int allocated;

  int n;
  int num_intrs;

  int *index;
  int *end_index;

  int type;
  list_type select;
};
typedef _reax_list reax_list;


typedef struct
{
  FILE *strj;
  int trj_offset;
  int atom_line_len;
  int bond_line_len;
  int angle_line_len;
  int write_atoms;
  int write_bonds;
  int write_angles;
  char *line;
  int buffer_len;
  char *buffer;

  FILE *out;
  FILE *pot;
  FILE *log;
  FILE *mol, *ign;
  FILE *dpl;
  FILE *drft;
  FILE *pdb;
  FILE *prs;

  int write_steps;
  int traj_compress;
  int traj_method;
  char traj_title[81];
  int atom_info;
  int bond_info;
  int angle_info;

  int restart_format;
  int restart_freq;
  int debug_level;
  int energy_update_freq;

} output_controls;

inline int Start_Index( int, reax_list* );
inline int End_Index( int, reax_list* );

inline int Start_Index( int i, reax_list *l )
{
  return l->index[i];
}

inline int End_Index( int i, reax_list *l )
{
  return l->end_index[i];
}
enum lists { BONDS, OLD_BONDS, THREE_BODIES,
             HBONDS, FAR_NBRS, DBOS, DDELTAS, LIST_N };
