#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>

extern "C" {
void scale_mm2s(ap_int<32> *mem, int n_sample,
                 hls::stream<ap_axis<32, 0, 0, 0>> &s) {
#pragma HLS INTERFACE m_axi port = mem offset = slave bundle = gmem
#pragma HLS INTERFACE axis port = s
#pragma HLS INTERFACE s_axilite port = mem bundle = control
#pragma HLS INTERFACE s_axilite port = n_sample bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

  for (int i = 0; i < n_sample; i++) {
    ap_axis<32, 0, 0, 0> x;
    x.data = mem[i];
    s.write(x);
  }
}
}
