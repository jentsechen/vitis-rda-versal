#ifndef __col_fft_twd_mul_graph__
#define __col_fft_twd_mul_graph__

// #include "../../Vitis_Libraries/dsp/L2/include/aie/dds_mixer_graph.hpp"
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
  adf::kernel twd_mult_kernel;

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

    twd_mult_kernel = adf::kernel::create(twd_mult);
    source(twd_mult_kernel) = "./twd_mult.cpp";
    runtime<ratio>(twd_mult_kernel) = 0.8;
    runtime<ratio>(*fft_kernel.getKernels()) = 0.8;

    adf::connect<>(col_fft_twd_mul_in.out[0], fft_kernel.in[0]);
    adf::connect<>(fft_kernel.out[0], twd_mult_kernel.in[0]);
    adf::connect<>(twd_mult_kernel.out[0], col_fft_twd_mul_out.in[0]);
  }
};

#endif
