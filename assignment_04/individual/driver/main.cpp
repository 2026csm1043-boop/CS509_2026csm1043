#include "csr.h"
#include "vertex_coloring.h"
#include "pagerank.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using Clock = std::chrono::steady_clock;

static bool readVertexColoringFile(
    const std::string& filename,
    std::vector<std::vector<Edge>>& adj,
    int& V,
    int& E,
    std::string& error) {

    std::ifstream in(filename);
    if (!in) {
        error = "Cannot open input file: " + filename;
        return false;
    }

    if (!(in >> V >> E) || V <= 0 || E < 0) {
        error = "Invalid V/E header.";
        return false;
    }

    adj.assign(V, {});

    for (int i = 0; i < V; ++i) {
        int u, degree;

        if (!(in >> u >> degree)) {
            error = "Missing vertex/degree information at vertex " +
                    std::to_string(i) + ".";
            return false;
        }

        if (u != i) {
            error = "Vertex IDs must be 0..V-1 in order.";
            return false;
        }

        if (degree < 0) {
            error = "Negative degree for vertex " + std::to_string(u) + ".";
            return false;
        }

        for (int j = 0; j < degree; ++j) {
            int v;
            if (!(in >> v)) {
                error = "Missing neighbor for vertex " +
                        std::to_string(u) + ".";
                return false;
            }

            if (v < 0 || v >= V) {
                error = "Neighbor vertex ID out of range: " +
                        std::to_string(v) + ".";
                return false;
            }

            if (v == u) {
                error = "Self-loop found at vertex " +
                        std::to_string(u) + ".";
                return false;
            }

            adj[u].push_back({v, 1});
        }
    }

    // For an undirected graph, every edge must appear in both lists.
    // Check symmetry and reject duplicate neighbors.
    long long directedEntries = 0;

    for (int u = 0; u < V; ++u) {
        directedEntries += static_cast<long long>(adj[u].size());

        std::vector<int> neighbors;
        neighbors.reserve(adj[u].size());

        for (const Edge& e : adj[u])
            neighbors.push_back(e.vertex);

        std::sort(neighbors.begin(), neighbors.end());

        for (size_t i = 1; i < neighbors.size(); ++i) {
            if (neighbors[i] == neighbors[i - 1]) {
                error = "Parallel/duplicate edge found at vertex " +
                        std::to_string(u) + ".";
                return false;
            }
        }

        for (int v : neighbors) {
            bool found = false;

            for (const Edge& back : adj[v]) {
                if (back.vertex == u) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                error = "Undirected edge " + std::to_string(u) +
                        " - " + std::to_string(v) +
                        " is not present in both adjacency lists.";
                return false;
            }
        }
    }

    if (directedEntries != 2LL * E) {
        error = "Edge count mismatch: header E=" + std::to_string(E) +
                ", but adjacency lists contain " +
                std::to_string(directedEntries) +
                " entries; expected 2E.";
        return false;
    }

    return true;
}

static bool readPageRankFile(
    const std::string& filename,
    std::vector<std::vector<Edge>>& adj,
    int& V,
    int& E,
    double& damping,
    double& tolerance,
    int& maxIterations,
    std::string& error) {

    std::ifstream in(filename);
    if (!in) {
        error = "Cannot open input file: " + filename;
        return false;
    }

    if (!(in >> V >> E) || V <= 0 || E < 0) {
        error = "Invalid V/E header.";
        return false;
    }

    adj.assign(V, {});

    long long actualEdges = 0;

    for (int i = 0; i < V; ++i) {
        int u, outDegree;

        if (!(in >> u >> outDegree)) {
            error = "Missing vertex/outdegree information at vertex " +
                    std::to_string(i) + ".";
            return false;
        }

        if (u != i) {
            error = "Vertex IDs must be 0..V-1 in order.";
            return false;
        }

        if (outDegree < 0) {
            error = "Negative outdegree at vertex " +
                    std::to_string(u) + ".";
            return false;
        }

        for (int j = 0; j < outDegree; ++j) {
            int v;

            if (!(in >> v)) {
                error = "Missing outgoing neighbor for vertex " +
                        std::to_string(u) + ".";
                return false;
            }

            if (v < 0 || v >= V) {
                error = "Destination vertex ID out of range: " +
                        std::to_string(v) + ".";
                return false;
            }

            if (v == u) {
                error = "Self-loop found at vertex " +
                        std::to_string(u) + ".";
                return false;
            }

            adj[u].push_back({v, 1});
            ++actualEdges;
        }
    }

    if (actualEdges != E) {
        error = "Edge count mismatch: header E=" + std::to_string(E) +
                ", but read " + std::to_string(actualEdges) + " edges.";
        return false;
    }

    std::string label;

    if (!(in >> label >> damping) || label != "DAMPING") {
        error = "Expected: DAMPING <value>.";
        return false;
    }

    if (!(in >> label >> tolerance) || label != "TOLERANCE") {
        error = "Expected: TOLERANCE <value>.";
        return false;
    }

    if (!(in >> label >> maxIterations) || label != "MAX_ITERATIONS") {
        error = "Expected: MAX_ITERATIONS <value>.";
        return false;
    }

    if (!(damping > 0.0 && damping < 1.0)) {
        error = "Damping must satisfy 0 < damping < 1.";
        return false;
    }

    if (!(tolerance > 0.0) || !std::isfinite(tolerance)) {
        error = "Tolerance must be positive and finite.";
        return false;
    }

    if (maxIterations <= 0) {
        error = "MAX_ITERATIONS must be positive.";
        return false;
    }

    return true;
}

