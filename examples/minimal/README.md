## Minimal AIE + HLS + PS example

The smallest design that exercises every stage of the full flow used by the
top-level project: AIE compile → HLS synth → `v++` link → run on board (with
packaging available for a from-scratch board bring-up). No batching, tiling,
or DSP-library kernels — just enough to prove the pipeline end-to-end.

### Pipeline
```
PS (host.cpp)
  → HLS scale_mm2s   (DDR -> AXI4-Stream)
  → AIE "scale"      (y[i] = x[i] * GAIN)
  → HLS scale_s2mm   (AXI4-Stream -> DDR)
  → PS (host.cpp checks output == input * GAIN)
```
16 `int32` samples, one AIE graph iteration, one shot. `host.exe` does its own
verification and prints `PASS`/`FAIL` — no golden-data files or Python
post-processing needed.

### Layout
```
aie/
  kernel.h, kernel.cpp   # the "scale" AIE kernel
  scale_graph.h          # ScaleGraph: input_plio -> kernel -> output_plio
  graph.cpp              # top-level graph.cpp instantiating `scale_graph`
hls/
  scale_mm2s.cpp/.cfg    # DDR -> stream feeder
  scale_s2mm.cpp/.cfg    # stream -> DDR drainer
  Makefile
sw/
  host.cpp               # XRT: run PL kernels + AIE graph, verify, print PASS/FAIL
  fix_aie_control_xrt.sh # strips kernel-stub block from generated aie_control_xrt.cpp
  Makefile
system.cfg               # nk=/sc=/sp= connectivity (2 PL kernels <-> 1 AIE graph)
Makefile                  # same targets as the top-level project
verify/run_on_board.sh    # scp host.exe + a.xclbin, run over ssh, show PASS/FAIL
```

This mirrors the top-level project's conventions 1:1 (same `nk=`/`sc=`/`sp=`
syntax, same `xrt::kernel`/`xrt::graph`/`xrt::bo` host pattern, same
`v++ -p --package.*` flags) so anything learned here transfers directly.
Reused verbatim from the top-level project: the base platform, rootfs/kernel
image, and `fix_aie_control_xrt.sh`. Everything else is written from scratch,
trimmed to the minimum needed to be readable in one sitting.

### Build
```bash
cd examples/minimal
make aie        # AIE compile -> libadf.a
make hls        # HLS synth  -> hls/scale_mm2s/*.xo, hls/scale_s2mm/*.xo
make xsa        # v++ link   -> aie_scale_hw.xsa
make host       # host.exe (needs Work/ps/c_rts/aie_control_xrt.cpp from `make xsa`)
```
`make package` (below) is **not** part of the normal loop — see why.

### Run / verify on board
If your board has already been flashed with a BOOT.BIN/rootfs built from
this same base platform (e.g. from the top-level project — they share
`PLATFORM`/`ROOTFS`/`IMAGE`), you never need to touch the SD card for this
example. `xrt::device::load_xclbin()` reconfigures the PL *and* the AIE
array at runtime; `host.exe` and `a.xclbin` are the only artifacts that
differ per-design.
```bash
bash verify/run_on_board.sh   # scp host.exe + a.xclbin, run over ssh, print PASS/FAIL
```
or, matching the top-level project's targets:
```bash
make upload          # host.exe only
make upload-xclbin   # host.exe + a.xclbin (after `make xsa`)
```

### `make package` — only for a board that has never booted this platform
```bash
make package    # v++ -p -> BOOT.BIN, sd_card.img (~3GB, several minutes)
```
This produces a standalone bootable SD card image (Linux + XRT + this
xclbin). Skip it unless you're bringing up a fresh board — it's one-time
setup, not part of the build/run loop above. `BOOT.BIN`/`Image` require an
SD card rewrite + reboot to apply; `host.exe`/`a.xclbin` never do.
