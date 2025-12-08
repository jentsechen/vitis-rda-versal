PLATFORM=/media/jtc/Data/2024.2/Vitis/2024.2/base_platforms/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm

fft_strided_mm2s = ./fft_strided_mm2s/fft_strided_mm2s.xo
fft_strided_mm2s_bat = ./fft_strided_mm2s_bat/fft_strided_mm2s_bat.xo
# fft_strided_mm2s_bat_fan_out = ./fft_strided_mm2s_bat_fan_out/fft_strided_mm2s_bat_fan_out.xo
# fft_strided_s2mm = ./fft_strided_s2mm/fft_strided_s2mm.xo
# all: $(fft_strided_mm2s) $(fft_strided_s2mm)
all: $(fft_strided_mm2s) $(fft_strided_mm2s_bat)
# all: $(fft_strided_mm2s) $(fft_strided_mm2s_bat_fan_out)

# $(fft_strided_s2mm): fft_strided_s2mm.cpp fft_strided_s2mm.cfg
# 	v++ -c --mode hls --platform ${PLATFORM} --config ./fft_strided_s2mm.cfg

$(fft_strided_mm2s): fft_strided_mm2s.cpp fft_strided_mm2s.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./fft_strided_mm2s.cfg

$(fft_strided_mm2s_bat): fft_strided_mm2s_bat.cpp fft_strided_mm2s_bat.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./fft_strided_mm2s_bat.cfg

# $(fft_strided_mm2s_bat_fan_out): fft_strided_mm2s_bat_fan_out.cpp fft_strided_mm2s_bat_fan_out.cfg
# 	v++ -c --mode hls --platform ${PLATFORM} --config ./fft_strided_mm2s_bat_fan_out.cfg


