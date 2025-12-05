#include <adf.h>
#include <aie_api/aie.hpp>

void widget_distributer_level0(input_stream<cfloat> *in,
                               output_stream<cfloat> *out0,
                               output_stream<cfloat> *out1) {
  for (int i = 0; i < 2048; i++) {
    writeincr(out0, readincr(in));
  }
  for (int i = 0; i < 2048; i++) {
    writeincr(out1, readincr(in));
  }
}
