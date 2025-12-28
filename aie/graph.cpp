#include "col_fft_twd_mul_graph.h"
#include "row_fft_graph.h"
#include <adf.h>
#include <fstream>
#include <unistd.h>
using namespace adf;

ColFftTwdMulGraph col_fft_twd_mul_graph;
// RowFftGraph row_fft_graph;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv) {
  col_fft_twd_mul_graph.init();
  col_fft_twd_mul_graph.run(1);
  col_fft_twd_mul_graph.end();
//   row_fft_graph.init();
//   row_fft_graph.run(1);
//   row_fft_graph.end();
  return 0;
};
#endif