static void runColoring(const std::string& filename) {
    std::vector<std::vector<Edge>> adj;
    int V = 0, E = 0;
    std::string error;

    if (!readVertexColoringFile(filename, adj, V, E, error)) {
        std::cerr << "ERROR: " << error << '\n';
        return;
    }

    // Preprocessing: NOT included in algorithm timing.
    CSRGraph graph = convertToCSR(adj);

    const auto start = Clock::now();
    ColoringResult result = greedyVertexColoring(graph);
    const auto stop = Clock::now();

    const double milliseconds =
        std::chrono::duration<double, std::milli>(stop - start).count();

    std::cout << "Algorithm: Greedy Vertex Coloring\n";
    std::cout << "Vertices: " << V << '\n';
    std::cout << "Edges: " << E << '\n';
    std::cout << "Vertex colors:\n";

    for (int v = 0; v < V; ++v) {
        std::cout << v << ' ' << result.colors[v] << '\n';
    }

    std::cout << "Colors used: " << result.colorsUsed << '\n';
    std::cout << "Valid coloring: "
              << (result.valid ? "true" : "false") << '\n';

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Execution time: " << milliseconds << " ms\n";
}

static void runPageRank(const std::string& filename) {
    std::vector<std::vector<Edge>> adj;
    int V = 0, E = 0, maxIterations = 0;
    double damping = 0.0, tolerance = 0.0;
    std::string error;

    if (!readPageRankFile(filename, adj, V, E, damping,
                          tolerance, maxIterations, error)) {
        std::cerr << "ERROR: " << error << '\n';
        return;
    }

    // Preprocessing: NOT included in algorithm timing.
    CSRGraph graph = convertToCSR(adj);

    const auto start = Clock::now();
    PageRankResult result =
        pageRank(graph, damping, tolerance, maxIterations);
    const auto stop = Clock::now();

    const double milliseconds =
        std::chrono::duration<double, std::milli>(stop - start).count();

    double sum = std::accumulate(result.ranks.begin(),
                                 result.ranks.end(), 0.0);

    std::cout << "Algorithm: PageRank\n";
    std::cout << "Vertices: " << V << '\n';
    std::cout << "Edges: " << E << '\n';

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Damping: " << damping << '\n';

    std::cout << "Vertex ranks:\n";
    for (int v = 0; v < V; ++v) {
        std::cout << v << ' ' << result.ranks[v] << '\n';
    }

    std::cout << "Sum of ranks: " << sum << '\n';
    std::cout << "Iterations: " << result.iterations << '\n';
    std::cout << "Converged: "
              << (result.converged ? "true" : "false") << '\n';
    std::cout << "Execution time: " << milliseconds << " ms\n";
}

static void printUsage(const char* program) {
    std::cout << "Usage:\n"
              << "  " << program << " color <input-file>\n"
              << "  " << program << " pagerank <input-file>\n\n"
              << "Examples:\n"
              << "  " << program << " color tests/color_10.txt\n"
              << "  " << program << " pagerank tests/pagerank_10.txt\n";
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }

    const std::string algorithm = argv[1];
    const std::string filename = argv[2];

    if (algorithm == "color" || algorithm == "coloring") {
        runColoring(filename);
    } else if (algorithm == "pagerank" || algorithm == "pr") {
        runPageRank(filename);
    } else {
        std::cerr << "ERROR: Unknown algorithm '" << algorithm << "'.\n\n";
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}
