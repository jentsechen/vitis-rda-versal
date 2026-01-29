#include "kernel.h"
#include "twd_factor_acc.cpp"
#include "twd_factor_acc_1.cpp"
#include "twd_factor_inc.cpp"
// #include "twd_factor_inc_offset.cpp"
#include "twd_factor_table.h"
#include <adf.h>
#include <aie_api/aie.hpp>

void twd_mult(
    adf::input_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &in,
    adf::output_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &out) {
  // 準備向量迭代器 (Vector Iterators)
  auto inIter = aie::begin_vector<VEC_SIZE>(in);
  auto outIter = aie::begin_vector<VEC_SIZE>(out);
  // 取得旋轉因子累加器 (Accumulator) 與增量 (Increment) 的指標
  aie::vector<cfloat, VEC_SIZE> *acc_ptr =
      (aie::vector<cfloat, VEC_SIZE> *)twd_factor_acc;
  const aie::vector<cfloat, VEC_SIZE> *inc_ptr =
      (const aie::vector<cfloat, VEC_SIZE> *)twd_factor_inc;
  for (int bat_idx = 0; bat_idx < N_BATCH; bat_idx++) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
      // 執行複數乘法: 輸入資料 * 旋轉因子
      *outIter++ = aie::mul(*inIter++, acc_ptr[i]);
      // 更新旋轉因子: 累加器 * 增量
      acc_ptr[i] = aie::mul(acc_ptr[i], inc_ptr[i]);
    }
  }
}

void twd_mult_1(
    adf::input_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &in,
    adf::output_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &out) {
  auto inIter = aie::begin_vector<VEC_SIZE>(in);
  auto outIter = aie::begin_vector<VEC_SIZE>(out);
  aie::vector<cfloat, VEC_SIZE> *acc_ptr =
      (aie::vector<cfloat, VEC_SIZE> *)twd_factor_acc_1;
  const aie::vector<cfloat, VEC_SIZE> *inc_ptr =
      (const aie::vector<cfloat, VEC_SIZE> *)twd_factor_inc;
  for (int bat_idx = 0; bat_idx < N_BATCH; bat_idx++) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
      *outIter++ = aie::mul(*inIter++, acc_ptr[i]);
      acc_ptr[i] = aie::mul(acc_ptr[i], inc_ptr[i]);
    }
  }
}

// void twd_mult_2_paral(
//     adf::input_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &in_0,
//     adf::input_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &in_1,
//     adf::output_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &out_0,
//     adf::output_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &out_1) {
//   auto inIter_0 = aie::begin_vector<VEC_SIZE>(in_0);
//   auto inIter_1 = aie::begin_vector<VEC_SIZE>(in_1);
//   auto outIter_0 = aie::begin_vector<VEC_SIZE>(out_0);
//   auto outIter_1 = aie::begin_vector<VEC_SIZE>(out_1);
//   const aie::vector<cfloat, VEC_SIZE> *inc_ptr =
//       (const aie::vector<cfloat, VEC_SIZE> *)twd_factor_inc;
//   const aie::vector<cfloat, VEC_SIZE> *inc_offset_ptr =
//       (const aie::vector<cfloat, VEC_SIZE> *)twd_factor_inc_offset;
//   aie::vector<cfloat, VEC_SIZE> *acc_ptr_0 =
//       (aie::vector<cfloat, VEC_SIZE> *)twd_factor_acc;
//   for (int bat_idx = 0; bat_idx < N_BATCH; bat_idx++) {
//     for (int i = 0; i < BLOCK_SIZE; i++) {
//       *outIter_0++ = aie::mul(*inIter_0++, acc_ptr_0[i]);
//       aie::vector<cfloat, VEC_SIZE> acc_ptr_1 =
//           aie::mul(acc_ptr_0[i], inc_offset_ptr[0]);
//       *outIter_1++ = aie::mul(*inIter_1++, acc_ptr_1);
//       acc_ptr_0[i] = aie::mul(acc_ptr_0[i], inc_ptr[i]);
//     }
//   }
// }
