#include "uram_controller.h"
// Local URAM Array (The core memory resource)
// Note: This must be defined *inside* the function for HLS to map it locally
DTYPE uram_array[URAM_DEPTH];

void uram_controller(DTYPE *ddr_in, DTYPE *ddr_out, unsigned long start_addr,
                     unsigned long transfer_size) {
// 1. --- URAM Pragma ---
// Force the local array to be implemented as URAM
// #pragma HLS RESOURCE variable = uram_array core = URAM
#pragma HLS bind_storage variable = uram_array type = RAM_T2P impl = URAM

// Interface Pragmas (usually applied in the header, but shown here for clarity)
#pragma HLS INTERFACE m_axi port = ddr_in bundle = DDR_R
#pragma HLS INTERFACE m_axi port = ddr_out bundle = DDR_W
#pragma HLS INTERFACE s_axilite port = return

  // Calculate the number of AXI bursts needed
  unsigned int num_bursts = transfer_size / BURST_SIZE;

  // 2. --- Read from DDR to URAM ---
  // The DDR access loop is pipelined to maximize throughput.
  // The AXI master interface handles burst addressing automatically.

  // Set the initial AXI pointer (optional if using internal offset)
  DTYPE *current_ddr_in = ddr_in + start_addr;

READ_LOOP:
  for (int i = 0; i < transfer_size; ++i) {
#pragma HLS PIPELINE II = 1 // Target Initiation Interval of 1 cycle

    // Read from DDR AXI interface (DDR latency is hidden by the pipeline)
    DTYPE data = current_ddr_in[i];

    // Write the data sequentially to the fast URAM block
    uram_array[i] = data;
  }

  // 3. --- Optional: Processing Stage ---
  // If you need to process the data, you would insert a third loop here,
  // reading from and writing back to uram_array[].

  // 4. --- Write from URAM to DDR ---
  // Write back the processed/buffered data to DDR via the AXI master.

  DTYPE *current_ddr_out = ddr_out + start_addr;

WRITE_LOOP:
  for (int i = 0; i < transfer_size; ++i) {
#pragma HLS PIPELINE II = 1

    // Read data sequentially from the fast URAM block
    DTYPE data = uram_array[i];

    // Write data to the DDR AXI interface
    current_ddr_out[i] = data;
  }
}