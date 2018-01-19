#ifndef TYPEDEF_HCQC_SIZE
#define TYPEDEF_HCQC_SIZE
typedef long long int HCQC_SIZE;
#endif

extern void HCQC_save_begin ();
extern void HCQC_save_end ();
extern void HCQC_load_begin ();
extern void HCQC_load_end ();
extern void HCQC_save_data_s (const char *name, const void *data_start, int unit_size);
extern void HCQC_save_data_a (const char *name, const void *data_start, int unit_size, HCQC_SIZE unit_number);
extern void HCQC_load_data_s (const char *name, void *data_start);
extern void HCQC_load_data_a (const char *name, void *data_start);

extern void HCQC_check_data_s_b (const char *name, _Bool check_data, _Bool data);
extern void HCQC_check_data_s_i (const char *name, int check_data, int data);
extern void HCQC_check_data_s_ui (const char *name, unsigned int check_data, unsigned int data);
extern void HCQC_check_data_s_l (const char *name, long check_data, long data);
extern void HCQC_check_data_s_ul (const char *name, unsigned long check_data, unsigned long data);
extern void HCQC_check_data_s_ll (const char *name, long long check_data, long long data);
extern void HCQC_check_data_s_ull (const char *name, unsigned long long check_data, unsigned long long data);
extern void HCQC_check_data_s_f (const char *name, float check_data, float data);
extern void HCQC_check_data_s_d (const char *name, double check_data, double data);
extern void HCQC_check_data_s_ld (const char *name, long double check_data, long double data);
extern void HCQC_check_data_s_cf (const char *name, float _Complex check_data, float _Complex data);
extern void HCQC_check_data_s_cd (const char *name, double _Complex check_data, double _Complex data);
extern void HCQC_check_data_s_cld (const char *name, long double _Complex check_data, long double _Complex data);
extern void HCQC_check_data_a_b (const char *name, const _Bool *check_data, const _Bool *data, HCQC_SIZE unit_number);
extern void HCQC_check_data_a_i (const char *name, const int *check_data, const int *data, HCQC_SIZE unit_number);
extern void HCQC_check_data_a_ui (const char *name, const unsigned int *check_data, const unsigned int *data, HCQC_SIZE unit_number);
extern void HCQC_check_data_a_l (const char *name, const long *check_data, const long *data, HCQC_SIZE unit_number);
extern void HCQC_check_data_a_ul (const char *name, const unsigned long *check_data, const unsigned long *data, HCQC_SIZE unit_number);
extern void HCQC_check_data_a_ll (const char *name, const long long *check_data, const long long *data, HCQC_SIZE unit_number);
extern void HCQC_check_data_a_ull (const char *name, const unsigned long long *check_data, const unsigned long long *data, HCQC_SIZE unit_number);
extern void HCQC_check_data_a_f (const char *name, const float *check_data, const float *data, HCQC_SIZE unit_number);
extern void HCQC_check_data_a_d (const char *name, const double *check_data, const double *data, HCQC_SIZE unit_number);
extern void HCQC_check_data_a_ld (const char *name, const long double *check_data, const long double *data, HCQC_SIZE unit_number);
extern void HCQC_check_data_a_cf (const char *name, const float _Complex *check_data, const float _Complex *data, HCQC_SIZE unit_number);
extern void HCQC_check_data_a_cd (const char *name, const double _Complex *check_data, const double _Complex *data, HCQC_SIZE unit_number);
extern void  HCQC_check_data_a_cld (const char *name, const long double _Complex *check_data, const long double _Complex *data, HCQC_SIZE unit_number);
