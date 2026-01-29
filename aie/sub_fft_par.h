#define TT_DATA_ cfloat        // 數據類型: 複數浮點數 (Complex Float)
#define TT_TWIDDLE_ cfloat     // 旋轉因子類型: 複數浮點數
#define TP_POINT_SIZE 1024     // FFT 點數: 1024 點
#define TP_FFT_NIFFT 1         // 模式: 1 代表 FFT (正向), 0 代表 IFFT (反向)
#define TP_SHIFT 0             // 位移量 (Scaling shift): 0 (不位移)
#define TP_CASC_LEN 1          // 串接長度 (Cascade length): 1 (單一 Kernel)
#define TP_DYN_PT_SIZE 0       // 動態點數: 0 (靜態，編譯時決定)
// #define TP_WINDOW_VSIZE 1024 * 2 // 視窗向量大小 (註解掉的備份)
#define TP_API 0               // API 類型: 0 (Window 介面), 1 (Stream 介面)
#define TP_PARALLEL_POWER 0    // 平行度冪次: 0 (無 SSR 平行化)
#define TP_USE_WIDGETS 0       // 是否使用 Widget: 0 (否)
#define N_PARAL 1              // 平行路徑數量 (自定義巨集)
#define N_BATCH_FFT 2          // 批次數量: 每次 Kernel 執行處理 2 個 FFT 幀