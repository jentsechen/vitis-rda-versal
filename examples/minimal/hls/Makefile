PLATFORM=/media/jtc/Data/2024.2/Vitis/2024.2/base_platforms/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm

scale_mm2s = ./scale_mm2s/scale_mm2s.xo
scale_s2mm = ./scale_s2mm/scale_s2mm.xo

all: $(scale_mm2s) $(scale_s2mm)

$(scale_mm2s): scale_mm2s.cpp scale_mm2s.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./scale_mm2s.cfg

$(scale_s2mm): scale_s2mm.cpp scale_s2mm.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config ./scale_s2mm.cfg

clean:
	rm -rf ./scale_mm2s ./scale_s2mm
