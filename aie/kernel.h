#ifndef __kernel__
#define __kernel__
#include <adf.h>
#include <aie_api/aie.hpp>
#define SUB_FFT_SIZE 1024
#define VEC_SIZE 8
#define BLOCK_SIZE SUB_FFT_SIZE / VEC_SIZE
#define N_BATCH 2

void widget_distributer_level0(input_stream<cfloat> *in,
                               output_stream<cfloat> *out0,
                               output_stream<cfloat> *out1);
void widget_collector_level0(input_stream<cfloat> *in0,
                             input_stream<cfloat> *in1,
                             output_stream<cfloat> *out);

// void twd_mult(input_stream<cfloat> *restrict in,
//               output_stream<cfloat> *restrict out);
// void twd_mult(
//     adf::input_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &in,
//     output_stream<cfloat> *restrict out);
void twd_mult(
    adf::input_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &in,
    adf::output_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &out);
void twd_mult_1(
    adf::input_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &in,
    adf::output_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &out);
void twd_mult_2_paral(
    adf::input_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &in_0,
    adf::input_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &in_1,
    adf::output_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &out_0,
    adf::output_buffer<cfloat, adf::extents<SUB_FFT_SIZE * N_BATCH>> &out_1);

#endif