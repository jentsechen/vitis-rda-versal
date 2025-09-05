/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __KERNEL_H__
#define __KERNEL_H__
#include <adf.h>
using namespace adf;
void bypass_func(input_buffer<cfloat, extents<256>> &restrict in,
                 output_buffer<cfloat, extents<256>> &restrict out);
#endif
