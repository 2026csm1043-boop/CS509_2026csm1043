#include "graph.h"
#include "bfs.h"
#include "dfs.h"
#include "sssp.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>

using namespace std;
using namespace chrono;

void printBFSResult(const BFSResult& result, int source, double timeMs) {
    cout << "Algorithm: BFS" << endl;
    cout << "Source: " << source << endl;

    cout << "Traversal: ";

    for (size_t i = 0; i < result.traversal.size(); i++) {
        cout << result.traversal[i];

        if (i + 1 < result.traversal.size()) {
            cout << " ";
        }
    }

    cout << endl;

    cout << "Distances:" << endl;

    for (size_t i = 0; i < result.distance.size(); i++) {
        cout << i << " " << result.distance[i] << endl;
    }

    cout << fixed << setprecision(6);
    cout << "Execution time: " << timeMs << " ms" << endl;
}

void printDFSResult(const vector<int>& traversal,
                    int source,
                    double timeMs) {

    cout << "Algorithm: DFS" << endl;
    cout << "Source: " << source << endl;

    cout << "Traversal: ";

    for (size_t i = 0; i < traversal.size(); i++) {
        cout << traversal[i];

        if (i + 1 < traversal.size()) {
            cout << " ";
        }
    }

    cout << endl;

    cout << fixed << setprecision(6);
    cout << "Execution time: " << timeMs << " ms" << endl;
}

void printSSSPResult(const vector<long long>& distance,
                     int source,
                     double timeMs) {

    cout << "Algorithm: SSSP" << endl;
    cout << "Source: " << source << endl;

    cout << "Vertex Distance" << endl;

    for (size_t i = 0; i < distance.size(); i++) {
        cout << i << " ";

        if (distance[i] == INF) {
            cout << "INF";
        } else {
            cout << distance[i];
        }

        cout << endl;
    }

    cout << fixed << setprecision(6);
    cout << "Execution time: " << timeMs << " ms" << endl;
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cerr << "Usage: ./buddy <algorithm> <input_file>" << endl;
        cerr << "Algorithm: BFS, DFS or SSSP" << endl;
        return 1;
    }

    string algorithm = argv[1];
    string filename = argv[2];

    if (algorithm != "BFS" &&
        algorithm != "DFS" &&
        algorithm != "SSSP") {

        cerr << "Error: invalid algorithm." << endl;
        cerr << "Use BFS, DFS or SSSP." << endl;
        return 1;
    }

    bool weighted = (algorithm == "SSSP");

    InputGraph inputGraph;

    if (!readGraph(filename, weighted, inputGraph)) {
        return 1;
    }

    /*
     * CSR conversion is preprocessing.
     * Therefore it is intentionally performed before timing.
     */
    CSRGraph csrGraph = convertToCSR(inputGraph);

    if (algorithm == "BFS") {

        auto start = high_resolution_clock::now();

        BFSResult result = bfs(csrGraph, inputGraph.source);

        auto end = high_resolution_clock::now();

        double timeMs =
            duration<double, milli>(end - start).count();

        printBFSResult(result, inputGraph.source, timeMs);
    }

    else if (algorithm == "DFS") {

        auto start = high_resolution_clock::now();

        vector<int> traversal =
            dfs(csrGraph, inputGraph.source);

        auto end = high_resolution_clock::now();

        double timeMs =
            duration<double, milli>(end - start).count();

        printDFSResult(
            traversal,
            inputGraph.source,
            timeMs
        );
    }

    else if (algorithm == "SSSP") {

        auto start = high_resolution_clock::now();

        vector<long long> distance =
            sssp(csrGraph, inputGraph.source);

        auto end = high_resolution_clock::now();

        double timeMs =
            duration<double, milli>(end - start).count();

        printSSSPResult(
            distance,
            inputGraph.source,
            timeMs
        );
    }

    return 0;
}