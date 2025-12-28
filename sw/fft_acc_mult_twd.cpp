#include "fft_acc_mult_twd.h"

std::vector<std::complex<float>>
fft_acc_mult_twd(xrt::device &device, const xrt::uuid &uuid,
                 const std::complex<float> *input_data, size_t input_n_sample) {
  int n_batch = 2;
  // AIE kernels
  auto col_fft_twd_mul_rhdl = xrt::graph(device, uuid, "col_fft_twd_mul_graph");
  //   auto row_fft_graph_hdl = xrt::graph(device, uuid, "row_fft_graph");

  // PL kernels
  //   auto row_fft_mm2s_0 = xrt::kernel(
  //       device, uuid, "fft_strided_mm2s_bat:{fft_strided_mm2s_bat_0}");
  //   auto row_fft_mm2s_1 = xrt::kernel(
  //       device, uuid, "fft_strided_mm2s_bat:{fft_strided_mm2s_bat_1}");
  //   auto col_fft_strided_mm2s_0 = xrt::kernel(
  //       device, uuid, "fft_strided_mm2s_bat:{fft_strided_mm2s_bat_2}");
  //   auto col_fft_strided_mm2s_1 = xrt::kernel(
  //       device, uuid, "fft_strided_mm2s_bat:{fft_strided_mm2s_bat_3}");
  auto mm2s_0 = xrt::kernel(device, uuid, "mm2s:{mm2s_0}");
  auto mm2s_1 = xrt::kernel(device, uuid, "mm2s:{mm2s_1}");
  auto s2mm_0 = xrt::kernel(device, uuid, "s2mm:{s2mm_0}");
  auto s2mm_1 = xrt::kernel(device, uuid, "s2mm:{s2mm_1}");

  // buffers
  auto in_buf = xrt::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  //   auto twd_mul_out_buf =
  //       xrt::aie::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  //   xrt::bo row_fft_in_buf = xrt::bo{twd_mul_out_buf};
  auto row_fft_in_buf =
      xrt::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  //   auto out_buf = xrt::bo(device, block_size_in_byte,
  //   xrt::bo::flags::normal, 0);
  //   auto out_buf =
  //       xrt::aie::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0);
  std::vector<xrt::aie::bo> out_buf(n_paral);
  for (int i_paral = 0; i_paral < n_paral; i_paral++) {
    out_buf[i_paral] =
        xrt::aie::bo(device, n_sample_per_paral * n_byte_per_sample,
                     xrt::bo::flags::normal, 0);
  }

  // init. input buffer
  auto *in_arr = in_buf.map<__uint64_t *>();
  memset(in_arr, 0, block_size_in_byte);
  memcpy(in_arr, input_data,
         std::min(input_n_sample * n_byte_per_sample, block_size_in_byte));
  //   std::vector<std::complex<float> *> out_arr(n_paral);
  //   for (int i_paral = 0; i_paral < n_paral; i_paral++) {
  //     out_arr[i_paral] = out_buf[i_paral].map<std::complex<float> *>();
  //   }
  auto *row_fft_in_arr = row_fft_in_buf.map<__uint64_t *>();

  auto start = std::chrono::high_resolution_clock::now();
  in_buf.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  mm2s_0(in_buf, n_sample_per_paral, 0);
  mm2s_1(in_buf, n_sample_per_paral, n_sample_per_paral);
  col_fft_twd_mul_rhdl.run(n_iter / (n_paral * n_batch));
  s2mm_0(row_fft_in_buf, n_sample_per_paral, 0);
  s2mm_1(row_fft_in_buf, n_sample_per_paral, n_sample_per_paral);
  row_fft_in_buf.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  col_fft_twd_mul_rhdl.end();
  auto end0 = std::chrono::high_resolution_clock::now();
  std::cout << "time req. of col. FFT: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end0 -
                                                                     start)
                   .count()
            << " us" << std::endl;

  //   auto start1 = std::chrono::high_resolution_clock::now();
  //   row_fft_in_buf.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  //   offset = 0;
  //   for (int iter = 0; iter < n_iter / (n_batch * n_paral); iter++) {
  //     auto row_fft_mm2s_0_rhdl = row_fft_mm2s_0(row_fft_in_buf, iter *
  //     n_batch); auto row_fft_mm2s_1_rhdl =
  //         row_fft_mm2s_1(row_fft_in_buf, iter * n_batch + 512);
  //     row_fft_graph_hdl.run(1);
  //     row_fft_mm2s_0_rhdl.wait();
  //     row_fft_mm2s_1_rhdl.wait();
  //     row_fft_graph_hdl.wait();
  //     auto out_buf_run_0 = out_buf[0].async(
  //         "row_fft_graph.row_fft_out_0", XCL_BO_SYNC_BO_AIE_TO_GMIO,
  //         n_sample_per_iter * n_byte_per_sample * n_batch,
  //         iter * n_sample_per_iter * n_byte_per_sample * n_batch);
  //     auto out_buf_run_1 = out_buf[1].async(
  //         "row_fft_graph.row_fft_out_1", XCL_BO_SYNC_BO_AIE_TO_GMIO,
  //         n_sample_per_iter * n_byte_per_sample * n_batch,
  //         iter * n_sample_per_iter * n_byte_per_sample * n_batch);
  //     out_buf_run_0.wait();
  //     out_buf_run_1.wait();
  //   }
  //   //   out_buf.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  //   row_fft_graph_hdl.end();
  //   auto end = std::chrono::high_resolution_clock::now();
  //   std::cout << "time requirement: "
  //             << std::chrono::duration_cast<std::chrono::microseconds>(end -
  //                                                                      start)
  //                    .count()
  //             << " us" << std::endl;
  //   std::cout << "time req. of row. FFT: "
  //             << std::chrono::duration_cast<std::chrono::microseconds>(end -
  //                                                                      start1)
  //                    .count()
  //             << " us" << std::endl;

  std::vector<std::complex<float>> output;
  //   output.reserve(n_iter * n_sample_per_iter * n_byte_per_sample);
  //   for (int i_paral = 0; i_paral < n_paral; i_paral++) {
  //     memcpy(output.data() + i_paral * n_sample_per_paral,
  //            reinterpret_cast<std::complex<float> *>(out_arr[i_paral]),
  //            n_sample_per_paral * n_byte_per_sample);
  //   }
  output.reserve(n_iter * n_sample_per_iter * n_byte_per_sample);
  memcpy(output.data(), reinterpret_cast<std::complex<float> *>(row_fft_in_arr),
         n_iter * n_sample_per_iter * n_byte_per_sample);

  return output;
}