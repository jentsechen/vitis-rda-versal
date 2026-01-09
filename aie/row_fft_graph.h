#ifndef __row_fft_graph__
#define __row_fft_graph__

#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
#include "kernel.h"
#include "sub_fft_par.h"
#include <adf.h>

class RowFftGraph : public adf::graph {
private:
  xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<
      cfloat,                  // TT_DATA
      cfloat,                  // TT_TWIDDLE
      TP_POINT_SIZE,           // TP_POINT_SIZE
      1,                       // TP_FFT_NIFFT
      0,                       // TP_SHIFT
      1,                       // TP_CASC_LEN
      0,                       // TP_DYN_PT_SIZE
      TP_POINT_SIZE * N_BATCH, // TP_WINDOW_VSIZE
      0,                       // TP_API
      0,                       // TP_PARALLEL_POWER
      0                        // TP_USE_WIDGETS
      >
      fft_kernel[N_PARAL];

public:
  adf::input_plio row_fft_in_0, row_fft_in_1;
  adf::output_plio row_fft_out_0, row_fft_out_1;

  RowFftGraph() {
    row_fft_in_0 = input_plio::create("row_fft_in_0", adf::plio_64_bits);
    row_fft_in_1 = input_plio::create("row_fft_in_1", adf::plio_64_bits);
    row_fft_out_0 = output_plio::create("row_fft_out_0", adf::plio_64_bits);
    row_fft_out_1 = output_plio::create("row_fft_out_1", adf::plio_64_bits);

    adf::connect<>(row_fft_in_0.out[0], fft_kernel[0].in[0]);
    adf::connect<>(row_fft_in_1.out[0], fft_kernel[1].in[0]);
    adf::connect<>(fft_kernel[0].out[0], row_fft_out_0.in[0]);
    adf::connect<>(fft_kernel[1].out[0], row_fft_out_1.in[0]);
  }
};

#endif