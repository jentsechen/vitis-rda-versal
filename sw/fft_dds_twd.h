#ifndef __fft_dds_twd_h__
#define __fft_dds_twd_h__
#include "xrt/xrt_aie.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_kernel.h"
#include <algorithm>
#include <complex>
#include <cstdint>
#include <cstring>
#include <vector>

const size_t n_iter = 1024;
const size_t n_byte_per_sample = 8;
const size_t n_sample_per_iter = 1024;
const size_t block_size_in_byte =
    n_iter * n_sample_per_iter * n_byte_per_sample;

std::vector<std::complex<float>>
fft_dds_twd(xrt::device &device, const xrt::uuid &uuid,
            const std::complex<float> *input_data, size_t input_n_sample);
#endif