#include "./cnpy/cnpy.cpp"
#include "./cnpy/cnpy.h"
// #include "fft_dds_twd.h"
// #include "fft_acc_mult_twd.h"
#include "uram_ctrl.h"
#include "xrt/xrt_aie.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_kernel.h"
#include <adf.h>
#include <array>
#include <chrono>
#include <complex>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace adf;
using namespace std;

int main(int argc, char **argv) {
  cnpy::NpyArray arr = cnpy::npy_load(argv[2]);
  assert(arr.shape[0] > 0 && arr.shape[1] > 0);
  //   std::cout << "number of values: " << arr.shape[1] << std::endl;
  const complex<float> *DataInput = arr.data<complex<float>>();

  char *xclbinFilename = argv[1];
  auto device = xrt::device(0);
  auto uuid = device.load_xclbin(xclbinFilename);

  //   auto output = fft_dds_twd(device, uuid, DataInput, arr.shape[1]);
  //   auto output = fft_acc_mult_twd(device, uuid, DataInput, arr.shape[1]);
  auto output = uram_ctrl(device, uuid, DataInput, arr.shape[1]);

  cnpy::npy_save("output.npy", output.data(), {n_iter * n_sample_per_iter}, "w");

  std::cout << "Done!" << std::endl;

  return 0;
};
