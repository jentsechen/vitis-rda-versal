#ifndef __scale_graph_h__
#define __scale_graph_h__

#include "kernel.h"
#include <adf.h>

class ScaleGraph : public adf::graph {
private:
  adf::kernel scale_kernel;

public:
  adf::input_plio in;
  adf::output_plio out;

  ScaleGraph() {
    // The 3rd arg (file path) is only used by aiesimulator/x86simulator as
    // file-based stimulus/capture; it is ignored by the hardware AIE
    // compile, so it's safe to keep here for both flows.
    in = adf::input_plio::create("scale_in", adf::plio_32_bits,
                                  "data/scale_in.txt");
    out = adf::output_plio::create("scale_out", adf::plio_32_bits,
                                    "data/scale_out.txt");

    scale_kernel = adf::kernel::create(scale);
    adf::source(scale_kernel) = "kernel.cpp";
    adf::runtime<adf::ratio>(scale_kernel) = 0.5;

    adf::connect<>(in.out[0], scale_kernel.in[0]);
    adf::connect<>(scale_kernel.out[0], out.in[0]);
  }
};

#endif
