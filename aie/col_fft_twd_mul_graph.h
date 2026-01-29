// #ifndef __ReplicaColProcGraphTest__
// #define __ReplicaColProcGraphTest__

// #include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
// #include "kernel.h"
// #include "sub_fft_par.h"
// // #include "twd_factor_table.h"
// #include <adf.h>
// #include <aie_api/aie.hpp>

// class ColFftTwdMulGraph : public adf::graph {
// private:
//   xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<
//       cfloat,                      // TT_DATA
//       cfloat,                      // TT_TWIDDLE
//       TP_POINT_SIZE,               // TP_POINT_SIZE
//       1,                           // TP_FFT_NIFFT
//       0,                           // TP_SHIFT
//       1,                           // TP_CASC_LEN
//       0,                           // TP_DYN_PT_SIZE
//       TP_POINT_SIZE * N_BATCH_FFT, // TP_WINDOW_VSIZE
//       0,                           // TP_API
//       0,                           // TP_PARALLEL_POWER
//       0                            // TP_USE_WIDGETS
//       >
//       fft_kernel[N_PARAL];
//   adf::kernel twd_mult_kernel, twd_mult_kernel_1;
//   //   adf::kernel twd_mult_kernel;

// public:
//   //   adf::input_gmio col_fft_twd_mul_in;
// //   adf::input_plio col_fft_twd_mul_in_0, col_fft_twd_mul_in_1;
//     adf::input_plio col_fft_twd_mul_in_0;
//   //   adf::output_gmio col_fft_twd_mul_out_0, col_fft_twd_mul_out_1;
// //   adf::output_plio col_fft_twd_mul_out_0, col_fft_twd_mul_out_1;
//     adf::output_plio col_fft_twd_mul_out_0;
//   //   adf::output_gmio col_fft_twd_mul_out_0;

//   ColFftTwdMulGraph() {
//     // col_fft_twd_mul_in =
//     //     adf::input_gmio::create("col_fft_twd_mul_in", 64, 1000);
//     col_fft_twd_mul_in_0 =
//         input_plio::create("col_fft_twd_mul_in_0", adf::plio_64_bits);
//         // input_plio::create("col_fft_twd_mul_in_0", adf::plio_64_bits, "col_fft_twd_mul_in_0.txt");
//     // col_fft_twd_mul_in_1 =
//     //     input_plio::create("col_fft_twd_mul_in_1", adf::plio_64_bits);
//     // col_fft_twd_mul_out_0 =
//     //     adf::output_gmio::create("col_fft_twd_mul_out_0", 64, 1000);
//     // col_fft_twd_mul_out_1 =
//     //     adf::output_gmio::create("col_fft_twd_mul_out_1", 64, 1000);
//     col_fft_twd_mul_out_0 =
//         output_plio::create("col_fft_twd_mul_out_0", adf::plio_64_bits);
//         // output_plio::create("col_fft_twd_mul_out_0", adf::plio_64_bits, "col_fft_twd_mul_out_0.txt");
//     // col_fft_twd_mul_out_1 =
//     //     output_plio::create("col_fft_twd_mul_out_1", adf::plio_64_bits);

//     twd_mult_kernel = adf::kernel::create(twd_mult);
//     source(twd_mult_kernel) = "./twd_mult_0.cpp";
//     runtime<ratio>(twd_mult_kernel) = 0.8;
//     // twd_mult_kernel_1 = adf::kernel::create(twd_mult_1);
//     // source(twd_mult_kernel_1) = "./twd_mult_1.cpp";
//     // runtime<ratio>(twd_mult_kernel_1) = 0.8;
//     // twd_mult_kernel = adf::kernel::create(twd_mult_2_paral);
//     // source(twd_mult_kernel) = "./twd_mult.cpp";
//     // runtime<ratio>(twd_mult_kernel) = 0.8;

//     // adf::connect<>(col_fft_twd_mul_in_0.out[0], fft_kernel.in[0]);
//     // adf::connect<>(fft_kernel.out[0], twd_mult_kernel.in[0]);
//     // adf::connect<>(twd_mult_kernel.out[0], col_fft_twd_mul_out_0.in[0]);

