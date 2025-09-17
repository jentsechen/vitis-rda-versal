/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "../../Vitis_Libraries/dsp/L2/include/aie/dds_mixer_graph.hpp"
#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
// #include "kernel.h"
#include <adf.h>

#define TT_DATA_ cfloat
#define TT_TWIDDLE_ cfloat
#define TP_POINT_SIZE 1024
#define TP_FFT_NIFFT 1
#define TP_SHIFT 0
#define TP_CASC_LEN 1
#define TP_DYN_PT_SIZE 0
#define TP_WINDOW_VSIZE TP_POINT_SIZE
#define TP_API 0
#define TP_PARALLEL_POWER 0
#define TP_USE_WIDGETS 0

#define TP_MIXER_MODE 1
#define TP_SSR 1
#define TP_RND 4
#define TP_SAT 1
#define TP_USE_PHASE_RELOAD 1
#define TP_PHASE_RELOAD_API 0
#define TP_USE_PHASE_INC_RELOAD 1

class mygraph : public adf::graph {
private:
  xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<
      TT_DATA_, TT_TWIDDLE_, TP_POINT_SIZE, TP_FFT_NIFFT, TP_SHIFT, TP_CASC_LEN,
      TP_DYN_PT_SIZE, TP_WINDOW_VSIZE, TP_API, TP_PARALLEL_POWER,
      TP_USE_WIDGETS>
      fft_kernel;
  xf::dsp::aie::mixer::dds_mixer::dds_mixer_graph<
      TT_DATA_, TP_WINDOW_VSIZE, TP_MIXER_MODE, TP_API, TP_SSR, TP_RND, TP_SAT,
      TP_USE_PHASE_RELOAD, TP_PHASE_RELOAD_API, TP_USE_PHASE_INC_RELOAD>
      dds_kernel;

public:
  adf::output_gmio gmioOut;
  adf::input_gmio gmioIn;
  adf::port<input> PhaseIncRTP, PhaseRTP;

  mygraph() : dds_kernel(0x00000000) {
    gmioOut = adf::output_gmio::create("gmioOut", 64, 1000);
    gmioIn = adf::input_gmio::create("gmioIn", 64, 1000);

    adf::connect<>(gmioIn.out[0], fft_kernel.in[0]);
    adf::connect<>(fft_kernel.out[0], dds_kernel.in1[0]);
    adf::connect<>(dds_kernel.out[0], gmioOut.in[0]);
    adf::connect<>(PhaseIncRTP, dds_kernel.PhaseIncRTP[0]);
    adf::connect<>(PhaseRTP, dds_kernel.PhaseRTP[0]);
    runtime<ratio>(*fft_kernel.getKernels()) = 0.8;
    runtime<ratio>(*dds_kernel.getKernels()) = 0.8;
  };
};

#endif
