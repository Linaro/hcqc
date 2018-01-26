#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <complex.h>
#include <math.h>

#include "libHCQC.h"

int HCQC_error = 0;

static int DATA_FD;

void HCQC_save_begin (const char *data_filename)
{
  DATA_FD = creat (data_filename, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
  if (DATA_FD == -1) {
    abort ();
  }
}

#define DATA_SIZE_FIELD_BYTES sizeof(HCQC_SIZE)

union data_size_field {
  HCQC_SIZE size;
  unsigned char buffer[DATA_SIZE_FIELD_BYTES];
};

#define WORK_BUFFER_SIZE 1024

static void HCQC_save_data_common_0 (int fd, int unit_size, HCQC_SIZE unit_number)
{
  union data_size_field data;

  assert (DATA_SIZE_FIELD_BYTES == sizeof(union data_size_field));
  data.size = unit_size * unit_number;
  if (write (fd, data.buffer, DATA_SIZE_FIELD_BYTES) != DATA_SIZE_FIELD_BYTES)
    abort ();
}

static void HCQC_save_data_common_1 (int fd, const void *data_start, int unit_size, HCQC_SIZE unit_number)
{
  unsigned char buffer[WORK_BUFFER_SIZE];
  const unsigned char *src = (const unsigned char *)data_start;
  const unsigned char *p = src;
  size_t size = unit_size * unit_number;
  size_t rest_size = size;
  while (1) {
    size_t work_size;
    if (WORK_BUFFER_SIZE < rest_size)
      work_size = WORK_BUFFER_SIZE;
    else
      work_size = rest_size;
    memcpy (buffer, p, work_size);
    if (write (fd, buffer, work_size) != work_size)
      abort ();
    if (WORK_BUFFER_SIZE < rest_size) {
      rest_size -= WORK_BUFFER_SIZE;
      p += WORK_BUFFER_SIZE;
    } else
      break;
  }
}

void HCQC_save_data_s (const char *name, const void *data_start, int unit_size)
{
  HCQC_save_data_common_0 (DATA_FD, unit_size, 1);
  HCQC_save_data_common_1 (DATA_FD, data_start, unit_size, 1);
}

void HCQC_save_data_a (const char *name, const void *data_start, int unit_size, HCQC_SIZE unit_number)
{
  HCQC_save_data_common_0 (DATA_FD, unit_size, unit_number);
  HCQC_save_data_common_1 (DATA_FD, data_start, unit_size, unit_number);
}

void HCQC_save_end ()
{
  if (close (DATA_FD) == -1) {
    abort ();
  }
  DATA_FD = -1;
}

void HCQC_load_begin (const char *data_filename)
{
  DATA_FD = open (data_filename, O_RDONLY);
  if (DATA_FD == -1) {
    abort ();
  }
}

static HCQC_SIZE HCQC_load_data_common_0 (int fd)
{
  union data_size_field data;

  assert (DATA_SIZE_FIELD_BYTES == sizeof(union data_size_field));
  if (read (fd, data.buffer, DATA_SIZE_FIELD_BYTES) != DATA_SIZE_FIELD_BYTES)
    abort ();
  return data.size;
}

static void HCQC_load_data_common_1 (int fd, void *data_start, HCQC_SIZE size)
{
  unsigned char buffer[WORK_BUFFER_SIZE];
  unsigned char *dest = (unsigned char *)data_start;
  unsigned char *p = dest;
  size_t rest_size = size;
  while (1) {
    size_t work_size;
    if (WORK_BUFFER_SIZE < rest_size)
      work_size = WORK_BUFFER_SIZE;
    else
      work_size = rest_size;
    if (read (fd, buffer, work_size) != work_size)
      abort ();
    memcpy (p, buffer, work_size);
    if (WORK_BUFFER_SIZE < rest_size) {
      rest_size -= WORK_BUFFER_SIZE;
      p += WORK_BUFFER_SIZE;
    } else
      break;
  }
}

void HCQC_load_data_s (const char *name, void *data_start)
{
  {
    HCQC_SIZE size = HCQC_load_data_common_0 (DATA_FD);
    HCQC_load_data_common_1 (DATA_FD, data_start, size);
  }
}

void HCQC_load_data_a_core (const char *name, void *data_start, HCQC_SIZE *_size)
{
  {
    HCQC_SIZE size = HCQC_load_data_common_0 (DATA_FD);
    *_size = size;
    HCQC_load_data_common_1 (DATA_FD, data_start, size);
  }
}

void HCQC_load_data_a (const char *name, void *data_start)
{
  HCQC_SIZE size = -1;
  HCQC_load_data_a_core (name, data_start, &size);
}

void HCQC_load_end ()
{
  if (close (DATA_FD) == -1) {
    abort ();
  }
  DATA_FD = -1;
}

#define CHECK_PRINT(...) fprintf (stderr, __VA_ARGS__)

void HCQC_check_data_s_b (const char *name, _Bool check_data, _Bool data)
{
  CHECK_PRINT ("name = \"%s\"", name);
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=%d correct=%d\n", data, check_data);
    HCQC_error = 1;
  }
}

