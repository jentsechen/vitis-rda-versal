#ifndef __row_fft_graph__
#define __row_fft_graph__

#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
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

class RowFftGraph : public adf::graph {
private:
  xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<
      TT_DATA_, TT_TWIDDLE_, TP_POINT_SIZE, TP_FFT_NIFFT, TP_SHIFT, TP_CASC_LEN,
      TP_DYN_PT_SIZE, TP_WINDOW_VSIZE, TP_API, TP_PARALLEL_POWER,
      TP_USE_WIDGETS>
      fft_kernel;

public:
  adf::input_plio row_fft_in;
  adf::output_plio row_fft_out;
  RowFftGraph() {
    row_fft_in = input_plio::create("row_fft_in", adf::plio_64_bits);
    row_fft_out = output_plio::create("row_fft_out", adf::plio_64_bits);
    adf::connect<>(row_fft_in.out[0], fft_kernel.in[0]);
    adf::connect<>(fft_kernel.out[0], row_fft_out.in[0]);
    runtime<ratio>(*fft_kernel.getKernels()) = 0.8;
  }
};

#endif