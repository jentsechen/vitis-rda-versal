## How to compile
```
make aie        // if update aie design
make hls        // if update hls design
make xsa        // if update aie or hls
make host       // if update xsa or host design
make package    // if update host
```
## How to verify
```bash
# Host side
cd verify
bash copy_sd_card.sh

# FPGA side
./host.exe a.xclbin data_rx_1024_complex_64.npy

# Host side
cd verify
bash download_data.sh
python3 verify_output.py
```

## PS Flow Control
```c
// column-wise operation
for(int i=0; i<1024; i++){
    move_data_from_ddr_to_aie();
    trigger_fft_1024_twd_mult_graph();
    move_data_from_daie_to_ddr();
}
// row-wise operation
for(int i=0; i<1024; i++){
    move_data_from_ddr_to_aie();
    trigger_fft_1024_graph();
    move_data_from_daie_to_ddr();
}
```

## Implementation Result
| ... | ... |
| :---: | :---: |
| col. time req.: 79658 us <br> row time req.: 80362 us <br> total time req.: 160070 us| Time requirement: 161250 us|
|![](./imp_result/AIE_util_acc_mult.png)|
|![](./imp_result/graph_acc_mult.png)|
|![](./imp_result/array_acc_mult.png)|

# Comparing with GPU
* Raw data: 1024x22016 pixels
* GPU
    * (range FFT + element-wise mult. + range IFFT) of 1024 rows: 0.75 sec
* AIE
    * range FFT of 1 row: 0.16 sec