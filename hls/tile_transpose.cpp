#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>
#define SAMPLE_BIT_WIDTH 64
#define ROW_SIZE 1024
#define COL_SIZE 64

ap_uint<SAMPLE_BIT_WIDTH> uram_array[ROW_SIZE * COL_SIZE];

void tile_transpose(ap_int<SAMPLE_BIT_WIDTH> *mem, int mem_offset,
                    hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s0,
                    hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s1) {
#pragma HLS bind_storage variable = uram_array type = RAM_T2P impl = URAM

#pragma HLS INTERFACE m_axi port = mem offset = slave bundle = gmem
#pragma HLS INTERFACE axis port = s0
#pragma HLS INTERFACE axis port = s1
#pragma HLS INTERFACE s_axilite port = mem bundle = control
#pragma HLS INTERFACE s_axilite port = mem_offset bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

  for (int row_index = 0; row_index < ROW_SIZE; row_index++) {
    for (int col_index = 0; col_index < COL_SIZE; col_index++) {
#pragma HLS PIPELINE II = 1
      ap_uint<SAMPLE_BIT_WIDTH> data =
          mem[row_index * 1024 + col_index + mem_offset];
      uram_array[row_index * COL_SIZE + col_index] = data;
    }
  }

  for (int col_index = 0; col_index < COL_SIZE / 2; col_index++) {
    for (int row_index = 0; row_index < ROW_SIZE; row_index++) {
#pragma HLS PIPELINE II = 1
      ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x0;
      x0.data = uram_array[row_index * COL_SIZE + col_index];
      s0.write(x0);
      ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x1;
      x1.data = uram_array[row_index * COL_SIZE + col_index + 32];
      s1.write(x1);
    }
  }
}
