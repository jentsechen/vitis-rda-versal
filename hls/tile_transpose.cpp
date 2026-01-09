#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>
#define SAMPLE_BIT_WIDTH 64
#define ROW_SIZE 1024
#define COL_SIZE 64

ap_uint<SAMPLE_BIT_WIDTH> uram_array[ROW_SIZE * COL_SIZE];

void tile_transpose(ap_int<SAMPLE_BIT_WIDTH> *mem, int mem_offset,
                    hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s) {
#pragma HLS bind_storage variable = uram_array type = RAM_T2P impl = URAM
  // #pragma HLS INTERFACE m_axi port = ddr_in bundle = DDR_R

#pragma HLS INTERFACE m_axi port = mem offset = slave bundle = gmem
#pragma HLS INTERFACE axis port = s
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

  for (int col_index = 0; col_index < COL_SIZE; col_index++) {
    for (int row_index = 0; row_index < ROW_SIZE; row_index++) {
#pragma HLS PIPELINE II = 1
      ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x;
      x.data = uram_array[row_index * COL_SIZE + col_index];
      //   x.last = (col_index == COL_SIZE - 1 && row_index == ROW_SIZE - 1);
      s.write(x);
    }
  }
}
