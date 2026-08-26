// Minimal PS-side driver: run the scale_mm2s -> AIE scale_graph -> scale_s2mm
// pipeline once on N_SAMPLE int32 values and verify out[i] == in[i] * GAIN.
// No golden-data files, no Python post-processing -- prints PASS/FAIL.
#include "xrt/xrt_aie.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_kernel.h"
#include <cstdint>
#include <cstring>
#include <iostream>

// Must match N_SAMPLE/GAIN in aie/kernel.cpp.
static const int N_SAMPLE = 16;
static const int GAIN = 3;

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " <xclbin>" << std::endl;
    return 1;
  }
  char *xclbinFilename = argv[1];
  auto device = xrt::device(0);
  auto uuid = device.load_xclbin(xclbinFilename);

  // AIE graph
  auto scale_graph = xrt::graph(device, uuid, "scale_graph");

  // PL kernels
  auto scale_mm2s_0 = xrt::kernel(device, uuid, "scale_mm2s:{scale_mm2s_0}");
  auto scale_s2mm_0 = xrt::kernel(device, uuid, "scale_s2mm:{scale_s2mm_0}");

  // buffers
  const size_t n_byte = N_SAMPLE * sizeof(int32_t);
  auto in_buf = xrt::bo(device, n_byte, xrt::bo::flags::normal, 0);
  auto out_buf = xrt::bo(device, n_byte, xrt::bo::flags::normal, 0);

  auto *in_arr = in_buf.map<int32_t *>();
  for (int i = 0; i < N_SAMPLE; i++) {
    in_arr[i] = i;
  }
  auto *out_arr = out_buf.map<int32_t *>();
  memset(out_arr, 0, n_byte);

  in_buf.sync(XCL_BO_SYNC_BO_TO_DEVICE);

  auto run_mm2s = scale_mm2s_0(in_buf, N_SAMPLE);
  scale_graph.run(1);
  auto run_s2mm = scale_s2mm_0(out_buf, N_SAMPLE);

  run_mm2s.wait();
  run_s2mm.wait();
  scale_graph.wait();
  scale_graph.end();

  out_buf.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

  bool pass = true;
  for (int i = 0; i < N_SAMPLE; i++) {
    int32_t expected = in_arr[i] * GAIN;
    if (out_arr[i] != expected) {
      std::cout << "mismatch at " << i << ": expected " << expected
                << " got " << out_arr[i] << std::endl;
      pass = false;
    }
  }
  std::cout << (pass ? "PASS" : "FAIL") << std::endl;

  return pass ? 0 : 1;
}
