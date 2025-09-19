#include <adf.h>
#include <aie_api/aie.hpp>

// void widget_splitter(adf::input_buffer<cfloat, adf::extents<1024>> &in,
//                      adf::output_buffer<cfloat, adf::extents<512>> &out0,
//                      adf::output_buffer<cfloat, adf::extents<512>> &out1) {
//   auto inIter = aie::begin(in);
//   auto outIter0 = aie::begin(out0);
//   auto outIter1 = aie::begin(out1);

//   for (int i = 0; i < 1024; i++, inIter++) {
//     if (i % 2 == 0) {
//       *outIter0++ = *inIter;
//     } else {
//       *outIter1++ = *inIter;
//     }
//   }
// }

void widget_splitter(input_stream<cfloat> *in,
                     output_stream<cfloat> *out0,
                     output_stream<cfloat> *out1) {
  for (int i = 0; i < 1024; i++) {
    cfloat v = readincr(in);
    if (i % 2 == 0) {
      writeincr(out0, v);
    } else {
      writeincr(out1, v);
    }
  }
}
