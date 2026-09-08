#include "../src/csr.h"
#include "../src/gradient_descent.h"
#include "../src/maxflow.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

bool readGradientDescentInput(
    const string& filename,
    int& degree,
    vector<double>& coefficients,
    double& initialX,
    double& learningRate,
    double& tolerance,
    int& maxIterations)
{
    ifstream input(filename);

    if (!input.is_open())
    {
        cerr << "Error: Could not open input file: "
             << filename << '\n';
        return false;
    }

    string label;

    if (!(input >> label >> degree) || label != "DEGREE")
    {
        cerr << "Error: Invalid DEGREE line.\n";
        return false;
    }

    if (degree < 1)
    {
        cerr << "Error: Invalid degree.\n";
        return false;
    }

    if (!(input >> label) || label != "COEFFICIENTS")
    {
        cerr << "Error: Invalid COEFFICIENTS line.\n";
        return false;
    }

    coefficients.resize(degree + 1);

    for (int i = 0; i <= degree; ++i)
    {
        if (!(input >> coefficients[i]))
        {
            cerr << "Error: Coefficient count must be d+1.\n";
            return false;
        }
    }

    if (!(input >> label >> initialX) || label != "INITIAL_X")
    {
        cerr << "Error: Invalid INITIAL_X line.\n";
        return false;
    }

    if (!(input >> label >> learningRate) ||
        label != "LEARNING_RATE")
    {
        cerr << "Error: Invalid LEARNING_RATE line.\n";
        return false;
    }

    if (learningRate <= 0)
    {
        cerr << "Error: LEARNING_RATE must be positive.\n";
        return false;
    }

    if (!(input >> label >> tolerance) || label != "TOLERANCE")
    {
        cerr << "Error: Invalid TOLERANCE line.\n";
        return false;
    }

    if (tolerance <= 0)
    {
        cerr << "Error: TOLERANCE must be positive.\n";
        return false;
    }

    if (!(input >> label >> maxIterations) ||
        label != "MAX_ITERATIONS")
    {
        cerr << "Error: Invalid MAX_ITERATIONS line.\n";
        return false;
    }

    if (maxIterations <= 0)
    {
        cerr << "Error: MAX_ITERATIONS must be positive.\n";
        return false;
    }

    return true;
}

bool readMaxflowInput(
    const string& filename,
    WeightedAdjList& adjacencyList,
    int& vertices,
    int& edges,
    int& source,
    int& sink)
{
    ifstream input(filename);

    if (!input.is_open())
    {
        cerr << "Error: Could not open input file: "
             << filename << '\n';
        return false;
    }

    if (!(input >> vertices >> edges) ||
        vertices <= 0 ||
        edges < 0)
    {
        cerr << "Error: Invalid graph header.\n";
        return false;
    }

    adjacencyList.assign(vertices, {});

    for (int i = 0; i < vertices; ++i)
    {
        int u;
        int degree;

        if (!(input >> u >> degree))
        {
            cerr << "Error: Invalid adjacency-list entry.\n";
            return false;
        }

        if (u < 0 || u >= vertices || degree < 0)
        {
            cerr << "Error: Invalid vertex or degree.\n";
            return false;
        }

        for (int j = 0; j < degree; ++j)
        {
            int v;
            int capacity;

            if (!(input >> v >> capacity))
            {
                cerr << "Error: Invalid edge entry.\n";
                return false;
            }

            if (v < 0 || v >= vertices)
            {
                cerr << "Error: Invalid neighbour vertex.\n";
                return false;
            }

            if (capacity < 0)
            {
                cerr << "Error: Negative capacities are not allowed.\n";
                return false;
            }

            adjacencyList[u].push_back({v, capacity});
        }
    }

    string label;

    if (!(input >> label >> source) || label != "SOURCE")
    {
        cerr << "Error: Invalid SOURCE line.\n";
        return false;
    }

    if (!(input >> label >> sink) || label != "SINK")
    {
        cerr << "Error: Invalid SINK line.\n";
        return false;
    }

    if (source < 0 || source >= vertices ||
        sink < 0 || sink >= vertices)
    {
        cerr << "Error: Invalid source/sink vertex.\n";
        return false;
    }

    if (source == sink)
    {
        cerr << "Error: Source and sink must be different.\n";
        return false;
    }

    return true;
}

