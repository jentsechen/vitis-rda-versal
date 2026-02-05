#include "range_fft.h"

size_t get_header_len(std::string file_name) {
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
    return header_len;
}

std::vector<std::complex<float>>
range_fft(xrt::device &device, const xrt::uuid &uuid, std::string file_name) {

    auto col_fft_twd_mul_rhdl = xrt::graph(device, uuid, "col_fft_twd_mul_graph");
    auto mm2s_0 = xrt::kernel(device, uuid, "mm2s:{mm2s_0}");
    auto mm2s_1 = xrt::kernel(device, uuid, "mm2s:{mm2s_1}");
    auto s2mm_0 = xrt::kernel(device, uuid, "s2mm:{s2mm_0}");
    auto s2mm_1 = xrt::kernel(device, uuid, "s2mm:{s2mm_1}");

    auto start = std::chrono::high_resolution_clock::now();
    size_t header_len = get_header_len(file_name);

    auto in_buf = xrt::bo(device, stride_bytes * num_chunks, xrt::bo::flags::normal, 0);
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
    auto row_fft_in_buf = xrt::bo(device, block_size_in_byte * n_test_row, xrt::bo::flags::normal, 1);
    auto *row_fft_in_arr = row_fft_in_buf.map<__uint64_t *>();

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

    auto start1 = std::chrono::high_resolution_clock::now();
    auto run_mm2s_0 = mm2s_0(in_buf, n_sample_per_paral, 0, n_test_row);
    auto run_mm2s_1 = mm2s_1(in_buf, n_sample_per_paral, n_sample_per_paral, n_test_row);
    col_fft_twd_mul_rhdl.run(n_iter / n_paral * n_test_row);
    auto run_s2mm_0 = s2mm_0(row_fft_in_buf, n_sample_per_paral, 0, n_test_row);
    auto run_s2mm_1 =
        s2mm_1(row_fft_in_buf, n_sample_per_paral, n_sample_per_paral, n_test_row);
    run_mm2s_0.wait();
    run_mm2s_1.wait();
    run_s2mm_0.wait();
    run_s2mm_1.wait();
    col_fft_twd_mul_rhdl.wait();
    col_fft_twd_mul_rhdl.end();
    auto end1 = std::chrono::high_resolution_clock::now();
    std::cout << "time req. of col. FFT: "
                << std::chrono::duration_cast<std::chrono::microseconds>(end1 -
                                                                        start1)
                    .count()
                << " us" << std::endl;

    // std::vector<std::complex<float>> output(fft_size * n_row);
    //   memcpy(col_proc_out.data(),
    //          reinterpret_cast<std::complex<float> *>(row_fft_in_arr),
    //          n_iter * n_sample_per_iter * n_byte_per_sample);

    std::vector<std::complex<float>> output(n_iter * n_sample_per_iter * n_test_row);
    memcpy(output.data(),
         reinterpret_cast<std::complex<float> *>(row_fft_in_arr),
         n_iter * n_sample_per_iter * n_byte_per_sample * n_test_row);

  return output;
}