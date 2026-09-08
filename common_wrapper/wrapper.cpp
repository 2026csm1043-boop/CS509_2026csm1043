#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int runCommand(const string& command)
{
    cout << "\n> " << command << "\n";
    const int status = system(command.c_str());

    if (status != 0)
        cerr << "Error: command failed.\n";

    return status;
}

int compileAssignment(int a, const string& mode)
{
    if (a == 1 && mode == "individual")
        return runCommand("g++ -std=c++17 -O2 assignment_01/individual/src/gemm.cpp assignment_01/individual/src/csr.cpp assignment_01/individual/driver/main.cpp -Iassignment_01/individual/src -o assignment_01_ind.exe");
    if (a == 1 && mode == "buddy")
        return runCommand("g++ -std=c++17 -O2 assignment_01/buddy/src/graph.cpp assignment_01/buddy/src/bfs.cpp assignment_01/buddy/src/dfs.cpp assignment_01/buddy/src/sssp.cpp assignment_01/buddy/driver/main.cpp -Iassignment_01/buddy/src -o assignment_01_buddy.exe");
    if (a == 2 && mode == "individual")
        return runCommand("g++ -std=c++17 -O2 assignment_02/individual/src/algorithms.cpp assignment_02/individual/src/csr.cpp assignment_02/individual/driver/main.cpp -Iassignment_02/individual/src -o assignment_02_ind.exe");
    if (a == 2 && mode == "buddy")
        return runCommand("g++ -std=c++17 -O2 assignment_02/buddy/src/algorithms.cpp assignment_02/buddy/src/csr.cpp assignment_02/buddy/driver/main.cpp -Iassignment_02/buddy/src -o assignment_02_buddy.exe");
    if (a == 3 && mode == "individual")
        return runCommand("g++ -std=c++17 -O2 assignment_03/individual/src/algorithms.cpp assignment_03/individual/src/csr.cpp assignment_03/individual/driver/main.cpp -Iassignment_03/individual/src -o assignment_03_ind.exe");
    if (a == 3 && mode == "buddy")
        return runCommand("g++ -std=c++17 -O2 assignment_03/buddy/src/gradient_descent.cpp assignment_03/buddy/src/csr.cpp assignment_03/buddy/src/maxflow.cpp assignment_03/buddy/driver/assignment3_driver.cpp -Iassignment_03/buddy/src -o assignment_03_buddy.exe");
    if (a == 4 && mode == "individual")
        return runCommand("g++ -std=c++17 -O2 assignment_04/individual/src/csr.cpp assignment_04/individual/src/vertex_coloring.cpp assignment_04/individual/src/pagerank.cpp assignment_04/individual/driver/main.cpp -Iassignment_04/individual/src -o assignment_04_ind.exe");
    if (a == 4 && mode == "buddy")
        return runCommand("g++ -std=c++17 -O2 assignment_04/buddy/src/kmeans.cpp assignment_04/buddy/src/fastmap.cpp assignment_04/buddy/driver/assignment4_buddy.cpp -Iassignment_04/buddy/src -o assignment_04_buddy.exe");

    cerr << "Error: unknown assignment/mode.\n";
    return 1;
}

string executable(int a, const string& mode)
{
    return "assignment_0" + to_string(a) + "_" + mode + ".exe";
}

int runOne(int a, const string& mode, const string& algorithm, const string& file)
{
    const string exe = executable(a, mode);
    const string command = exe + " " + algorithm + " \"" + file + "\"";
    return runCommand(command);
}

