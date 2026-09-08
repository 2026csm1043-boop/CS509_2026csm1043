#ifndef VERTEX_COLORING_H
#define VERTEX_COLORING_H

#include "csr.h"
#include <vector>

struct ColoringResult {
    std::vector<int> colors;
    int colorsUsed;
    bool valid;
};

ColoringResult greedyVertexColoring(const CSRGraph& graph);

#endif
