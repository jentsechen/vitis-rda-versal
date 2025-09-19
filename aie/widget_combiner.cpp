#include <adf.h>
#include <aie_api/aie.hpp>

// void widget_combiner(adf::input_buffer<cfloat, adf::extents<512>> &in0,
//                      adf::input_buffer<cfloat, adf::extents<512>> &in1,
//                      adf::output_buffer<cfloat, adf::extents<1024>> &out) {
//   auto inIter0 = aie::begin(in0);
//   auto inIter1 = aie::begin(in1);
//   auto outIter = aie::begin(out);

//   for (int i = 0; i < 512; i++) {
//     *outIter++ = *inIter0++;
//     *outIter++ = *inIter1++;
//   }
// }

void widget_combiner(input_stream<cfloat> *in0,
                     input_stream<cfloat> *in1,
                     output_stream<cfloat> *out) {
  for (int i = 0; i < 512; i++) {
    cfloat v0 = readincr(in0);
    cfloat v1 = readincr(in1);
    writeincr(out, v0);
    writeincr(out, v1);
  }
}