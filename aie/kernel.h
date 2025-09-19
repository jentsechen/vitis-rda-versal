/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __KERNEL_H__
#define __KERNEL_H__
#include <adf.h>
void bypass_func(adf::input_buffer<cfloat, adf::extents<1024>> &restrict in,
                 adf::output_buffer<cfloat, adf::extents<1024>> &restrict out);

// void widget_splitter(adf::input_buffer<cfloat, adf::extents<1024>> &in,
//                      adf::output_buffer<cfloat, adf::extents<512>> &out0,
//                      adf::output_buffer<cfloat, adf::extents<512>> &out1);
// void widget_combiner(adf::input_buffer<cfloat, adf::extents<512>> &in0,
//                      adf::input_buffer<cfloat, adf::extents<512>> &in1,
//                      adf::output_buffer<cfloat, adf::extents<1024>> &out);
void widget_splitter(input_stream<cfloat> *in,
                     output_stream<cfloat> *out0,
                     output_stream<cfloat> *out1);
void widget_combiner(input_stream<cfloat> *in0,
                     input_stream<cfloat> *in1,
                     output_stream<cfloat> *out);
#endif
