/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __col_fft_twd_mul_graph__
#define __col_fft_twd_mul_graph__

#include "../../Vitis_Libraries/dsp/L2/include/aie/dds_mixer_graph.hpp"
#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
#include "kernel.h"
#include "sub_fft_par.h"
#include <adf.h>

#define TP_MIXER_MODE 1
#define TP_SSR 1
#define TP_RND 4
#define TP_SAT 1
#define TP_USE_PHASE_RELOAD 1
#define TP_PHASE_RELOAD_API 0
#define TP_USE_PHASE_INC_RELOAD 1

class ColFftTwdMulGraph : public adf::graph {
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
  kernel splitter, combiner;

public:
  //   adf::input_gmio col_fft_twd_mul_in;
  adf::input_plio col_fft_twd_mul_in;
  adf::output_gmio col_fft_twd_mul_out;
  adf::port<input> PhaseIncRTP, PhaseRTP;

  ColFftTwdMulGraph() : dds_kernel(0x00000000) {
    // col_fft_twd_mul_in =
    //     adf::input_gmio::create("col_fft_twd_mul_in", 64, 1000);
    col_fft_twd_mul_in =
        input_plio::create("col_fft_twd_mul_in", adf::plio_64_bits);
    col_fft_twd_mul_out =
        adf::output_gmio::create("col_fft_twd_mul_out", 64, 1000);

    splitter = kernel::create(widget_splitter);
    source(splitter) = "./widget_splitter.cpp";
    runtime<ratio>(splitter) = 0.8;
    combiner = kernel::create(widget_combiner);
    source(combiner) = "./widget_combiner.cpp";
    runtime<ratio>(combiner) = 0.8;

    // adf::connect<>(col_fft_twd_mul_in.out[0], fft_kernel.in[0]);
    // adf::connect<>(fft_kernel.out[0], dds_kernel.in1[0]);
    // adf::connect<>(dds_kernel.out[0], col_fft_twd_mul_out.in[0]);

    adf::connect<>(col_fft_twd_mul_in.out[0], splitter.in[0]);
    adf::connect<>(splitter.out[0], fft_kernel.in[0]);
    adf::connect<>(splitter.out[1], fft_kernel.in[1]);
    adf::connect<>(fft_kernel.out[0], combiner.in[0]);
    adf::connect<>(fft_kernel.out[1], combiner.in[1]);
    adf::connect<>(combiner.out[0], dds_kernel.in1[0]);
    adf::connect<>(dds_kernel.out[0], col_fft_twd_mul_out.in[0]);

    adf::connect<>(PhaseIncRTP, dds_kernel.PhaseIncRTP[0]);
    adf::connect<>(PhaseRTP, dds_kernel.PhaseRTP[0]);
    // runtime<ratio>(*fft_kernel.getKernels()) = 0.8;
    runtime<ratio>(*dds_kernel.getKernels()) = 0.8;
  };
};

#endif