void HCQC_check_data_s_i (const char *name, int check_data, int data)
{
  CHECK_PRINT ("name = \"%s\"", name);
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=%d correct=%d\n", data, check_data);
    HCQC_error = 1;
  }
}

void HCQC_check_data_s_ui (const char *name, unsigned int check_data, unsigned int data)
{
  CHECK_PRINT ("name = \"%s\"", name);
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=%u correct=%u\n", data, check_data);
    HCQC_error = 1;
  }
}

void HCQC_check_data_s_l (const char *name, long check_data, long data)
{
  CHECK_PRINT ("name = \"%s\"", name);
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=%ld correct=%ld\n", data, check_data);
    HCQC_error = 1;
  }
}

void HCQC_check_data_s_ul (const char *name, unsigned long check_data, unsigned long data)
{
  CHECK_PRINT ("name = \"%s\"", name);
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=%lu correct=%lu\n", data, check_data);
    HCQC_error = 1;
  }
}

void HCQC_check_data_s_ll (const char *name, long long check_data, long long data)
{
  CHECK_PRINT ("name = \"%s\"", name);
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=%lld correct=%lld\n", data, check_data);
    HCQC_error = 1;
  }
}

void HCQC_check_data_s_ull (const char *name, unsigned long long check_data, unsigned long long data)
{
  CHECK_PRINT ("name = \"%s\"", name);
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=%llu correct=%llu\n", data, check_data);
    HCQC_error = 1;
  }
}

void HCQC_check_data_s_f (const char *name, float check_data, float data)
{
  CHECK_PRINT ("name = \"%s\"", name);
#ifndef ERROR_RATIO
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=%E correct=%E\n", data, check_data);
    HCQC_error = 1;
  }
#else
  float E_R = fabsf((check_data*ERROR_RATIO)/100);
  if ((check_data - data) > E_R) {
    CHECK_PRINT (" wrong=%E correct=%E range=[%E, %E]\n", data, check_data, check_data - E_R, check_data + E_R);
    HCQC_error = 1;
  } else {
    CHECK_PRINT (" ok\n");
  }
#endif
}

void HCQC_check_data_s_d (const char *name, double check_data, double data)
{
  CHECK_PRINT ("name = \"%s\"", name);
#ifndef ERROR_RATIO
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=%E correct=%E\n", data, check_data);
    HCQC_error = 1;
  }
#else
  double E_R = fabs((check_data*ERROR_RATIO)/100);
  if ((check_data - data) > E_R) {
    CHECK_PRINT (" wrong=%E correct=%E range=[%E, %E]\n", data, check_data, check_data - E_R, check_data + E_R);
    HCQC_error = 1;
  } else {
    CHECK_PRINT (" ok\n");
  }
#endif
}

void HCQC_check_data_s_ld (const char *name, long double check_data, long double data)
{
  CHECK_PRINT ("name = \"%s\"", name);
#ifndef ERROR_RATIO
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=%LE correct=%LE\n", data, check_data);
    HCQC_error = 1;
  }
