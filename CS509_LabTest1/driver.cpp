#include "src/algorithms.h"
#include "src/csr.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using Clock = std::chrono::steady_clock;

static double ms(Clock::time_point a, Clock::time_point b) {
    return std::chrono::duration<double, std::milli>(b - a).count();
}

static void usage(const char* p) {
    std::cerr << "Usage:\n"
              << "  " << p << " csr <edge_list>\n"
              << "  " << p << " fw <edge_list> <block_size>\n"
              << "  " << p << " fw-bench <edge_list>\n"
              << "  " << p << " validate <edge_list> <source>\n";
}

static bool load(const std::string& file, CSRGraph& graph) {
    if (!readEdgeListCSR(file, graph)) return false;
    std::cout << "Graph: " << (graph.directed ? "directed" : "undirected")
              << ", V = " << graph.V
              << ", input E = " << graph.inputE
              << ", CSR entries = " << graph.entries << "\n";
    return true;
}

static int runCSR(const std::string& file) {
    CSRGraph graph;
    if (!load(file, graph)) return 1;

    std::cout << "Algorithm: CSR Conversion\n";
    printCSR(graph);
    return 0;
}

static int runFW(const std::string& file, int B) {
    if (B <= 0) {
        std::cerr << "Error: block size must be positive.\n";
        return 1;
    }

    CSRGraph graph;
    if (!load(file, graph)) return 1;

    auto base = initialDistanceMatrix(graph);
    auto naive = base;
    auto blocked = base;

    auto t1 = Clock::now();
    bool naiveNeg = floydWarshall(naive);
    auto t2 = Clock::now();

    auto t3 = Clock::now();
    bool blockedNeg = blockedFloydWarshall(blocked, B);
    auto t4 = Clock::now();

    bool ok = sameMatrix(naive, blocked) && (naiveNeg == blockedNeg);

    std::cout << "Block size: " << B << '\n';
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Naive Runtime: " << ms(t1, t2) << " ms\n";
    std::cout << "Blocked Runtime: " << ms(t3, t4) << " ms\n";
    std::cout << "Correctness: " << (ok ? "PASS" : "FAIL") << '\n';
    return ok ? 0 : 2;
}

static int runFWBench(const std::string& file) {
    CSRGraph graph;
    if (!load(file, graph)) return 1;

    const std::vector<int> blocks = {8, 16, 32, 64, 128};
    auto reference = initialDistanceMatrix(graph);
    auto naive = reference;
    auto n0 = Clock::now();
    bool naiveNeg = floydWarshall(naive);
    auto n1 = Clock::now();
    double naiveMs = ms(n0, n1);

    std::cout << "\nV | Block size | Runtime (ms) | Correctness\n";
    std::cout << "--|------------|---------------|------------\n";
    for (int B : blocks) {
        auto dist = reference;
        auto t0 = Clock::now();
        bool neg = blockedFloydWarshall(dist, B);
        auto t1 = Clock::now();
        bool ok = sameMatrix(naive, dist) && (naiveNeg == neg);

        std::cout << graph.V << " | " << B << " | " << std::fixed
                  << std::setprecision(6) << ms(t0, t1) << " | "
                  << (ok ? "PASS" : "FAIL") << '\n';
    }
    std::cout << "Naive Runtime: " << std::fixed << std::setprecision(6)
              << naiveMs << " ms\n";
    return 0;
}

