#ifndef __range_fft_h__
#define __range_fft_h__
#include "./cnpy/cnpy.h"
#include "xrt/xrt_aie.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_kernel.h"
#include <algorithm>
#include <chrono>
#include <complex>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

const size_t bytes_per_sample = 8;
const size_t fft_size = 1048576;
const size_t n_row = 32;

const size_t data_bytes = 176160;
const size_t pad_bytes  = 8212448;
const size_t stride_bytes = data_bytes + pad_bytes; // 8,388,608 bytes (8MB)
const int num_chunks = 32;

const size_t samples_per_data = data_bytes / sizeof(std::complex<float>); // 22,020 samples
const size_t samples_per_pad  = pad_bytes / sizeof(std::complex<float>);  // 1,026,556 samples
const size_t samples_stride   = samples_per_data + samples_per_pad;

std::vector<std::complex<float>>
range_fft(xrt::device &device, const xrt::uuid &uuid, std::string file_name);

#endif