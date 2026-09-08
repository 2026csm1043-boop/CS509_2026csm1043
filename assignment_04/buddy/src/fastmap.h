#ifndef FASTMAP_H
#define FASTMAP_H

#include <vector>
#include <utility>

struct FastMapResult
{
    std::vector<std::vector<double>> coordinates;
    std::vector<std::pair<int, int>> pivots;
};

FastMapResult fastMap(
    const std::vector<std::vector<double>>& distances,
    int targetDimensions
);

#endif
