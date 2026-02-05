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

// FFT config
const size_t n_iter = 1024;
const size_t n_paral = 2;
const size_t n_byte_per_sample = 8;
const size_t n_sample_per_iter = 1024;
const size_t n_sample_per_paral = n_iter * n_sample_per_iter / n_paral;
const size_t block_size_in_byte =
    n_iter * n_sample_per_iter * n_byte_per_sample;
const size_t n_test_row = 32;

size_t get_header_len(std::string file_name);

std::vector<std::complex<float>>
range_fft(xrt::device &device, const xrt::uuid &uuid, std::string file_name);

#endif