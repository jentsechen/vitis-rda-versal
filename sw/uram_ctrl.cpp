#include "uram_ctrl.h"

std::vector<std::complex<float>>
uram_ctrl(xrt::device &device, const xrt::uuid &uuid,
          const std::complex<float> *input_data, size_t input_n_sample) {
  size_t uram_depth = 1048576;
  auto uram_ctrl = xrt::kernel(device, uuid, "uram_controller:{uram_ctrl_0}");
  size_t data_size = uram_depth * sizeof(std::complex<float>);
  auto bo_in = xrt::bo(device, data_size, uram_ctrl.group_id(0));
  // xrt::bo(device, block_size_in_byte, xrt::bo::flags::normal, 0)
  auto bo_out = xrt::bo(device, data_size, uram_ctrl.group_id(1));
  std::complex<float> *bo_in_map = bo_in.map<std::complex<float> *>();
  memset(bo_in_map, 0, block_size_in_byte);
  memcpy(bo_in_map, input_data,
         std::min(input_n_sample * n_byte_per_sample, block_size_in_byte));
  bo_in.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  auto run = uram_ctrl(bo_in, bo_out, 0, uram_depth);
  run.wait();
  bo_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  std::complex<float> *bo_out_map = bo_out.map<std::complex<float> *>();

  std::vector<std::complex<float>> output(bo_out_map, bo_out_map + uram_depth);
  return output;
}