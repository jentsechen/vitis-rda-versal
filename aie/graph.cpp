/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "graph.h"
#include <adf.h>
#include <fstream>
#include <unistd.h>
using namespace adf;

mygraph gr;

#if defined(__AIESIM__) || defined(__X86SIM__)
const int ITERATION = 4;
const int n_bit_per_sample = 8;
const int n_sample_per_iter = 256;
const int BLOCK_SIZE_in_Bytes =
    ITERATION * n_sample_per_iter * n_bit_per_sample;
int main(int argc, char **argv) {
  gr.init();
  gr.run(ITERATION);
  gr.end();
  return 0;
};
#endif