//     adf::connect<>(col_fft_twd_mul_in_0.out[0], fft_kernel[0].in[0]);
//     adf::connect<>(fft_kernel[0].out[0], twd_mult_kernel.in[0]);
//     adf::connect<>(twd_mult_kernel.out[0], col_fft_twd_mul_out_0.in[0]);
//     // adf::connect<>(col_fft_twd_mul_in_1.out[0], fft_kernel[1].in[0]);
//     // adf::connect<>(fft_kernel[1].out[0], twd_mult_kernel_1.in[0]);
//     // adf::connect<>(twd_mult_kernel_1.out[0], col_fft_twd_mul_out_1.in[0]);

//     // adf::connect<>(col_fft_twd_mul_in_0.out[0], fft_kernel[0].in[0]);
//     // adf::connect<>(fft_kernel[0].out[0], twd_mult_kernel.in[0]);
//     // adf::connect<>(twd_mult_kernel.out[0], col_fft_twd_mul_out_0.in[0]);
//     // adf::connect<>(col_fft_twd_mul_in_1.out[0], fft_kernel[1].in[0]);
//     // adf::connect<>(fft_kernel[1].out[0], twd_mult_kernel.in[1]);
//     // adf::connect<>(twd_mult_kernel.out[1], col_fft_twd_mul_out_1.in[0]);
//   }
// };

// #endif


#ifndef __ReplicaColProcGraphTest__
#define __ReplicaColProcGraphTest__

#include "../../Vitis_Libraries/dsp/L2/include/aie/fft_ifft_dit_1ch_graph.hpp"
#include "kernel.h"
#include "sub_fft_par.h"
#include <adf.h>
#include <aie_api/aie.hpp>

class ColFftTwdMulGraph : public adf::graph {
private:
  // 定義 FFT Kernel，使用 Vitis DSP Library 的 fft_ifft_dit_1ch_graph
  xf::dsp::aie::fft::dit_1ch::fft_ifft_dit_1ch_graph<
      cfloat,                      // TT_DATA
      cfloat,                      // TT_TWIDDLE
      TP_POINT_SIZE,               // TP_POINT_SIZE
      1,                           // TP_FFT_NIFFT
      0,                           // TP_SHIFT
      1,                           // TP_CASC_LEN
      0,                           // TP_DYN_PT_SIZE
      TP_POINT_SIZE, // TP_WINDOW_VSIZE
      0,                           // TP_API
      0,                           // TP_PARALLEL_POWER
      0                            // TP_USE_WIDGETS
      >
      fft_kernel[N_PARAL];
  
  // 定義旋轉因子乘法 Kernel (Twiddle Multiplication Kernel)
  adf::kernel twd_mult_kernel, twd_mult_kernel_1;

public:
    // 定義 PLIO 輸入端口 (資料從 PL 進來)
    adf::input_plio col_fft_twd_mul_in_0;
    // 定義 PLIO 輸出端口 (資料輸出到 PL)
    adf::output_plio col_fft_twd_mul_out_0;

  ColFftTwdMulGraph() {

    // 創建 PLIO 接口，指定名稱和位寬 (64-bit)
    col_fft_twd_mul_in_0 =
        input_plio::create("col_fft_twd_mul_in_0", adf::plio_64_bits);

    col_fft_twd_mul_out_0 =
        output_plio::create("col_fft_twd_mul_out_0", adf::plio_64_bits);

    // 創建旋轉因子乘法 Kernel，並指定來源檔案
    twd_mult_kernel = adf::kernel::create(twd_mult);
    source(twd_mult_kernel) = "./twd_mult_0.cpp";
    runtime<ratio>(twd_mult_kernel) = 0.8;

    // 設定資料流向: PL 輸入 -> FFT Kernel -> 旋轉因子乘法 Kernel -> PL 輸出
    adf::connect<>(col_fft_twd_mul_in_0.out[0], fft_kernel[0].in[0]);
    adf::connect<>(fft_kernel[0].out[0], twd_mult_kernel.in[0]);
    adf::connect<>(twd_mult_kernel.out[0], col_fft_twd_mul_out_0.in[0]);

  }
};

#endif
