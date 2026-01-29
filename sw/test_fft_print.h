#ifndef __test_fft_print_h__
#define __test_fft_print_h__
#include "xrt/xrt_aie.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_kernel.h"
#include <algorithm>
#include <chrono>
#include <complex>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

std::vector<std::complex<float>>
test_fft_print(xrt::device &device, const xrt::uuid &uuid,
               const std::complex<float> *input_data, size_t input_n_sample);

#endif
