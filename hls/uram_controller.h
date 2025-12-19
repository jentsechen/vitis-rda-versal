#include <ap_int.h>

// Define parameters (must be powers of 2 for memory efficiency)
#define URAM_DEPTH 1048576
#define URAM_WIDTH 64
#define BURST_SIZE 64

// Data types
typedef ap_uint<URAM_WIDTH> DTYPE;

void uram_controller(
    // AXI Master for DDR Read
    DTYPE *ddr_in,
    // AXI Master for DDR Write
    DTYPE *ddr_out,
    // Control Parameters
    unsigned long start_addr,
    unsigned long transfer_size // in terms of DTYPE elements
);