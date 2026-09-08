#include "../src/kmeans.h"
#include "../src/fastmap.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using Clock = std::chrono::high_resolution_clock;

namespace
{
bool readKMeansFile(
    const std::string& filename,
    std::vector<std::vector<double>>& points,
    int& k,
    int& maxIterations,
    double& tolerance)
{
    std::ifstream input(filename);

    if (!input)
    {
        std::cerr << "Error: Cannot open input file: "
                  << filename << '\n';
        return false;
    }

    int n;
    int dimensions;

    if (!(input >> n >> dimensions >> k))
    {
        std::cerr << "Error: Invalid K-Means header.\n";
        return false;
    }

    if (n <= 0 || dimensions <= 0 || k <= 0 || k > n)
    {
        std::cerr << "Error: Invalid N, D, or K.\n";
        return false;
    }

    points.assign(n, std::vector<double>(dimensions));

    for (int i = 0; i < n; ++i)
    {
        for (int d = 0; d < dimensions; ++d)
        {
            if (!(input >> points[i][d]))
            {
                std::cerr << "Error: Invalid point data.\n";
                return false;
            }
        }
    }

    std::string label;

    if (!(input >> label >> maxIterations) ||
        label != "MAX_ITERATIONS")
    {
        std::cerr << "Error: Missing MAX_ITERATIONS.\n";
        return false;
    }

    if (!(input >> label >> tolerance) ||
        label != "TOLERANCE")
    {
        std::cerr << "Error: Missing TOLERANCE.\n";
        return false;
    }

    if (maxIterations <= 0 || tolerance <= 0.0)
    {
        std::cerr << "Error: Invalid iteration/tolerance values.\n";
        return false;
    }

    return true;
}

bool readFastMapFile(
    const std::string& filename,
    std::vector<std::vector<double>>& distances,
    int& targetDimensions)
{
    std::ifstream input(filename);

    if (!input)
    {
        std::cerr << "Error: Cannot open input file: "
                  << filename << '\n';
        return false;
    }

    int n;

    if (!(input >> n >> targetDimensions))
    {
        std::cerr << "Error: Invalid FastMap header.\n";
        return false;
    }

    if (n <= 0 || targetDimensions <= 0 ||
        targetDimensions >= n)
    {
        std::cerr << "Error: Invalid N or target dimensionality.\n";
        return false;
    }

    distances.assign(n, std::vector<double>(n));

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (!(input >> distances[i][j]))
            {
                std::cerr << "Error: Invalid distance matrix.\n";
                return false;
            }

            if (distances[i][j] < 0.0)
            {
                std::cerr << "Error: Negative distance.\n";
                return false;
            }
        }
    }

    constexpr double EPS = 1e-9;

    for (int i = 0; i < n; ++i)
    {
        if (std::abs(distances[i][i]) > EPS)
        {
            std::cerr << "Error: Distance matrix diagonal is not zero.\n";
            return false;
        }

        for (int j = i + 1; j < n; ++j)
        {
            if (std::abs(distances[i][j] - distances[j][i]) > EPS)
            {
                std::cerr << "Error: Distance matrix is not symmetric.\n";
                return false;
            }
        }
    }

    return true;
}

void runKMeans(const std::string& filename)
{
    std::vector<std::vector<double>> points;
    int k;
    int maxIterations;
    double tolerance;

    if (!readKMeansFile(
            filename,
            points,
            k,
            maxIterations,
            tolerance))
    {
        return;
    }

    const auto start = Clock::now();

    const KMeansResult result =
        kMeans(points, k, maxIterations, tolerance);

    const auto stop = Clock::now();

    const double milliseconds =
        std::chrono::duration<double, std::milli>(
            stop - start).count();

    std::cout << "\nAlgorithm: K-Means Clustering\n";
    std::cout << "K: " << k << '\n';

    std::cout << "Point assignments:\n";
    for (std::size_t i = 0; i < result.assignments.size(); ++i)
    {
        std::cout << i << ' '
                  << result.assignments[i] << '\n';
    }

    std::cout << "\nFinal centroids:\n";
    std::cout << std::fixed << std::setprecision(6);

    for (int c = 0; c < k; ++c)
    {
        std::cout << c << ':';

        for (double value : result.centroids[c])
        {
            std::cout << ' ' << value;
        }

        std::cout << '\n';
    }

    std::cout << "WCSS: " << result.wcss << '\n';
    std::cout << "Iterations: " << result.iterations << '\n';
    std::cout << "Converged: "
              << (result.converged ? "true" : "false") << '\n';
    std::cout << "Execution time: "
              << milliseconds << " ms\n";
}

void runFastMap(const std::string& filename)
{
    std::vector<std::vector<double>> distances;
    int targetDimensions;

    if (!readFastMapFile(
            filename,
            distances,
            targetDimensions))
    {
        return;
    }

    const auto start = Clock::now();

    const FastMapResult result =
        fastMap(distances, targetDimensions);

    const auto stop = Clock::now();

    const double milliseconds =
        std::chrono::duration<double, std::milli>(
            stop - start).count();

    std::cout << "\nAlgorithm: FastMap\n";
    std::cout << "Target dimensions: "
              << targetDimensions << '\n';

    std::cout << "Pivots per dimension:\n";

    for (std::size_t d = 0; d < result.pivots.size(); ++d)
    {
        std::cout << "Dim " << d + 1 << ": "
                  << result.pivots[d].first << ' '
                  << result.pivots[d].second << '\n';
    }

    std::cout << "\nObject coordinates:\n";
    std::cout << std::fixed << std::setprecision(6);

    for (std::size_t i = 0; i < result.coordinates.size(); ++i)
    {
        std::cout << i << ':';

        for (double value : result.coordinates[i])
        {
            std::cout << ' ' << value;
        }

        std::cout << '\n';
    }

    std::cout << "Execution time: "
              << milliseconds << " ms\n";
}
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr
            << "Usage: assignment4_buddy.exe "
            << "<kmeans|fastmap> <input-file>\n";
        return 1;
    }

    const std::string algorithm = argv[1];
    const std::string filename = argv[2];

    if (algorithm == "kmeans")
    {
        runKMeans(filename);
    }
    else if (algorithm == "fastmap")
    {
        runFastMap(filename);
    }
    else
    {
        std::cerr << "Error: Unknown algorithm.\n";
        return 1;
    }

    return 0;
}
