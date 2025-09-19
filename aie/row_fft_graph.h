#ifndef __row_fft_graph__
#define __row_fft_graph__

#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
#include "kernel.h"
#include "sub_fft_par.h"
#include <adf.h>

class RowFftGraph : public adf::graph {
private:
  xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<
      TT_DATA_, TT_TWIDDLE_, TP_POINT_SIZE, TP_FFT_NIFFT, TP_SHIFT, TP_CASC_LEN,
      TP_DYN_PT_SIZE, TP_WINDOW_VSIZE, TP_API, TP_PARALLEL_POWER,
      TP_USE_WIDGETS>
      fft_kernel;
  kernel splitter, combiner;

public:
  adf::input_plio row_fft_in;
  adf::output_plio row_fft_out;

  RowFftGraph() {
    row_fft_in = input_plio::create("row_fft_in", adf::plio_64_bits);
    row_fft_out = output_plio::create("row_fft_out", adf::plio_64_bits);

    splitter = kernel::create(widget_splitter);
    source(splitter) = "./widget_splitter.cpp";
    runtime<ratio>(splitter) = 0.8;
    combiner = kernel::create(widget_combiner);
    source(combiner) = "./widget_combiner.cpp";
    runtime<ratio>(combiner) = 0.8;

    // adf::connect<>(row_fft_in.out[0], fft_kernel.in[0]);
    // adf::connect<>(fft_kernel.out[0], row_fft_out.in[0]);
    // runtime<ratio>(*fft_kernel.getKernels()) = 0.8;

    adf::connect<>(row_fft_in.out[0], splitter.in[0]);
    adf::connect<>(splitter.out[0], fft_kernel.in[0]);
    adf::connect<>(splitter.out[1], fft_kernel.in[1]);
    adf::connect<>(fft_kernel.out[0], combiner.in[0]);
    adf::connect<>(fft_kernel.out[1], combiner.in[1]);
    adf::connect<>(combiner.out[0], row_fft_out.in[0]);
  }
};

#endif