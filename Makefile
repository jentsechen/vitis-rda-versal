
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

TARGET   = hw
ARCH = aie

# ifeq ($(ARCH), aie)
# 	PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm
# else ifeq ($(ARCH), aie-ml)
# 	PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vek280_base_202420_1/xilinx_vek280_base_202420_1.xpfm
# else
# $(error ARCH is not supported)
# endif
PLATFORM_REPO_PATHS=/media/jtc/Data/2024.2/Vitis/2024.2/base_platforms/xilinx_vck190_base_202420_1/
PLATFORM=/media/jtc/Data/2024.2/Vitis/2024.2/base_platforms/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm
COMMON_IMAGE_VERSAL=/media/jtc/Data/2024.2/xilinx-versal-common-v2024.2
export ROOTFS=${COMMON_IMAGE_VERSAL}/rootfs.ext4
export IMAGE=${COMMON_IMAGE_VERSAL}/Image
# export CXX=${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-xilinx-linux
export CXX=${XILINX_VITIS}/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++
export SDKTARGETSYSROOT=${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-xilinx-linux

MODE	 = linux
XSA	 = ${ARCH}_base_graph_${TARGET}.xsa
HOST_EXE = host.exe

GRAPH    = aie/graph.cpp
LIBADF  = libadf.a
# FFTStridedMM2S = ./hls/fft_strided_mm2s/fft_strided_mm2s.xo
FFTStridedMM2SBat = ./hls/fft_strided_mm2s_bat/fft_strided_mm2s_bat.xo
UramController = ./hls/uram_controller/uram_controller.xo
MM2S = ./hls/mm2s/mm2s.xo
S2MM = ./hls/s2mm/s2mm.xo
# FFTStridedMM2SBatFanOut = ./hls/fft_strided_mm2s_bat_fan_out/fft_strided_mm2s_bat_fan_out.xo
FFTStridedS2MM = ./hls/fft_strided_s2mm/fft_strided_s2mm.xo
AIE_CMPL_CMD = v++ -c --mode aie --platform=${PLATFORM} \
			-I../Vitis_Libraries/dsp/L1/include/aie \
			-I../Vitis_Libraries/dsp/L1/src/aie \
			-I../Vitis_Libraries/dsp/L2/include/aie \
			--include="./aie" --work_dir=./Work \
			--aie.constraints="aie_constraints.json" \
			${GRAPH} 2>&1 | tee log.txt
AIE_SIM_CMD = aiesimulator --pkg-dir=./Work --dump-vcd foo
EMU_CMD = ./launch_hw_emu.sh
HLS_SOURCES := $(wildcard hls/*.cpp) $(wildcard hls/*.h) $(wildcard hls/*.cfg)

##########################################################################################################################################################
### DO NOT MODIFY BELOW THIS LINE UNLESS NECESSARY
################################################################################################################################################


VCC      = v++
VPP_SPEC =system.cfg
VPP_FLAGS=--save-temps --verbose --config ${VPP_SPEC}  

.PHONY: clean

###
# Guarding Checks. Do not modify.
###
check_defined = \
	$(strip $(foreach 1,$1, \
		$(call __check_defined,$1,$(strip $(value 2)))))

__check_defined = \
	$(if $(value $1),, \
		$(error Undefined $1$(if $2, ($2))))

guard-PLATFORM_REPO_PATHS:
	$(call check_defined, PLATFORM_REPO_PATHS, Set your where you downloaded xilinx_vck190_base_202420_1)

guard-ROOTFS:
	$(call check_defined, ROOTFS, Set to: xilinx-versal-common-v2024.2/rootfs.ext4)

guard-IMAGE:
	$(call check_defined, IMAGE, Set to: xilinx-versal-common-v2024.2/Image)

guard-CXX:
	$(call check_defined, CXX, Run: xilinx-versal-common-v2024.2/environment-setup-cortexa72-cortexa53-xilinx-linux)

guard-SDKTARGETSYSROOT:
	$(call check_defined, SDKTARGETSYSROOT, Run: xilinx-versal-common-v2024.2/environment-setup-cortexa72-cortexa53-xilinx-linux)

###

all: ${XSA} ${HOST_EXE} package
run: all run_hw_emu
sd_card: all

aie: guard-PLATFORM_REPO_PATHS ${LIBADF}
${LIBADF}: aie/*
	${AIE_CMPL_CMD}

aiesim: ${LIBADF}
	${AIE_SIM_CMD}

# hls: hls/*
# 	${HLS_CMPL_CMD}

.PHONY: hls
hls: $(HLS_SOURCES)
	$(MAKE) -C hls clean
	$(MAKE) -C hls all

xsa: guard-PLATFORM_REPO_PATHS ${XSA}

# ${XSA}: ${FFTStridedMM2S} ${FFTStridedS2MM} ${LIBADF} ${VPP_SPEC} 
# 	${VCC} -g -l --platform ${PLATFORM} ${FFTStridedMM2S} ${FFTStridedS2MM} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@
# ${XSA}: ${FFTStridedMM2S} ${LIBADF} ${VPP_SPEC} 
# 	${VCC} -g -l --platform ${PLATFORM} ${FFTStridedMM2S} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@
# ${XSA}: ${FFTStridedMM2S} ${FFTStridedMM2SBat} ${LIBADF} ${VPP_SPEC} 
# 	${VCC} -g -l --platform ${PLATFORM} ${FFTStridedMM2S} ${FFTStridedMM2SBat} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@
# ${XSA}: ${FFTStridedMM2SBat} ${LIBADF} ${VPP_SPEC} 
# 	${VCC} -g -l --platform ${PLATFORM} ${FFTStridedMM2SBat} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@
# ${XSA}: ${FFTStridedMM2S} ${FFTStridedMM2SBatFanOut} ${LIBADF} ${VPP_SPEC} 
# 	${VCC} -g -l --platform ${PLATFORM} ${FFTStridedMM2S} ${FFTStridedMM2SBatFanOut} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@
# ${XSA}: ${UramController} ${VPP_SPEC} 
# 	${VCC} -g -l --platform ${PLATFORM} ${UramController} -t ${TARGET} ${VPP_FLAGS} -o $@
${XSA}: ${MM2S} ${S2MM} ${LIBADF} ${VPP_SPEC} 
	${VCC} -l --platform ${PLATFORM} ${MM2S} ${S2MM} ${LIBADF} \
			--debug.chipscope mm2s_0:s \
			-t ${TARGET} ${VPP_FLAGS} -o $@

host: guard-CXX guard-SDKTARGETSYSROOT ${HOST_EXE}

# ${HOST_EXE}: ${GRAPH} ./Work/ps/c_rts/aie_control_xrt.cpp ./sw/host.cpp ./sw/fft_dds_twd.cpp
# 	$(MAKE) -C sw/
${HOST_EXE}: ${GRAPH} ./Work/ps/c_rts/aie_control_xrt.cpp ./sw/host.cpp ./sw/fft_acc_mult_twd.cpp
	$(MAKE) -C sw/

package: guard-ROOTFS guard-IMAGE guard-PLATFORM_REPO_PATHS package_${TARGET}

package_${TARGET}: ${LIBADF} ${XSA} ${HOST_EXE} 
	${VCC} -p -t ${TARGET} -f ${PLATFORM} \
		--package.rootfs ${ROOTFS} \
		--package.kernel_image ${IMAGE} \
		--package.boot_mode=sd \
		--package.image_format=ext4 \
		--package.defer_aie_run \
		--package.sd_file ${HOST_EXE} ${XSA} ${LIBADF}

clean:
	rm -rf _x v++_* ${XOS} ${OS} ${LIBADF} *.o.* *.o *.xpe *.xo.* \
	       *.xclbin* *.xsa *.log *.jou xnwOut Work Map_Report.csv \
	       ilpProblem* sol.db drivers .Xil  aiesimu* x86simu*
	$(MAKE) -C sw clean
