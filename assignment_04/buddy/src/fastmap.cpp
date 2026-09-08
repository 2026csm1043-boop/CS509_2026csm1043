#include "fastmap.h"

#include <cmath>
#include <limits>
#include <utility>
#include <vector>

namespace
{
std::pair<int, int> choosePivots(
    const std::vector<std::vector<double>>& distances,
    const std::vector<std::vector<double>>& residual,
    int startObject)
{
    const int n = static_cast<int>(distances.size());

    int first = startObject;
    int second = first;
    double best = -1.0;

    // Find an object farthest from the starting object.
    for (int v = 0; v < n; ++v)
    {
        if (residual[first][v] > best)
        {
            best = residual[first][v];
            second = v;
        }
    }

    // Then find an object farthest from that object.
    int pivotA = second;
    int pivotB = second;
    best = -1.0;

    for (int v = 0; v < n; ++v)
    {
        if (residual[pivotA][v] > best)
        {
            best = residual[pivotA][v];
            pivotB = v;
        }
    }

    return {pivotA, pivotB};
}
}

FastMapResult fastMap(
    const std::vector<std::vector<double>>& distances,
    int targetDimensions)
{
    const int n = static_cast<int>(distances.size());

    std::vector<std::vector<double>> coordinates(
        n, std::vector<double>(targetDimensions, 0.0));

    std::vector<std::vector<double>> residual = distances;

    std::vector<std::pair<int, int>> pivots;
    pivots.reserve(targetDimensions);

    for (int dim = 0; dim < targetDimensions; ++dim)
    {
        // Deterministic starting object keeps runs reproducible.
        const std::pair<int, int> pivotPair =
            choosePivots(distances, residual, 0);

        const int a = pivotPair.first;
        const int b = pivotPair.second;

        pivots.push_back(pivotPair);

        const double dab = residual[a][b];

        if (dab <= 0.0)
        {
            // No remaining separation: all coordinates in this
            // dimension are zero and later dimensions are also zero.
            break;
        }

        // Projection using the law of cosines:
        // x_i = (d(a,i)^2 + d(a,b)^2 - d(b,i)^2) / (2*d(a,b))
        for (int i = 0; i < n; ++i)
        {
            const double dai = residual[a][i];
            const double dbi = residual[b][i];

            coordinates[i][dim] =
                (dai * dai + dab * dab - dbi * dbi) /
                (2.0 * dab);
        }

        // Deflate the remaining distances by removing the
        // contribution represented by the current coordinate.
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                const double diff =
                    residual[i][j] *
                    residual[i][j] -
                    (coordinates[i][dim] - coordinates[j][dim]) *
                    (coordinates[i][dim] - coordinates[j][dim]);

                residual[i][j] =
                    std::sqrt(diff > 0.0 ? diff : 0.0);
            }
        }
    }

    return {coordinates, pivots};
}
