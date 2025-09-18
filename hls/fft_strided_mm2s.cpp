#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>
#define SAMPLE_BIT_WIDTH 64
#define SUB_FFT_SIZE 1024

extern "C" {
void fft_strided_mm2s(ap_int<SAMPLE_BIT_WIDTH> *mem, int strided_index,
                      hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s) {
#pragma HLS INTERFACE m_axi port = mem offset = slave bundle = gmem
#pragma HLS INTERFACE axis port = s
#pragma HLS INTERFACE s_axilite port = mem bundle = control
#pragma HLS INTERFACE s_axilite port = strided_index bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

  for (int i = 0; i < SUB_FFT_SIZE; i++) {
#pragma HLS LOOP_TRIPCOUNT min = SUB_FFT_SIZE max = SUB_FFT_SIZE
#pragma HLS PIPELINE II = 1
    ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x;
    x.data = mem[i * SUB_FFT_SIZE + strided_index];
    s.write(x);
  }
}
}
