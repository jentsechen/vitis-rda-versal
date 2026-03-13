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

* Group ID (on FPGA)
```bash
versal-rootfs-common-20242:~$ xbutil examine --device 0000:00:00.0 --report memory
----------------------------------------------------------------------
                              WARNING:
                xbutil has been renamed to xrt-smi
        Please migrate to using xrt-smi instead of xbutil.

    Commands, options, arguments and their descriptions can also be 
                    reported via the --help option.
----------------------------------------------------------------------

----------------------
[0000:00:00.0] : edge
----------------------

  Memory Topology
    HW Context Slot: 0
      Xclbin UUID: 0f54111d-aaca-5434-bc8c-139beb318907
      |Index  ||Tag    ||Type      ||Temp(C)  ||Size     ||Base Address   |
      |-------||-------||----------||---------||---------||---------------|
      |0      ||DDR    ||MEM_DDR4  ||N/A      ||2 GB     ||0x0            |
      |1      ||DDR    ||MEM_DRAM  ||N/A      ||6144 MB  ||0x800000000    |
      |2      ||LPDDR  ||MEM_DRAM  ||N/A      ||8 GB     ||0x50000000000  |

```

`xclbinutil --info --input a.xclbin`
```
Memory Configuration
--------------------
   Name:         DDR
   Index:        0
   Type:         MEM_DDR4
   Base Address: 0x0
   Address Size: 0x80000000
   Bank Used:    Yes

   Name:         DDR
   Index:        1
   Type:         MEM_DRAM
   Base Address: 0x800000000
   Address Size: 0x180000000
   Bank Used:    Yes

   Name:         LPDDR
   Index:        2
   Type:         MEM_DRAM
   Base Address: 0x50000000000
   Address Size: 0x200000000
   Bank Used:    No
```

num_chunks = 170
write input buffer: 2748342 us
column-wise FFT: 433276 us

* Temporary conclusion: If `sp` (signal port) in `system.cfg` is defined for both DDR and LPDDR, buffer object of size smaller than CMA can be used for each segment of DDR and LPDDR
    * In this test, 3 buffer objects with 1.36GB can be used