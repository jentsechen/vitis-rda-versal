/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "./cnpy/cnpy.cpp"
#include "./cnpy/cnpy.h"
#include "xrt/xrt_aie.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_kernel.h"
#include <adf.h>
#include <array>
#include <chrono>
#include <complex>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace adf;
using namespace std;

const int ITERATION = 1024;
const int n_byte_per_sample = 8;
const int n_sample_per_iter = 1024;
const int BLOCK_SIZE_in_Bytes =
    ITERATION * n_sample_per_iter * n_byte_per_sample;

int main(int argc, char **argv) {
  auto start1 = std::chrono::high_resolution_clock::now();
  cnpy::NpyArray arr = cnpy::npy_load(argv[2]);
  assert(arr.shape[0] > 0 && arr.shape[1] > 0);
  //   std::cout << "number of values: " << arr.shape[1] << std::endl;
  const complex<float> *DataInput = arr.data<complex<float>>();
  auto end1 = std::chrono::high_resolution_clock::now();
  cout << "load npy: "
       << std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1)
              .count()
       << " us" << endl;

  auto start2 = std::chrono::high_resolution_clock::now();
  char *xclbinFilename = argv[1];
  auto device = xrt::device(0);
  auto uuid = device.load_xclbin(xclbinFilename);
  auto end2 = std::chrono::high_resolution_clock::now();
  cout << "load xclbin: "
       << std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2)
              .count()
       << " us" << endl;

  auto start3 = std::chrono::high_resolution_clock::now();
  auto din_buffer =
      xrt::aie::bo(device, BLOCK_SIZE_in_Bytes, xrt::bo::flags::normal, 0);
  auto *dinArray = din_buffer.map<__uint64_t *>();
  //   memset(dinArray, 0,
  //          std::pow(2, std::ceil(std::log2(arr.shape[1]))) *
  //          n_byte_per_sample);
  memset(dinArray, 0, BLOCK_SIZE_in_Bytes);
  //   memcpy(dinArray, DataInput, arr.shape[1] * n_byte_per_sample);
  //   memcpy(dinArray, DataInput, BLOCK_SIZE_in_Bytes);
  int data_size = arr.shape[1] * n_byte_per_sample;
  memcpy(dinArray, DataInput, min(data_size, BLOCK_SIZE_in_Bytes));
  auto end3 = std::chrono::high_resolution_clock::now();
  cout << "move data to DDR: "
       << std::chrono::duration_cast<std::chrono::microseconds>(end3 - start3)
              .count()
       << " us" << endl;

  auto dout_buffer =
      xrt::aie::bo(device, BLOCK_SIZE_in_Bytes, xrt::bo::flags::normal, 0);
  auto *doutArray = dout_buffer.map<__uint64_t *>();
  memset(doutArray, 0, BLOCK_SIZE_in_Bytes);

  auto start4 = std::chrono::high_resolution_clock::now();
  auto ghdl = xrt::graph(device, uuid, "gr");
  auto end4 = std::chrono::high_resolution_clock::now();
  cout << "initialize graph: "
       << std::chrono::duration_cast<std::chrono::microseconds>(end4 - start4)
              .count()
       << " us" << endl;

  int offset = 0, iter_sum = 0;
  for (int iter = 0; iter < ITERATION; ++iter) {
    // cout << "iter: " << iter << endl;
    auto start5 = std::chrono::high_resolution_clock::now();
    int32_t val = iter << 11;
    int32_t neg_val = -val;
    uint32_t neg_val_hex = static_cast<uint32_t>(neg_val);
    ghdl.update("gr.PhaseIncRTP", neg_val_hex);
    uint32_t phase = (iter == 0 || iter == 1)
                         ? 0
                         : (iter_sum << 11) * (n_sample_per_iter - 1);
    ghdl.update("gr.PhaseRTP", phase);
    din_buffer.async("gr.gmioIn", XCL_BO_SYNC_BO_GMIO_TO_AIE,
                     n_sample_per_iter * n_byte_per_sample, offset);
    ghdl.run(1);
    ghdl.wait();
    auto dout_buffer_run =
        dout_buffer.async("gr.gmioOut", XCL_BO_SYNC_BO_AIE_TO_GMIO,
                          n_sample_per_iter * n_byte_per_sample, offset);
    dout_buffer_run.wait();
    offset += (n_sample_per_iter * n_byte_per_sample);
    iter_sum += iter;
    auto end5 = std::chrono::high_resolution_clock::now();
    // if (iter < 30) {
    //   cout << "computation: "
    //        << std::chrono::duration_cast<std::chrono::microseconds>(end5 -
    //                                                                 start5)
    //               .count()
    //        << " us" << endl;
    // }
  }
  ghdl.end();

  auto transpose_dout_buffer =
      xrt::bo(device, BLOCK_SIZE_in_Bytes, xrt::bo::flags::normal, 0);
  auto *transposeDoutArray = transpose_dout_buffer.map<__uint64_t *>();
  xrt::bo row_fft_mm2s_buffer = xrt::bo{dout_buffer};
  row_fft_mm2s_buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  auto row_fft_strided_mm2s =
      xrt::kernel(device, uuid, "fft_strided_mm2s:{fft_strided_mm2s_0}");
  auto row_fft_strided_s2mm =
      xrt::kernel(device, uuid, "fft_strided_s2mm:{fft_strided_s2mm_0}");
  auto row_fft_graph_hdl = xrt::graph(device, uuid, "row_fft_graph");

  for (int iter = 0; iter < ITERATION; iter++) {
    cout << "iter: " << iter << endl;
    auto row_fft_strided_mm2s_rhdl =
        row_fft_strided_mm2s(row_fft_mm2s_buffer, iter);
    auto row_fft_strided_s2mm_rhdl =
        row_fft_strided_s2mm(transpose_dout_buffer, iter);
    row_fft_graph_hdl.run(1);
    row_fft_strided_mm2s_rhdl.wait();
    row_fft_strided_s2mm_rhdl.wait();
    row_fft_graph_hdl.wait();
    transpose_dout_buffer.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  }

  std::complex<float> *out_ptr =
      reinterpret_cast<std::complex<float> *>(doutArray);
  for (int i = 0; i < 10; i++) {
    printf("output[%d]=%f+1j*%f\n", i, out_ptr[i].real(), out_ptr[i].imag());
  }
  std::complex<float> *trans_out_ptr =
      reinterpret_cast<std::complex<float> *>(transposeDoutArray);
  for (int i = 0; i < 10; i++) {
    printf("transpose output[%d]=%f+1j*%f\n", i, trans_out_ptr[i].real(),
           trans_out_ptr[i].imag());
  }

  auto start7 = std::chrono::high_resolution_clock::now();
  cnpy::npy_save("output.npy",
                 reinterpret_cast<std::complex<float> *>(transposeDoutArray),
                 {ITERATION * n_sample_per_iter}, "w");
  auto end7 = std::chrono::high_resolution_clock::now();
  cout << "save npy: "
       << std::chrono::duration_cast<std::chrono::microseconds>(end7 - start7)
              .count()
       << " us" << endl;

  std::cout << "Done!" << std::endl;

  return 0;
};
