#include "range_fft.h"

std::vector<std::complex<float>>
range_fft(xrt::device &device, const xrt::uuid &uuid, std::string file_name) {
  auto start = std::chrono::high_resolution_clock::now();

  FILE *fp = fopen(file_name.c_str(), "rb");

  std::vector<size_t> shape;
  size_t word_size;
  bool fortran_order;

  cnpy::parse_npy_header(fp, word_size, shape, fortran_order);

  std::cout << "Word size: " << word_size << " bytes" << std::endl;
  std::cout << "Dimensions: " << shape.size() << std::endl;
  std::cout << "Shape: ";
  for (size_t s : shape)
    std::cout << s << " ";
  std::cout << std::endl;

  size_t header_len = ftell(fp);
  fclose(fp);

  auto in_buf =
      xrt::bo(device, stride_bytes * num_chunks, xrt::bo::flags::normal, 0);
  auto *in_arr = in_buf.map<std::complex<float> *>();
  std::ifstream ifs(file_name, std::ios::binary);
  ifs.seekg(header_len);
  for (int i = 0; i < num_chunks; ++i) {
    std::complex<float> *current_data_ptr = in_arr + (i * samples_stride);
    ifs.read(reinterpret_cast<char *>(current_data_ptr), data_bytes);
    std::fill_n(current_data_ptr + samples_per_data, samples_per_pad,
                std::complex<float>(0.0f, 0.0f));
  }
  in_buf.sync(XCL_BO_SYNC_BO_TO_DEVICE);

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "time requirement: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                     start)
                   .count()
            << " us" << std::endl;

  std::cout << "DDR content after write:" << std::endl;
  std::cout << "the first chunk:" << std::endl;
  for (auto i = 0; i < 10; i++) {
    std::cout << in_arr[i] << std::endl;
  }
  std::cout << "the second chunk:" << std::endl;
  for (auto i = 0; i < 10; i++) {
    std::cout << in_arr[samples_stride + i] << std::endl;
  }

  std::vector<std::complex<float>> output(fft_size * n_row);
  //   memcpy(col_proc_out.data(),
  //          reinterpret_cast<std::complex<float> *>(row_fft_in_arr),
  //          n_iter * n_sample_per_iter * n_byte_per_sample);

  return output;
}