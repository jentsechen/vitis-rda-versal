PLATFORM=/media/jtc/Data/2024.2/Vitis/2024.2/base_platforms/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm

fft_strided_mm2s = ./fft_strided_mm2s/fft_strided_mm2s.xo
mm2s = ./mm2s/mm2s.xo
s2mm = ./s2mm/s2mm.xo
tile_s2mm = ./tile_s2mm/tile_s2mm.xo
tile_transpose = ./tile_transpose/tile_transpose.xo
fft_strided_mm2s_bat = ./fft_strided_mm2s_bat/fft_strided_mm2s_bat.xo
uram_controller = ./uram_controller/uram_controller.xo
# fft_strided_mm2s_bat_fan_out = ./fft_strided_mm2s_bat_fan_out/fft_strided_mm2s_bat_fan_out.xo
# fft_strided_s2mm = ./fft_strided_s2mm/fft_strided_s2mm.xo
# all: $(fft_strided_mm2s) $(fft_strided_s2mm)
# all: $(fft_strided_mm2s) $(fft_strided_mm2s_bat) $(uram_controller)
all: $(mm2s) $(s2mm) $(tile_s2mm) $(tile_transpose)

# all: $(fft_strided_mm2s) $(fft_strided_mm2s_bat_fan_out)

# $(fft_strided_s2mm): fft_strided_s2mm.cpp fft_strided_s2mm.cfg
# 	v++ -c --mode hls --platform ${PLATFORM} --config ./fft_strided_s2mm.cfg

$(fft_strided_mm2s): fft_strided_mm2s.cpp fft_strided_mm2s.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./fft_strided_mm2s.cfg

$(fft_strided_mm2s_bat): fft_strided_mm2s_bat.cpp fft_strided_mm2s_bat.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./fft_strided_mm2s_bat.cfg

# $(fft_strided_mm2s_bat_fan_out): fft_strided_mm2s_bat_fan_out.cpp fft_strided_mm2s_bat_fan_out.cfg
# 	v++ -c --mode hls --platform ${PLATFORM} --config ./fft_strided_mm2s_bat_fan_out.cfg

$(uram_controller): uram_controller.cpp uram_controller.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./uram_controller.cfg

$(mm2s): mm2s.cpp mm2s.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./mm2s.cfg

$(s2mm): s2mm.cpp s2mm.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./s2mm.cfg

$(tile_s2mm): tile_s2mm.cpp tile_s2mm.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./tile_s2mm.cfg

$(tile_transpose): tile_transpose.cpp tile_transpose.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./tile_transpose.cfg

# clean:
# 	rm -r ./uram_controller

clean:
	rm -r ./mm2s ./s2mm ./tile_transpose
