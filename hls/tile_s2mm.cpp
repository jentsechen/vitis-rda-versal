#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>
#define SAMPLE_BIT_WIDTH 64
#define SUB_FFT_SIZE 1024

extern "C" {

void tile_s2mm(ap_int<SAMPLE_BIT_WIDTH> *mem, int period_number,
               int period_size, int block_size, int block_offset,
               int mem_offset,
               hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s) {
#pragma HLS INTERFACE m_axi port = mem offset = slave bundle = gmem
#pragma HLS interface axis port = s
#pragma HLS INTERFACE s_axilite port = mem bundle = control
#pragma HLS INTERFACE s_axilite port = period_number bundle = control
#pragma HLS INTERFACE s_axilite port = period_size bundle = control
#pragma HLS INTERFACE s_axilite port = block_size bundle = control
#pragma HLS INTERFACE s_axilite port = block_offset bundle = control
#pragma HLS INTERFACE s_axilite port = mem_offset bundle = control
#pragma HLS interface s_axilite port = return bundle = control

  for (int period_index = 0; period_index < period_number; period_index++) {
#pragma HLS PIPELINE II = 1
    for (int sample_index = 0; sample_index < block_size; sample_index++) {
      ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x = s.read();
      mem[period_index * period_size + block_offset + sample_index +
          mem_offset] = x.data;
    }
  }
}
}
