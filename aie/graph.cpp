/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "graph.h"
#include "row_fft_graph.h"
#include <adf.h>
#include <fstream>
#include <unistd.h>
using namespace adf;

mygraph gr;
RowFFTGraph row_fft_graph;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv) {
  gr.init();
  gr.run(1);
  gr.end();
  row_fft_graph.init();
  row_fft_graph.run(1);
  row_fft_graph.end();
  return 0;
};
#endif
