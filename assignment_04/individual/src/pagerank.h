#ifndef PAGERANK_H
#define PAGERANK_H

#include "csr.h"
#include <vector>

struct PageRankResult {
    std::vector<double> ranks;
    int iterations;
    bool converged;
};

PageRankResult pageRank(const CSRGraph& graph,
                        double damping,
                        double tolerance,
                        int maxIterations);

#endif
