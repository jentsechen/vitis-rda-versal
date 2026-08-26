#include "kernel.h"

// Number of int32 samples processed per graph iteration. Must match
// N_SAMPLE in sw/host.cpp and the n_sample argument passed to the
// scale_mm2s/scale_s2mm HLS kernels.
#define N_SAMPLE 16
#define GAIN 3

void scale(input_stream<int32> *in, output_stream<int32> *out) {
  for (int i = 0; i < N_SAMPLE; i++) {
    writeincr(out, readincr(in) * GAIN);
  }
}
