# CS509 Laboratory Repository

## Repository Overview

This repository contains the CS509 PGSL laboratory work for First-Year M.Tech CSE, 2026. It contains the supplied Individual and Double / Buddy assignment implementations, dedicated drivers, test files, a repository-level common wrapper, execution logs, and reports.

**Important:** the assignment source code supplied in the input archives has been preserved. No algorithm implementation was rewritten. Assignment 4 Buddy source and driver files were copied from the separately supplied Buddy solution archive.

## Student / Pair Details

### Individual

- **Name:** Sejal Gupta
- **Entry Number:** 2026CSM1043
- **Repository naming pattern:** `CS509_<EntryNumber>`

### Double / Buddy

- **Student 1:** Harshita Mahor — 2026CSM1016
- **Student 2:** Sejal Gupta — 2026CSM1043
- **Shared repository naming pattern:** `CS509_<EntryNumber1>_<EntryNumber2>`
- **Reference repository shown in the supplied report:** `https://github.com/2026csm1016-lab/CS509_CSM1016_CSM1043`

## Language and Environment

- Programming language: **C++**
- C++ standard: **C++17**
- Compiler: **g++ 14.2.0**
- Compilation flags: **`-std=c++17 -O2`**
- Timing unit: **milliseconds (ms)**
- Timing API used by the supplied drivers: `std::chrono`
- Execution environment used for the verification run: Intel Xeon E5-2673 v4 @ 2.30 GHz, 3 available CPUs.

The algorithm timer is placed immediately around the supplied algorithm call where the driver already implements timing. Input reading, parsing, validation and CSR conversion are outside the timed region in the graph drivers that explicitly perform CSR preprocessing.

## Directory Structure

```text
CS509_2026CSM1043/
|-- README.md
|-- common_wrapper/
|   `-- wrapper.cpp
|-- assignment_01/
|   |-- README.md
|   |-- individual/
|   |   |-- src/
|   |   |-- driver/
|   |   `-- tests/
|   `-- buddy/
|       |-- src/
|       |-- driver/
|       `-- tests/
|-- assignment_02/
|   |-- README.md
|   |-- individual/
|   `-- buddy/
|-- assignment_03/
|   |-- README.md
|   |-- individual/
|   `-- buddy/
|-- assignment_04/
|   |-- README.md
|   |-- individual/
|   `-- buddy/
|-- execution_outputs/
|   `-- raw stdout/stderr/status files from verification runs
`-- reports/
    `-- Individual and Buddy PDF reports
```

Each assignment mode follows the required `src/`, `driver/`, and `tests/` separation. The dedicated driver remains separate from the common wrapper.

## Common Wrapper: Build and Usage

Compile the wrapper from the repository root:

```bash
g++ -std=c++17 -O2 common_wrapper/wrapper.cpp -o common_wrapper.exe
```

Run it:

```bash
./common_wrapper.exe
```

The wrapper provides:

1. Compile one selected assignment and mode.
2. Run one selected test file through the dedicated assignment driver.
3. Run all test files for one assignment/mode.
4. Compile all eight assignment/mode combinations.
5. Compile and run all submitted assignment/mode combinations.
6. Clear errors when a compilation or requested source/test/executable is unavailable.

The wrapper invokes the **dedicated driver**; it does not replace the driver.

## Test and Output Conventions

- Test files are kept inside the corresponding assignment/mode `tests/` directory.
- Sequential names such as `gd_01.txt` are retained where supplied.
- Size-based graph names such as `mst_10000.txt` and `pagerank_50000.txt` are retained.
- Raw outputs from the verification run are stored in `execution_outputs/`.
- Each output contains the algorithm result followed by the execution time when the supplied driver reports timing.
- For very large Bellman-Ford inputs where the supplied implementation did not finish during the bounded verification run, the status is recorded as a timeout rather than an invented timing value.

## Assignment Summary

| Assignment | Individual | Double / Buddy |
|---|---|---|
| 01 | GEMM Simple, GEMM Blocking, CSR conversion support | BFS, DFS, positive-weight SSSP |
| 02 | Bellman-Ford, Floyd-Warshall | Triangle Counting, Betweenness Centrality, Connected Components |
| 03 | Kruskal MST, Prim MST | Gradient Descent, Maxflow-Mincut |
| 04 | Greedy Vertex Coloring, PageRank | K-Means Clustering, FastMap |

## Reports

The `reports/` directory contains one report for every supplied assignment/mode combination:

- Assignment 1 — Individual
- Assignment 1 — Buddy
- Assignment 2 — Individual
- Assignment 2 — Buddy
- Assignment 3 — Individual
- Assignment 3 — Buddy
- Assignment 4 — Individual
- Assignment 4 — Buddy

The reports follow the supplied Assignment 3 Buddy report's overall organization: objective, algorithms, contributions where applicable, test cases, execution/performance, observations, and conclusion.

## Timing Note

The requested laboratory guideline says that only algorithm execution should be timed. The supplied drivers have been retained unchanged. Consequently, where a supplied driver does not expose a timing measurement (for example, the Assignment 1 CSR conversion command), the report explicitly states that the timing was not reported by the supplied driver instead of modifying the assignment code.

## Verification Note

All assignment source and driver files originally present in the main archive were compiled with C++17 and `-O2`. Assignment 4 Buddy source and driver files were compiled after being copied verbatim from the separately supplied Buddy solution archive.

For reproducibility, the exact raw program outputs are stored under `execution_outputs/`.
