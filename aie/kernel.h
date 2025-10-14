#ifndef __kernel__
#define __kernel__
#include <adf.h>
#define SUB_FFT_SIZE 1024

void bypass_func(adf::input_buffer<cfloat, adf::extents<1024>> &restrict in,
                 adf::output_buffer<cfloat, adf::extents<1024>> &restrict out);

// void widget_splitter(adf::input_buffer<cfloat, adf::extents<1024>> &in,
//                      adf::output_buffer<cfloat, adf::extents<512>> &out0,
//                      adf::output_buffer<cfloat, adf::extents<512>> &out1);
// void widget_combiner(adf::input_buffer<cfloat, adf::extents<512>> &in0,
//                      adf::input_buffer<cfloat, adf::extents<512>> &in1,
//                      adf::output_buffer<cfloat, adf::extents<1024>> &out);

void widget_splitter(input_stream<cfloat> *in, output_stream<cfloat> *out0,
                     output_stream<cfloat> *out1);
void widget_combiner(input_stream<cfloat> *in0, input_stream<cfloat> *in1,
                     output_stream<cfloat> *out);
                     
void twd_mult(adf::input_buffer<cfloat, adf::extents<SUB_FFT_SIZE>> &in,
              adf::output_buffer<cfloat, adf::extents<SUB_FFT_SIZE>> &out);
#endif