static int runValidation(const std::string& file, int source) {
    CSRGraph graph;
    if (!load(file, graph)) return 1;
    if (source < 0 || source >= graph.V) {
        std::cerr << "Error: source out of range.\n";
        return 1;
    }

    std::cout << "\n========================\n"
              << "Graph Validation\n"
              << "========================\n";

    // 1. CC <-> BFS
    auto t0 = Clock::now();
    int componentCount = 0;
    std::vector<int> component = connectedComponents(graph, componentCount);

    std::vector<int> componentSize(componentCount, 0);
    std::vector<int> representative(componentCount, -1);
    for (int v = 0; v < graph.V; ++v) {
        ++componentSize[component[v]];
        if (representative[component[v]] == -1)
            representative[component[v]] = v;
    }

    bool check1 = true;
    int badComponent = -1;
    long long totalBFSReached = 0;
    for (int c = 0; c < componentCount; ++c) {
        BFSResult r = bfs(graph, representative[c]);
        int reached = static_cast<int>(r.traversal.size());
        totalBFSReached += reached;
        if (reached != componentSize[c]) {
            check1 = false;
            if (badComponent == -1) badComponent = c;
        }
    }
    auto t1 = Clock::now();

    // 2. BFS <-> SSSP, treating every edge as weight 1.
    auto t2 = Clock::now();
    BFSResult bfsSource = bfs(graph, source);
    std::vector<long long> ssspUnit = sssp(graph, source, false);
    bool check2 = true;
    int badVertex2 = -1;
    for (int v = 0; v < graph.V; ++v) {
        bool bfsReachable = bfsSource.distance[v] != -1;
        bool ssspReachable = ssspUnit[v] != INF;
        if (bfsReachable != ssspReachable ||
            (bfsReachable && static_cast<long long>(bfsSource.distance[v]) != ssspUnit[v])) {
            check2 = false;
            if (badVertex2 == -1) badVertex2 = v;
        }
    }
    auto t3 = Clock::now();

    // 3. TC <-> CC.  Every triangle must have all three vertices in the same CC.
    auto t4 = Clock::now();
    TriangleResult tc = triangleCounting(graph, true);
    std::vector<long long> trianglesPerComponent(componentCount, 0);
    bool check3 = true;
    int badTriangle = -1;
    for (size_t i = 0; i < tc.triangles.size(); ++i) {
        auto [a, b, c] = tc.triangles[i];
        if (component[a] != component[b] || component[b] != component[c]) {
            check3 = false;
            badTriangle = static_cast<int>(i);
            break;
        }
        ++trianglesPerComponent[component[a]];
    }
    long long sumTriangles = std::accumulate(
        trianglesPerComponent.begin(), trianglesPerComponent.end(), 0LL);
    if (sumTriangles != tc.count) check3 = false;
    auto t5 = Clock::now();

    // 4. BC <-> CC.  Full BC is computed on the same CSR.  For each component,
    // BC is recomputed while restricting traversal to that component, still
    // using the same CSR arrays. Values must agree component-wise.
    auto t6 = Clock::now();
    std::vector<double> fullBC = betweennessCentrality(graph, component, -1);
    bool check4 = true;
    int badVertex4 = -1;
    constexpr double EPS = 1e-9;

    for (int c = 0; c < componentCount && check4; ++c) {
        std::vector<double> localBC = betweennessCentrality(graph, component, c);
        for (int v = 0; v < graph.V; ++v) {
            if (component[v] != c) continue;
            if (std::fabs(fullBC[v] - localBC[v]) > EPS) {
                check4 = false;
                badVertex4 = v;
                break;
            }
        }
    }
    auto t7 = Clock::now();

    std::cout << "CC <-> BFS:      " << (check1 ? "PASS" : "FAIL") << '\n';
    std::cout << "BFS <-> SSSP:    " << (check2 ? "PASS" : "FAIL") << '\n';
    std::cout << "TC <-> CC:       " << (check3 ? "PASS" : "FAIL") << '\n';
    std::cout << "BC <-> CC:       " << (check4 ? "PASS" : "FAIL") << '\n';
    std::cout << "\nOverall:          "
              << (check1 && check2 && check3 && check4 ? "PASS" : "FAIL") << '\n';
    std::cout << "========================\n";

    std::cout << "\nRuntime:\n";
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Validation 1: " << ms(t0, t1) << " ms\n";
    std::cout << "Validation 2: " << ms(t2, t3) << " ms\n";
    std::cout << "Validation 3: " << ms(t4, t5) << " ms\n";
    std::cout << "Validation 4: " << ms(t6, t7) << " ms\n";
    std::cout << "Total        : " << ms(t0, t7) << " ms\n";

    if (!check1) std::cout << "First CC/BFS inconsistency: component " << badComponent << '\n';
    if (!check2) std::cout << "First BFS/SSSP inconsistency: vertex " << badVertex2 << '\n';
    if (!check3) std::cout << "First TC/CC inconsistency: triangle index " << badTriangle << '\n';
    if (!check4) std::cout << "First BC/CC inconsistency: vertex " << badVertex4 << '\n';

    return (check1 && check2 && check3 && check4) ? 0 : 2;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        usage(argv[0]);
        return 1;
    }

    std::string command = argv[1];
    std::string file = argv[2];

    if (command == "csr") return runCSR(file);
    if (command == "fw") {
        if (argc < 4) {
            usage(argv[0]);
            return 1;
        }
        return runFW(file, std::stoi(argv[3]));
    }
    if (command == "fw-bench") return runFWBench(file);
    if (command == "validate") {
        if (argc < 4) {
            usage(argv[0]);
            return 1;
        }
        return runValidation(file, std::stoi(argv[3]));
    }

    usage(argv[0]);
    return 1;
}
