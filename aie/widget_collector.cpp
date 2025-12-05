#include <adf.h>
#include <aie_api/aie.hpp>

void widget_collector_level0(input_stream<cfloat> *in0,
                             input_stream<cfloat> *in1,
                             output_stream<cfloat> *out) {
  for (int i = 0; i < 2048; i++) {
    writeincr(out, readincr(in0));
  }
  for (int i = 0; i < 2048; i++) {
    writeincr(out, readincr(in1));
  }
}
