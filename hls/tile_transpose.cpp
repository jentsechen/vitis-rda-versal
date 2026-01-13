#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>
#define SAMPLE_BIT_WIDTH 64
#define ROW_SIZE 1024
#define COL_SIZE 64

void write_uram(ap_int<SAMPLE_BIT_WIDTH> *mem, int mem_offset,
                ap_uint<SAMPLE_BIT_WIDTH> uram_array[ROW_SIZE * COL_SIZE]) {
  for (int row_index = 0; row_index < ROW_SIZE; row_index++) {
    for (int col_index = 0; col_index < COL_SIZE; col_index++) {
#pragma HLS PIPELINE II = 1
      uram_array[row_index * COL_SIZE + col_index] =
          mem[row_index * 1024 + col_index + mem_offset];
    }
  }
}

void read_uram(ap_uint<SAMPLE_BIT_WIDTH> uram_array[ROW_SIZE * COL_SIZE],
               hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s0,
               hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s1) {
  for (int col_index = 0; col_index < COL_SIZE / 2; col_index++) {
    for (int row_index = 0; row_index < ROW_SIZE; row_index++) {
#pragma HLS PIPELINE II = 1
      ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0> x0, x1;
      x0.data = uram_array[row_index * COL_SIZE + col_index];
      x1.data = uram_array[row_index * COL_SIZE + col_index + 32];
      s0.write(x0);
      s1.write(x1);
    }
  }
}

void tile_transpose(ap_int<SAMPLE_BIT_WIDTH> *mem,
                    hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s0,
                    hls::stream<ap_axis<SAMPLE_BIT_WIDTH, 0, 0, 0>> &s1) {
#pragma HLS INTERFACE m_axi port = mem offset = slave bundle = gmem
#pragma HLS INTERFACE axis port = s0
#pragma HLS INTERFACE axis port = s1
#pragma HLS INTERFACE s_axilite port = mem bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

  for (int tile_index = 0; tile_index < 16; tile_index++) {
    ap_uint<SAMPLE_BIT_WIDTH> local_uram[ROW_SIZE * COL_SIZE];
#pragma HLS bind_storage variable = local_uram type = RAM_T2P impl = URAM

#pragma HLS DATAFLOW
    write_uram(mem, COL_SIZE * tile_index, local_uram);
    read_uram(local_uram, s0, s1);
  }
}