int runAll(int a, const string& mode)
{
    const string prefix = "assignment_0" + to_string(a) + "/" + mode + "/tests/";

    if (a == 1 && mode == "individual") {
        runOne(a, mode, "gemm-simple", prefix + "gemm_test_01.txt");
        runOne(a, mode, "gemm-block", prefix + "gemm_test_01.txt");
        runOne(a, mode, "csr", prefix + "graph_10.txt");
        return 0;
    }

    if (a == 1 && mode == "buddy") {
        for (const string& alg : {"BFS", "DFS", "SSSP"})
            for (int n : {10, 100, 10000, 50000, 100000})
                runOne(a, mode, alg, prefix + string(alg == "BFS" ? "bfs_" : alg == "DFS" ? "dfs_" : "sssp_") + to_string(n) + ".txt");
        return 0;
    }

    if (a == 2 && mode == "individual") {
        for (int n : {10, 100, 10000, 50000, 100000})
            runOne(a, mode, "bf", prefix + "bf_" + to_string(n) + ".txt");
        for (int n : {10, 100, 500, 1000, 2000})
            runOne(a, mode, "fw", prefix + "fw_" + to_string(n) + ".txt");
        return 0;
    }

    if (a == 2 && mode == "buddy") {
        for (int n : {10, 100, 10000, 50000, 100000})
            runOne(a, mode, "tc", prefix + "tc_" + to_string(n) + ".txt");
        for (int n : {10, 100, 1000, 5000, 10000})
            runOne(a, mode, "bc", prefix + "bc_" + to_string(n) + ".txt");
        for (int n : {10, 100, 10000, 50000, 100000})
            runOne(a, mode, "cc", prefix + "cc_" + to_string(n) + ".txt");
        return 0;
    }

    if (a == 3 && mode == "individual") {
        for (int n : {10, 100, 10000, 50000, 100000}) {
            runOne(a, mode, "mst-k", prefix + "mst_" + to_string(n) + ".txt");
            runOne(a, mode, "mst-p", prefix + "mst_" + to_string(n) + ".txt");
        }
        return 0;
    }

    if (a == 3 && mode == "buddy") {
        for (int i = 1; i <= 5; ++i) {
            const string s = (i < 10 ? "0" : "") + to_string(i);
            runOne(a, mode, "gd", prefix + "gd_" + s + ".txt");
        }
        for (int n : {10, 100, 1000, 10000, 50000})
            runOne(a, mode, "maxflow", prefix + "maxflow_" + to_string(n) + ".txt");
        return 0;
    }

    if (a == 4 && mode == "individual") {
        for (const string& file : {"color_example.txt", "color_10.txt", "color_100.txt", "color_10000.txt", "color_50000.txt", "color_100000.txt"})
            runOne(a, mode, "color", prefix + file);
        for (const string& file : {"pagerank_example.txt", "pagerank_10.txt", "pagerank_100.txt", "pagerank_1000.txt", "pagerank_10000.txt", "pagerank_50000.txt"})
            runOne(a, mode, "pagerank", prefix + file);
        return 0;
    }

    if (a == 4 && mode == "buddy") {
        for (int i = 1; i <= 4; ++i) {
            const string s = (i < 10 ? "0" : "") + to_string(i);
            runOne(a, mode, "kmeans", prefix + "km_" + s + ".txt");
            runOne(a, mode, "fastmap", prefix + "fm_" + s + ".txt");
        }
        return 0;
    }

    cerr << "Error: unknown assignment/mode.\n";
    return 1;
}

void menu()
{
    cout << "\nCS509 Lab Common Wrapper\n"
         << "1. Compile one assignment\n"
         << "2. Run one test file\n"
         << "3. Run all tests for one assignment/mode\n"
         << "4. Compile all assignments\n"
         << "5. Compile and run all submitted assignments\n"
         << "0. Exit\n"
         << "Choice: ";
}

bool readAssignmentMode(int& a, string& mode)
{
    cout << "Assignment (1-4): ";
    cin >> a;
    if (!cin || a < 1 || a > 4) {
        cerr << "Error: assignment must be 1, 2, 3, or 4.\n";
        return false;
    }

    cout << "Mode (individual/buddy): ";
    cin >> mode;
    if (mode != "individual" && mode != "buddy") {
        cerr << "Error: mode must be individual or buddy.\n";
        return false;
    }
    return true;
}

int main()
{
    while (true) {
        menu();

        int choice;
        cin >> choice;

        if (!cin) {
            cerr << "Error: invalid menu input.\n";
            return 1;
        }

        if (choice == 0)
            return 0;

        if (choice == 1) {
            int a; string mode;
            if (readAssignmentMode(a, mode))
                compileAssignment(a, mode);
        }
        else if (choice == 2) {
            int a; string mode;
            if (!readAssignmentMode(a, mode))
                continue;

            string algorithm, file;
            cout << "Algorithm: ";
            cin >> algorithm;
            cout << "Test file path: ";
            cin >> file;
            runOne(a, mode, algorithm, file);
        }
        else if (choice == 3) {
            int a; string mode;
            if (readAssignmentMode(a, mode))
                runAll(a, mode);
        }
        else if (choice == 4) {
            for (int a = 1; a <= 4; ++a) {
                compileAssignment(a, "individual");
                compileAssignment(a, "buddy");
            }
        }
        else if (choice == 5) {
            bool ok = true;
            for (int a = 1; a <= 4; ++a) {
                for (const string& mode : {"individual", "buddy"}) {
                    if (compileAssignment(a, mode) != 0)
                        ok = false;
                    else
                        runAll(a, mode);
                }
            }
            if (!ok)
                cerr << "One or more compilations failed.\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
}
