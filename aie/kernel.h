#ifndef __kernel__
#define __kernel__
#include <adf.h>
#define SUB_FFT_SIZE 1024

void widget_distributer_level0(input_stream<cfloat> *in,
                               output_stream<cfloat> *out0,
                               output_stream<cfloat> *out1);
void widget_collector_level0(input_stream<cfloat> *in0,
                             input_stream<cfloat> *in1,
                             output_stream<cfloat> *out);

void twd_mult(adf::input_buffer<cfloat, adf::extents<SUB_FFT_SIZE>> &in,
              adf::output_buffer<cfloat, adf::extents<SUB_FFT_SIZE>> &out);
// void twd_mult(input_stream<cfloat> *restrict in,
//               output_stream<cfloat> *restrict out);

#endif