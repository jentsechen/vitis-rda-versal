## Example
* Default
```bash
versal-rootfs-common-20242:~$ cat /proc/meminfo | grep Cma
CmaTotal:         524288 kB
CmaFree:          523648 kB
```
* After adding uEnv.txt
```bash
versal-rootfs-common-20242:~$ cat /proc/meminfo | grep Cma
CmaTotal:        1048576 kB
CmaFree:          763476 kB
```
### How to Add uEnv.txt
* Create uEnv.txt, for example
```bash
bootargs=console=ttyUSB1,115200 root=/dev/mmcblk0p2 rw rootwait cma=1024M
uenvcmd=run default_bootcmd
```
* Add this line to package `--package.sd_file uEnv.txt`

## Constraint of The First Segment
* Based on the device tree, 2GB segment is used, 1800MB fails, but 1500MB pass
```bash
bootargs=console=ttyUSB1,115200 root=/dev/mmcblk0p2 rw rootwait cma=1500M
uenvcmd=run default_bootcmd
```
```bash
versal-rootfs-common-20242:~$ cat /proc/meminfo | grep Cma
CmaTotal:        1536000 kB
CmaFree:         1459444 kB
```

## Generate Device Tree by XSCT
* system-top.dts
    * VCK190 is 64-bit device, so each address and size is expressed by 2 of 32-bit data
    * Based on the device tree
        * DDR4 has two segments: 2GB and 6GB (0x0000000080000000 and 0x0000000180000000)
        * LPDDR4 has one segment: 8GB (0x0000000200000000)
```bash
memorynoc_ddr4_ddr: memory@0 {
    device_type = "memory";
    reg = <0x0 0x00000000 0x0 0x80000000>, <0x00000008 0x00000000 0x00000001 0x80000000>;
};
memorynoc_lpddr4_ddr: memory@50000000000 {
    device_type = "memory";
    reg = <0x00000500 0x00000000 0x00000002 0x00000000>;
};
```

## How to Generate Device-Tree by XSCT
* Git clone and checkout
```bash
git clone https://github.com/Xilinx/device-tree-xlnx.git
cd device-tree-xlnx
git checkout xlnx_rel_v2024.2  # Match Vitis version
```
* Open XSCT on ternimal
```bash
source /media/jtc/Data/2024.2/Vitis/2024.2/settings64.sh
xsct
```
* XSCT (assume current directory is `device-tree-xlnx`)
```bash
hsi::open_hw_design ../aie_base_graph_hw.xsa
hsi::set_repo_path .
hsi::get_cells -hier *cortexa72* # terminal shows: CIPS_0_pspmc_0_psv_cortexa72_0 CIPS_0_pspmc_0_psv_cortexa72_1
hsi::create_sw_design device-tree -os device_tree -proc CIPS_0_pspmc_0_psv_cortexa72_0
hsi::generate_target -dir ./my_device_tree
hsi::close_hw_design [hsi::current_hw_design]
```