#else
  long double E_R = fabsl((check_data*ERROR_RATIO)/100);
  if ((check_data - data) > E_R) {
    CHECK_PRINT (" wrong=%LE correct=%LE range=[%LE, %LE]\n", data, check_data, check_data - E_R, check_data + E_R);
    HCQC_error = 1;
  } else {
    CHECK_PRINT (" ok\n");
  }
#endif
}

void HCQC_check_data_s_cf (const char *name, float _Complex check_data, float _Complex data)
{
  CHECK_PRINT ("name = \"%s\"", name);
#ifndef ERROR_RATIO
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=(%E)+i(%E) correct=(%E)+i(%E)\n", crealf(data), cimagf(data), crealf(check_data), cimagf(check_data));
    HCQC_error = 1;
  }
#else
  float E_R = fabsf((crealf(check_data)*ERROR_RATIO)/100);
  float E_I = fabsf((cimagf(check_data)*ERROR_RATIO)/100);
  if (((crealf(check_data) - crealf(data)) > E_R)
      || ((cimagf(check_data) - cimagf(data)) > E_I)) {
    CHECK_PRINT (" wrong=(%E)+i(%E) correct=(%E)+i(%E)", crealf(data), cimagf(data), crealf(check_data), cimagf(check_data));
    CHECK_PRINT (" range=[%E, %E]+i[%E, %E]\n", crealf(check_data)-E_R, crealf(check_data)+E_R, cimagf(check_data)-E_I, cimagf(check_data)+E_I);
    HCQC_error = 1;
  } else {
    CHECK_PRINT (" ok\n");
  }
#endif
}

void HCQC_check_data_s_cd (const char *name, double _Complex check_data, double _Complex data)
{
  CHECK_PRINT ("name = \"%s\"", name);
#ifndef ERROR_RATIO
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=(%E)+i(%E) correct=(%E)+i(%E)\n", creal(data), cimag(data), creal(check_data), cimag(check_data));
    HCQC_error = 1;
  }
#else
  double E_R = fabs((creal(check_data)*ERROR_RATIO)/100);
  double E_I = fabs((cimag(check_data)*ERROR_RATIO)/100);
  if (((creal(check_data) - creal(data)) > E_R)
      || ((cimag(check_data) - cimag(data)) > E_I)) {
    CHECK_PRINT (" wrong=(%E)+i(%E) correct=(%E)+i(%E)", creal(data), cimag(data), creal(check_data), cimag(check_data));
    CHECK_PRINT (" range=[%E, %E]+i[%E, %E]\n", creal(check_data)-E_R, creal(check_data)+E_R, cimag(check_data)-E_I, cimag(check_data)+E_I);
    HCQC_error = 1;
  } else {
    CHECK_PRINT (" ok\n");
  }
#endif
}

void HCQC_check_data_s_cld (const char *name, long double _Complex check_data, long double _Complex data)
{
  CHECK_PRINT ("name = \"%s\"", name);
#ifndef ERROR_RATIO
  if (check_data == data)
    CHECK_PRINT (" ok\n");
  else {
    CHECK_PRINT (" wrong=(%LE)+i(%LE) correct=(%LE)+i(%LE)\n", creall(data), cimagl(data), creall(check_data), cimagl(check_data));
    HCQC_error = 1;
  }
#else
  long double E_R = fabsl((creall(check_data)*ERROR_RATIO)/100);
  long double E_I = fabsl((cimagl(check_data)*ERROR_RATIO)/100);
  if (((creall(check_data) - creall(data)) > E_R)
      || ((cimagl(check_data) - cimagl(data)) > E_I)) {
    CHECK_PRINT (" wrong=(%LE)+i(%LE) correct=(%LE)+i(%LE)", creall(data), cimagl(data), creall(check_data), cimagl(check_data));
    CHECK_PRINT (" range=[%LE, %LE]+i[%LE, %LE]\n", creall(check_data)-E_R, creall(check_data)+E_R, cimagl(check_data)-E_I, cimagl(check_data)+E_I);
    HCQC_error = 1;
  } else {
    CHECK_PRINT (" ok\n");
  }
#endif
}

