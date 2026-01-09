#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>
#define SAMPLE_BIT_WIDTH 64
#define ROW_SIZE 1024
#define COL_SIZE 64

void direct_transpose(ap_int<SAMPLE_BIT_WIDTH> *mem, int mem_offset,
                    hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s) {
#pragma HLS INTERFACE m_axi port = mem offset = slave bundle = gmem
#pragma HLS INTERFACE axis port = s
#pragma HLS INTERFACE s_axilite port = mem bundle = control
#pragma HLS INTERFACE s_axilite port = mem_offset bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

  for (int col_index = 0; col_index < COL_SIZE; col_index++) {
    for (int row_index = 0; row_index < ROW_SIZE; row_index++) {
#pragma HLS PIPELINE II = 1
      ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x;
      x.data = mem[row_index * 1024 + col_index + mem_offset];
      s.write(x);
    }
  }
}
