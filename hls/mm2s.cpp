#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>
#define SAMPLE_BIT_WIDTH 64
#define SUB_FFT_SIZE 1024

extern "C" {
void mm2s(ap_int<SAMPLE_BIT_WIDTH> *mem, int n_sample, int mem_offset,
          hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s) {
#pragma HLS INTERFACE m_axi port = mem offset = slave bundle = gmem
#pragma HLS INTERFACE axis port = s
#pragma HLS INTERFACE s_axilite port = mem bundle = control
#pragma HLS INTERFACE s_axilite port = n_sample bundle = control
#pragma HLS INTERFACE s_axilite port = mem_offset bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

  for (int i = 0; i < n_sample; i++) {
    ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x;
    x.data = mem[i + mem_offset];
    s.write(x);
  }
}
}
