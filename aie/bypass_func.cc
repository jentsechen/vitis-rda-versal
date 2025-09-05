/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"
using namespace adf;
// alignas(aie::vector_decl_align) int32 weights[8] = {1, 2, 3, 4, 5, 6, 7, 8};

void bypass_func(input_buffer<cfloat, extents<256>> &restrict in,
                 output_buffer<cfloat, extents<256>> &restrict out) {
  const int n_sample_per_vector = 4; // 256 / n_bit_per_sample
  auto inIter = aie::begin_vector<n_sample_per_vector>(in);
  auto outIter = aie::begin_vector<n_sample_per_vector>(out);

  for (unsigned i = 0; i < 256 / n_sample_per_vector; i++) {
    *outIter++ = *inIter++;
  }
}
