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
## Implementation Result
Time requirement of computation: 161250 us
![](./imp_result/AIE_util_acc_mult.png)
![](./imp_result/graph_acc_mult.png)
![](./imp_result/array_acc_mult.png)