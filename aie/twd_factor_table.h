#ifndef __twd_factor_table__
#define __twd_factor_table__
#include "kernel.h"
#include <adf.h>
#include <aie_api/aie.hpp>

extern const cfloat twd_factor_inc[SUB_FFT_SIZE];
// extern const cfloat twd_factor_inc_offset[VEC_SIZE];
extern cfloat twd_factor_acc[SUB_FFT_SIZE];
extern cfloat twd_factor_acc_1[SUB_FFT_SIZE];
// alignas(aie::vector<cfloat, VEC_SIZE>) cfloat twd_factor_acc[SUB_FFT_SIZE];
// alignas(aie::vector<cfloat, VEC_SIZE>) cfloat twd_factor_acc_1[SUB_FFT_SIZE];
// alignas(aie::vector<cfloat, VEC_SIZE>) const cfloat
//     twd_factor_inc[SUB_FFT_SIZE];
#endif