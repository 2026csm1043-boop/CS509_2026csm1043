#ifndef KMEANS_H
#define KMEANS_H

#include <vector>

struct KMeansResult
{
    std::vector<int> assignments;
    std::vector<std::vector<double>> centroids;
    int iterations;
    double wcss;
    bool converged;
};

KMeansResult kMeans(
    const std::vector<std::vector<double>>& points,
    int k,
    int maxIterations,
    double tolerance
);

#endif