void HCQC_check_data_a_b (const char *name, const _Bool *check_data, const _Bool *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
      if (! (check_data[i] == data[i])) {
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
        CHECK_PRINT (" wrong=%d correct=%d\n", data[i], check_data[i]);
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}

void HCQC_check_data_a_i (const char *name, const int *check_data, const int *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
      if (! (check_data[i] == data[i])) {
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
        CHECK_PRINT (" wrong=%d correct=%d\n", data[i], check_data[i]);
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}

void HCQC_check_data_a_ui (const char *name, const unsigned int *check_data, const unsigned int *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
      if (! (check_data[i] == data[i])) {
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
        CHECK_PRINT (" wrong=%u correct=%u\n", data[i], check_data[i]);
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}

void HCQC_check_data_a_l (const char *name, const long *check_data, const long *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
      if (! (check_data[i] == data[i])) {
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
        CHECK_PRINT (" wrong=%ld correct=%ld\n", data[i], check_data[i]);
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}

void HCQC_check_data_a_ul (const char *name, const unsigned long *check_data, const unsigned long *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
      if (! (check_data[i] == data[i])) {
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
        CHECK_PRINT (" wrong=%lu correct=%lu\n", data[i], check_data[i]);
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}

void HCQC_check_data_a_ll (const char *name, const long long *check_data, const long long *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
      if (! (check_data[i] == data[i])) {
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
        CHECK_PRINT (" wrong=%lld correct=%lld\n", data[i], check_data[i]);
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}

void HCQC_check_data_a_ull (const char *name, const unsigned long long *check_data, const unsigned long long *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
      if (! (check_data[i] == data[i])) {
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
        CHECK_PRINT (" wrong=%llu correct=%llu\n", data[i], check_data[i]);
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}

void HCQC_check_data_a_f (const char *name, const float *check_data, const float *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
#ifndef ERROR_RATIO
      if (! (check_data[i] == data[i])) {
#else
      float E_R = fabsf((check_data[i]*ERROR_RATIO)/100);
      if ((check_data[i] - data[i]) > E_R) {
#endif
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
#ifndef ERROR_RATIO
        CHECK_PRINT (" wrong=%E correct=%E\n", data[i], check_data[i]);
#else
        CHECK_PRINT (" wrong=%E correct=%E range=[%E, %E]\n", data[i], check_data[i], check_data[i] - E_R, check_data[i] + E_R);
#endif
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}

void HCQC_check_data_a_d (const char *name, const double *check_data, const double *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
#ifndef ERROR_RATIO
      if (! (check_data[i] == data[i])) {
#else
      double E_R = fabs((check_data[i]*ERROR_RATIO)/100);
      if ((check_data[i] - data[i]) > E_R) {
#endif
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
#ifndef ERROR_RATIO
        CHECK_PRINT (" wrong=%E correct=%E\n", data[i], check_data[i]);
#else
        CHECK_PRINT (" wrong=%E correct=%E range=[%E, %E]\n", data[i], check_data[i], check_data[i] - E_R, check_data[i] + E_R);
#endif
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}

void HCQC_check_data_a_ld (const char *name, const long double *check_data, const long double *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
#ifndef ERROR_RATIO
      if (! (check_data[i] == data[i])) {
#else
      long double E_R = fabsl((check_data[i]*ERROR_RATIO)/100);
      if ((check_data[i] - data[i]) > E_R) {
#endif
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
#ifndef ERROR_RATIO
        CHECK_PRINT (" wrong=%LE correct=%LE\n", data[i], check_data[i]);
#else
        CHECK_PRINT (" wrong=%LE correct=%LE range=[%LE, %LE]\n", data[i], check_data[i], check_data[i] - E_R, check_data[i] + E_R);
#endif
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}

void HCQC_check_data_a_cf (const char *name, const float _Complex *check_data, const float _Complex *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
#ifndef ERROR_RATIO
      if (! (check_data[i] == data[i])) {
#else
      float E_R = fabsf((crealf(check_data[i])*ERROR_RATIO)/100);
      float E_I = fabsf((cimagf(check_data[i])*ERROR_RATIO)/100);
      if (((crealf(check_data[i]) - crealf(data[i])) > E_R)
          || ((cimagf(check_data[i]) - cimagf(data[i])) > E_I)) {
#endif
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
#ifndef ERROR_RATIO
        CHECK_PRINT (" wrong=(%E)+i(%E) correct=(%E)+i(%E)\n", crealf(data[i]), cimagf(data[i]), crealf(check_data[i]), cimagf(check_data[i]));
#else
        CHECK_PRINT (" wrong=(%E)+i(%E) correct=(%E)+i(%E)", crealf(data[i]), cimagf(data[i]), crealf(check_data[i]), cimagf(check_data[i]));
        CHECK_PRINT (" range=[%E, %E]+i[%E, %E]\n", crealf(check_data[i])-E_R, crealf(check_data[i])+E_R, cimagf(check_data[i])-E_I, cimagf(check_data[i])+E_I);
#endif
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}

void HCQC_check_data_a_cd (const char *name, const double _Complex *check_data, const double _Complex *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
#ifndef ERROR_RATIO
      if (! (check_data[i] == data[i])) {
#else
      double E_R = fabs((creal(check_data[i])*ERROR_RATIO)/100);
      double E_I = fabs((cimag(check_data[i])*ERROR_RATIO)/100);
      if (((creal(check_data[i]) - creal(data[i])) > E_R)
          || ((cimag(check_data[i]) - cimag(data[i])) > E_I)) {
#endif
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
#ifndef ERROR_RATIO
        CHECK_PRINT (" wrong=(%E)+i(%E) correct=(%E)+i(%E)\n", creal(data[i]), cimag(data[i]), creal(check_data[i]), cimag(check_data[i]));
#else
        CHECK_PRINT (" wrong=(%E)+i(%E) correct=(%E)+i(%E)", creal(data[i]), cimag(data[i]), creal(check_data[i]), cimag(check_data[i]));
        CHECK_PRINT (" range=[%E, %E]+i[%E, %E]\n", creal(check_data[i])-E_R, creal(check_data[i])+E_R, cimag(check_data[i])-E_I, cimag(check_data[i])+E_I);
#endif
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}

void HCQC_check_data_a_cld (const char *name, const long double _Complex *check_data, const long double _Complex *data, HCQC_SIZE unit_number)
{
  CHECK_PRINT ("name = \"%s\"", name);
  {
    int count = 0;
    HCQC_SIZE i;
    for (i = 0; i < unit_number; i++) {
#ifndef ERROR_RATIO
      if (! (check_data[i] == data[i])) {
#else
      long double E_R = fabsl((creall(check_data[i])*ERROR_RATIO)/100);
      long double E_I = fabsl((cimagl(check_data[i])*ERROR_RATIO)/100);
      if (((creall(check_data[i]) - creall(data[i])) > E_R)
          || ((cimagl(check_data[i]) - cimagl(data[i])) > E_I)) {
#endif
        count++;
        if (count == 1) {
          CHECK_PRINT ("\n");
        }
        if (count == 5) {
          CHECK_PRINT (" index=%lld ...\n", i);
          break;
        }
        CHECK_PRINT (" index=%lld", i);
#ifndef ERROR_RATIO
        CHECK_PRINT (" wrong=(%LE)+i(%LE) correct=(%LE)+i(%LE)\n", creall(data[i]), cimagl(data[i]), creall(check_data[i]), cimagl(check_data[i]));
#else
        CHECK_PRINT (" wrong=(%LE)+i(%LE) correct=(%LE)+i(%LE)", creall(data[i]), cimagl(data[i]), creall(check_data[i]), cimagl(check_data[i]));
        CHECK_PRINT (" range=[%LE, %LE]+i[%LE, %LE]\n", creall(check_data[i])-E_R, creall(check_data[i])+E_R, cimagl(check_data[i])-E_I, cimagl(check_data[i])+E_I);
#endif
	HCQC_error = 1;
      }
    }
    if (count == 0) {
      CHECK_PRINT (" ok\n");
    }
  }
}
