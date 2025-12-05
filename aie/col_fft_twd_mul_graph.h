#ifndef __col_fft_twd_mul_graph__
#define __col_fft_twd_mul_graph__

#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
#include "kernel.h"
#include "sub_fft_par.h"
#include <adf.h>

class ColFftTwdMulGraph : public adf::graph {
private:
  //   xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<
  //       TT_DATA_, TT_TWIDDLE_, TP_POINT_SIZE, TP_FFT_NIFFT, TP_SHIFT,
  //       TP_CASC_LEN, TP_DYN_PT_SIZE, TP_WINDOW_VSIZE, TP_API,
  //       TP_PARALLEL_POWER, TP_USE_WIDGETS> fft_kernel;
  xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<
      TT_DATA_, TT_TWIDDLE_, TP_POINT_SIZE, TP_FFT_NIFFT, TP_SHIFT, TP_CASC_LEN,
      TP_DYN_PT_SIZE, TP_POINT_SIZE, TP_API, TP_PARALLEL_POWER,
      TP_USE_WIDGETS>
      fft_kernel;
  adf::kernel twd_mult_kernel;
  //   adf::kernel distributer_kernel, collector_kernel;

public:
  //   adf::input_gmio col_fft_twd_mul_in;
  adf::input_plio col_fft_twd_mul_in;
  adf::output_gmio col_fft_twd_mul_out;

  ColFftTwdMulGraph() {
    // col_fft_twd_mul_in =
    //     adf::input_gmio::create("col_fft_twd_mul_in", 64, 1000);
    col_fft_twd_mul_in =
        input_plio::create("col_fft_twd_mul_in", adf::plio_64_bits);
    col_fft_twd_mul_out =
        adf::output_gmio::create("col_fft_twd_mul_out", 64, 1000);

    // for (int i = 0; i < 2; i++) {
    //   twd_mult_kernel[i] = adf::kernel::create(twd_mult);
    //   source(twd_mult_kernel[i]) = "./twd_mult.cpp";
    //   runtime<ratio>(twd_mult_kernel[i]) = 0.8;
    // }
    twd_mult_kernel = adf::kernel::create(twd_mult);
    source(twd_mult_kernel) = "./twd_mult.cpp";
    runtime<ratio>(twd_mult_kernel) = 0.8;

    // distributer_kernel = adf::kernel::create(widget_distributer_level0);
    // source(distributer_kernel) = "./widget_distributer.cpp";
    // runtime<ratio>(distributer_kernel) = 0.8;
    // collector_kernel = adf::kernel::create(widget_collector_level0);
    // source(collector_kernel) = "./widget_collector.cpp";
    // runtime<ratio>(collector_kernel) = 0.8;

    // adf::connect<>(col_fft_twd_mul_in.out[0], distributer_kernel.in[0]);
    // for (int i = 0; i < 2; i++) {
    //   adf::connect<>(distributer_kernel.out[i], fft_kernel[i].in[0]);
    //   adf::connect<>(fft_kernel[i].out[0], twd_mult_kernel[i].in[0]);
    //   adf::connect<>(twd_mult_kernel[i].out[0], collector_kernel.in[i]);
    // }
    // adf::connect<>(collector_kernel.out[0], col_fft_twd_mul_out.in[0]);

    adf::connect<>(col_fft_twd_mul_in.out[0], fft_kernel.in[0]);
    adf::connect<>(fft_kernel.out[0], twd_mult_kernel.in[0]);
    adf::connect<>(twd_mult_kernel.out[0], col_fft_twd_mul_out.in[0]);
  }
};

#endif
