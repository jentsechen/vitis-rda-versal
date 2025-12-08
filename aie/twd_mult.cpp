#include "twd_factor_table.cpp"
#include "twd_factor_table.h"
#include <adf.h>
#include <aie_api/aie.hpp>

#define SUB_FFT_SIZE 1024
#define VEC_SIZE 8
#define BLOCK_SIZE SUB_FFT_SIZE / VEC_SIZE
#define N_BATCH 2

// void twd_mult(input_stream<cfloat> *restrict in,
//               output_stream<cfloat> *restrict out) {
//   aie::vector<cfloat, VEC_SIZE> *acc_ptr =
//       (aie::vector<cfloat, VEC_SIZE> *)twd_factor_acc;
//   const aie::vector<cfloat, VEC_SIZE> *inc_ptr =
//       (const aie::vector<cfloat, VEC_SIZE> *)twd_factor_inc;
//   for (int i = 0; i < BLOCK_SIZE; i++) {
//     aie::vector<cfloat, VEC_SIZE> data_in = readincr_v<VEC_SIZE>(in);
//     aie::vector<cfloat, VEC_SIZE> data_out = aie::mul(data_in, acc_ptr[i]);
//     writeincr_v(out, data_out);
//   }
//   for (int i = 0; i < BLOCK_SIZE; i++) {
//     acc_ptr[i] = aie::mul(acc_ptr[i], inc_ptr[i]);
//   }
// }

void twd_mult(
    adf::input_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &in,
    adf::output_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &out) {
  auto inIter = aie::begin_vector<VEC_SIZE>(in);
  auto outIter = aie::begin_vector<VEC_SIZE>(out);
  aie::vector<cfloat, VEC_SIZE> *acc_ptr =
      (aie::vector<cfloat, VEC_SIZE> *)twd_factor_acc;
  const aie::vector<cfloat, VEC_SIZE> *inc_ptr =
      (const aie::vector<cfloat, VEC_SIZE> *)twd_factor_inc;
  for (int bat_idx = 0; bat_idx < N_BATCH; bat_idx++) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
      *outIter++ = aie::mul(*inIter++, acc_ptr[i]);
      acc_ptr[i] = aie::mul(acc_ptr[i], inc_ptr[i]);
    }
  }
}
