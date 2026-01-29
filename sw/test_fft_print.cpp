#include "test_fft_print.h"

std::vector<std::complex<float>>
test_fft_print(xrt::device &device, const xrt::uuid &uuid,
               const std::complex<float> *input_data,
               size_t input_n_sample) {
  (void)device;
  (void)uuid;
  std::cout << std::fixed << std::setprecision(8);
  std::cout << "test_fft_print: first row, first 10 elements:\n";
  size_t n_print = std::min<size_t>(10, input_n_sample);
  for (size_t i = 0; i < n_print; ++i) {
    const auto &v = input_data[i];
    std::cout << "  [" << 0 << "," << i << "] " << v.real()
              << (v.imag() < 0 ? "" : "+") << v.imag() << "j\n";
  }
  return {};
}
