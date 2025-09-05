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

const int ITERATION = 8;
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
  memcpy(dinArray, DataInput, BLOCK_SIZE_in_Bytes);
  auto end3 = std::chrono::high_resolution_clock::now();
  cout << "move data to DDR: "
       << std::chrono::duration_cast<std::chrono::microseconds>(end3 - start3)
              .count()
       << " us" << endl;

  auto dout_buffer =
      xrt::aie::bo(device, BLOCK_SIZE_in_Bytes, xrt::bo::flags::normal, 0);
  auto *doutArray = dout_buffer.map<__uint64_t *>();

  auto start4 = std::chrono::high_resolution_clock::now();
  auto ghdl = xrt::graph(device, uuid, "gr");
  //   din_buffer.async("gr.gmioIn", XCL_BO_SYNC_BO_GMIO_TO_AIE,
  //   BLOCK_SIZE_in_Bytes,
  //                    0);
  //   ghdl.run(ITERATION);
  auto end4 = std::chrono::high_resolution_clock::now();
  cout << "initialize graph: "
       << std::chrono::duration_cast<std::chrono::microseconds>(end4 - start4)
              .count()
       << " us" << endl;

  int offset = 0;
  for (int iter = 0; iter < ITERATION; ++iter) {
    cout << "iter: " << iter << endl;
    auto start5 = std::chrono::high_resolution_clock::now();
    int32_t val = iter << 11;
    int32_t neg_val = -val;
    uint32_t neg_val_hex = static_cast<uint32_t>(neg_val);
    ghdl.update("gr.PhaseIncRTP", neg_val_hex);
    // ghdl.update("gr.PhaseIncRTP", iter << 11);
    uint32_t phase =
        (iter == 0) ? 0 : ((iter - 1) << 11) * (n_sample_per_iter - 1);
    cout << "phase: " << phase << endl;
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
    auto end5 = std::chrono::high_resolution_clock::now();
    cout << "computation: "
         << std::chrono::duration_cast<std::chrono::microseconds>(end5 - start5)
                .count()
         << " us" << endl;
  }
  ghdl.end();
  //   auto dout_buffer_run = dout_buffer.async(
  //       "gr.gmioOut", XCL_BO_SYNC_BO_AIE_TO_GMIO, BLOCK_SIZE_in_Bytes, 0);
  //   // PS can do other tasks here when data is transferring
  //   dout_buffer_run.wait(); // Wait for gmioOut to complete

  auto start6 = std::chrono::high_resolution_clock::now();
  std::array<std::complex<float>, ITERATION * n_sample_per_iter> output;
  memcpy(output.data(), doutArray, output.size() * n_byte_per_sample);
  auto end6 = std::chrono::high_resolution_clock::now();
  cout << "move to PS: "
       << std::chrono::duration_cast<std::chrono::microseconds>(end6 - start6)
              .count()
       << " us" << endl;

  auto start7 = std::chrono::high_resolution_clock::now();
  cnpy::npy_save("output.npy", output.data(), {ITERATION * n_sample_per_iter},
                 "w");
  auto end7 = std::chrono::high_resolution_clock::now();
  cout << "save npy: "
       << std::chrono::duration_cast<std::chrono::microseconds>(end7 - start7)
              .count()
       << " us" << endl;

  std::cout << "Done!" << std::endl;

  return 0;
};
