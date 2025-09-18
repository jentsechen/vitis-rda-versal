#include "fft_dds_twd.h"

std::vector<std::complex<float>>
fft_dds_twd(xrt::device &device, const xrt::uuid &uuid,
            const std::complex<float> *input_data, size_t input_n_sample) {
  // AIE kernels
  auto col_fft_twd_mul_rhdl = xrt::graph(device, uuid, "col_fft_twd_mul_graph");
  auto row_fft_graph_hdl = xrt::graph(device, uuid, "row_fft_graph");

  // PL kernels
  auto col_fft_strided_mm2s =
      xrt::kernel(device, uuid, "fft_strided_mm2s:{fft_strided_mm2s_1}");
  auto row_fft_strided_mm2s =
      xrt::kernel(device, uuid, "fft_strided_mm2s:{fft_strided_mm2s_0}");
  auto row_fft_strided_s2mm =
      xrt::kernel(device, uuid, "fft_strided_s2mm:{fft_strided_s2mm_0}");

  // buffers
  auto din_buffer =
      xrt::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  auto dout_buffer =
      xrt::aie::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  xrt::bo row_fft_mm2s_buffer = xrt::bo{dout_buffer};
  auto transpose_dout_buffer =
      xrt::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);

  // init. input buffer
  auto *dinArray = din_buffer.map<__uint64_t *>();
  memset(dinArray, 0, block_size_in_byte);
  memcpy(dinArray, input_data,
         std::min(input_n_sample * n_byte_per_sample, block_size_in_byte));

  auto *transposeDoutArray = transpose_dout_buffer.map<__uint64_t *>();

  int offset = 0, iter_sum = 0;
  for (int iter = 0; iter < n_iter; ++iter) {
    int32_t val = iter << 11;
    int32_t neg_val = -val;
    uint32_t neg_val_hex = static_cast<uint32_t>(neg_val);
    col_fft_twd_mul_rhdl.update("col_fft_twd_mul_graph.PhaseIncRTP",
                                neg_val_hex);
    uint32_t phase = (iter == 0 || iter == 1)
                         ? 0
                         : (iter_sum << 11) * (n_sample_per_iter - 1);
    col_fft_twd_mul_rhdl.update("col_fft_twd_mul_graph.PhaseRTP", phase);
    auto col_fft_strided_mm2s_rhdl = col_fft_strided_mm2s(din_buffer, iter);
    col_fft_twd_mul_rhdl.run(1);
    col_fft_strided_mm2s_rhdl.wait();
    col_fft_twd_mul_rhdl.wait();
    auto dout_buffer_run = dout_buffer.async(
        "col_fft_twd_mul_graph.col_fft_twd_mul_out", XCL_BO_SYNC_BO_AIE_TO_GMIO,
        n_sample_per_iter * n_byte_per_sample, offset);
    dout_buffer_run.wait();
    offset += (n_sample_per_iter * n_byte_per_sample);
    iter_sum += iter;
  }
  col_fft_twd_mul_rhdl.end();

  row_fft_mm2s_buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  for (int iter = 0; iter < n_iter; iter++) {
    auto row_fft_strided_mm2s_rhdl =
        row_fft_strided_mm2s(row_fft_mm2s_buffer, iter);
    auto row_fft_strided_s2mm_rhdl =
        row_fft_strided_s2mm(transpose_dout_buffer, iter);
    row_fft_graph_hdl.run(1);
    row_fft_strided_mm2s_rhdl.wait();
    row_fft_strided_s2mm_rhdl.wait();
    row_fft_graph_hdl.wait();
    transpose_dout_buffer.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  }
  row_fft_graph_hdl.end();

  std::vector<std::complex<float>> output(
      reinterpret_cast<std::complex<float> *>(transposeDoutArray),
      reinterpret_cast<std::complex<float> *>(transposeDoutArray) +
          n_iter * n_sample_per_iter);

  return output;
}