void printUsage()
{
    cout << "Usage:\n";
    cout << "  assignment_03.exe gd <input_file>\n";
    cout << "  assignment_03.exe maxflow <input_file>\n";
}

int runGradientDescent(const string& filename)
{
    int degree;
    vector<double> coefficients;
    double initialX;
    double learningRate;
    double tolerance;
    int maxIterations;

    if (!readGradientDescentInput(
            filename,
            degree,
            coefficients,
            initialX,
            learningRate,
            tolerance,
            maxIterations))
    {
        return 1;
    }

    // Only the algorithm call is timed.
    const auto start = chrono::high_resolution_clock::now();

    GradientDescentResult result =
        gradientDescent(
            coefficients,
            initialX,
            learningRate,
            tolerance,
            maxIterations);

    const auto end = chrono::high_resolution_clock::now();

    const chrono::duration<double, milli> elapsed =
        end - start;

    cout << "Algorithm: Gradient Descent\n";
    cout << "Degree: " << degree << '\n';
    cout << fixed << setprecision(10);
    cout << "Final x: " << result.finalX << '\n';
    cout << "Final f(x): " << result.finalValue << '\n';
    cout << "Iterations: " << result.iterations << '\n';
    cout << "Converged: "
         << (result.converged ? "true" : "false") << '\n';
    cout << "Execution time: "
         << elapsed.count()
         << " ms\n";

    return 0;
}

int runMaxflow(const string& filename)
{
    WeightedAdjList adjacencyList;
    int vertices;
    int edges;
    int source;
    int sink;

    if (!readMaxflowInput(
            filename,
            adjacencyList,
            vertices,
            edges,
            source,
            sink))
    {
        return 1;
    }

    // CSR conversion is preprocessing and is outside the timed region.
    CSRGraph graph = convertToCSR(adjacencyList);

    // Only the max-flow/min-cut algorithm is timed.
    const auto start = chrono::high_resolution_clock::now();

    MaxflowResult result =
        maxflowMincut(graph, source, sink);

    const auto end = chrono::high_resolution_clock::now();

    const chrono::duration<double, milli> elapsed =
        end - start;

    cout << "Algorithm: Maxflow-Mincut\n";
    cout << "Source: " << source << '\n';
    cout << "Sink: " << sink << '\n';
    cout << "Maximum flow: " << result.maximumFlow << '\n';
    cout << "Minimum cut capacity: "
         << result.minimumCutCapacity << '\n';

    cout << "Source side:";
    for (int v : result.sourceSide)
    {
        cout << ' ' << v;
    }
    cout << '\n';

    cout << "Sink side:";
    for (int v : result.sinkSide)
    {
        cout << ' ' << v;
    }
    cout << '\n';

    cout << "Cut edges:\n";

    for (const CutEdge& edge : result.cutEdges)
    {
        cout << edge.from << ' '
             << edge.to << ' '
             << edge.capacity << '\n';
    }

    cout << fixed << setprecision(3);
    cout << "Execution time: "
         << elapsed.count()
         << " ms\n";

    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printUsage();
        return 1;
    }

    const string algorithm = argv[1];
    const string filename = argv[2];

    if (algorithm == "gd")
    {
        return runGradientDescent(filename);
    }

    if (algorithm == "maxflow")
    {
        return runMaxflow(filename);
    }

    cerr << "Error: Unknown algorithm '" << algorithm << "'.\n";
    printUsage();

    return 1;
}
