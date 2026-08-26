#include "scale_graph.h"
#include <adf.h>

// Variable name must match the string passed to xrt::graph(device, uuid, "scale_graph")
// in sw/host.cpp -- that's how the PS side locates this graph in the xclbin.
ScaleGraph scale_graph;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv) {
  scale_graph.init();
  scale_graph.run(1);
  scale_graph.end();
  return 0;
};
#endif
