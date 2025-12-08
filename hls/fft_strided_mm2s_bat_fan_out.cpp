#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>
#define SAMPLE_BIT_WIDTH 64
#define SUB_FFT_SIZE 1024
#define N_BATCH 2

extern "C" {
void fft_strided_mm2s_bat_fan_out(
    ap_int<SAMPLE_BIT_WIDTH> *mem, int strided_index,
    hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s0,
    hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s1,
    hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s2,
    hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s3) {
#pragma HLS INTERFACE m_axi port = mem offset = slave bundle = gmem
#pragma HLS INTERFACE axis port = s0
#pragma HLS INTERFACE axis port = s1
#pragma HLS INTERFACE axis port = s2
#pragma HLS INTERFACE axis port = s3
#pragma HLS INTERFACE s_axilite port = mem bundle = control
#pragma HLS INTERFACE s_axilite port = strided_index bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

  for (int batch_idx = 0; batch_idx < N_BATCH; batch_idx++) {
    for (int i = 0; i < SUB_FFT_SIZE; i++) {
      ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x0;
      x0.data = mem[i * SUB_FFT_SIZE + strided_index + batch_idx];
      s0.write(x0);
      ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x1;
      x1.data = mem[i * SUB_FFT_SIZE + strided_index + batch_idx + 256];
      s1.write(x1);
      ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x2;
      x2.data = mem[i * SUB_FFT_SIZE + strided_index + batch_idx + 512];
      s2.write(x2);
      ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x3;
      x3.data = mem[i * SUB_FFT_SIZE + strided_index + batch_idx + 768];
      s3.write(x3);
    }
  }
}
}
