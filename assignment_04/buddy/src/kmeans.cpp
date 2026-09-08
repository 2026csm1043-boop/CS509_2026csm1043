#include "kmeans.h"

#include <cmath>
#include <limits>
#include <vector>

namespace
{
double squaredDistance(
    const std::vector<double>& a,
    const std::vector<double>& b)
{
    double sum = 0.0;

    for (std::size_t d = 0; d < a.size(); ++d)
    {
        const double diff = a[d] - b[d];
        sum += diff * diff;
    }

    return sum;
}
}

KMeansResult kMeans(
    const std::vector<std::vector<double>>& points,
    int k,
    int maxIterations,
    double tolerance)
{
    const int n = static_cast<int>(points.size());
    const int dimensions = static_cast<int>(points[0].size());

    std::vector<std::vector<double>> centroids(
        points.begin(), points.begin() + k);

    std::vector<int> assignments(n, -1);
    int iterations = 0;
    bool converged = false;

    for (int iteration = 0; iteration < maxIterations; ++iteration)
    {
        bool changed = false;

        // Assignment step
        for (int i = 0; i < n; ++i)
        {
            int bestCluster = 0;
            double bestDistance = std::numeric_limits<double>::infinity();

            for (int c = 0; c < k; ++c)
            {
                const double distance =
                    squaredDistance(points[i], centroids[c]);

                if (distance < bestDistance)
                {
                    bestDistance = distance;
                    bestCluster = c;
                }
            }

            if (assignments[i] != bestCluster)
            {
                changed = true;
                assignments[i] = bestCluster;
            }
        }

        // Update step
        std::vector<std::vector<double>> sums(
            k, std::vector<double>(dimensions, 0.0));
        std::vector<int> counts(k, 0);

        for (int i = 0; i < n; ++i)
        {
            const int cluster = assignments[i];
            ++counts[cluster];

            for (int d = 0; d < dimensions; ++d)
            {
                sums[cluster][d] += points[i][d];
            }
        }

        double maximumShift = 0.0;

        for (int c = 0; c < k; ++c)
        {
            // Required behavior: keep the previous centroid if
            // a cluster becomes empty.
            if (counts[c] == 0)
            {
                continue;
            }

            std::vector<double> newCentroid(dimensions);

            for (int d = 0; d < dimensions; ++d)
            {
                newCentroid[d] = sums[c][d] / counts[c];
            }

            const double shift =
                std::sqrt(squaredDistance(centroids[c], newCentroid));

            if (shift > maximumShift)
            {
                maximumShift = shift;
            }

            centroids[c] = newCentroid;
        }

        ++iterations;

        if (!changed || maximumShift <= tolerance)
        {
            converged = true;
            break;
        }
    }

    double wcss = 0.0;

    for (int i = 0; i < n; ++i)
    {
        wcss += squaredDistance(
            points[i],
            centroids[assignments[i]]);
    }

    return {
        assignments,
        centroids,
        iterations,
        wcss,
        converged
    };
}
