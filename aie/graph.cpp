#include "col_fft_twd_mul_graph.h"
#include "row_fft_graph.h"
#include <adf.h>
#include <fstream>
#include <unistd.h>
using namespace adf;

ColFftTwdMulGraph  ReplicaColProcGraphTest; // col_fft_twd_mul_graph; // ReplicaColProcGraphTest
RowFftGraph ReplicaRowProcGraph; // row_fft_graph; // ReplicaRowProcGraph

// test
// test
#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv) {
  ReplicaColProcGraphTest.init();
  ReplicaColProcGraphTest.run(1);
  ReplicaColProcGraphTest.end();
  ReplicaRowProcGraph.init();
  ReplicaRowProcGraph.run(1);
  ReplicaRowProcGraph.end();
  return 0;
};
#endif
