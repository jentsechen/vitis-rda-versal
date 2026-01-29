#include "col_fft_twd_mul_graph.h"
#include "row_fft_graph.h"
#include <adf.h>
#include <fstream>
#include <unistd.h>
using namespace adf;

// 實例化 Column 方向處理的 Graph (負責 Column FFT + 旋轉因子乘法)
ColFftTwdMulGraph  ReplicaColProcGraphTest; // col_fft_twd_mul_graph; // ReplicaColProcGraphTest

// 實例化 Row 方向處理的 Graph (負責 Row FFT)
RowFftGraph ReplicaRowProcGraph; // row_fft_graph; // ReplicaRowProcGraph

//test
#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv) { 
  // --- Column 處理階段 ---
  // 初始化 Column Graph，載入 ELF 並設定緩衝區
  ReplicaColProcGraphTest.init();
  // 執行 Column Graph 1 次迭代 (Iteration)
  ReplicaColProcGraphTest.run(1);
  ReplicaColProcGraphTest.end();

  // --- Row 處理階段 ---
  // 初始化 Row Graph
  ReplicaRowProcGraph.init();
  // 執行 Row Graph 1 次迭代
  ReplicaRowProcGraph.run(1);
  ReplicaRowProcGraph.end();
  return 0;
};
#endif
