# Assignment 03 — Individual

## Assignment Mode

Individual submission — Sejal Gupta (2026CSM1043).

## Objective

Compute a minimum spanning tree for the supplied weighted undirected graphs using Kruskal's and Prim's algorithms.

## Algorithm / Approach

- **Kruskal:** processes edges by weight and uses a disjoint-set structure to avoid cycles.
- **Prim:** grows an MST from the source-side tree using a priority queue.
- The supplied driver converts the adjacency list to CSR before timing.

## Input Format

```text
V E
u degree v weight ...
...
```

The driver validates reverse edges and matching weights for the undirected graph.

## Compilation

```bash
g++ -std=c++17 -O2 assignment_03/individual/src/algorithms.cpp assignment_03/individual/src/csr.cpp assignment_03/individual/driver/main.cpp -Iassignment_03/individual/src -o assignment_03_ind.exe
```

## Execution

```bash
./assignment_03_ind.exe mst-k assignment_03/individual/tests/mst_10.txt
./assignment_03_ind.exe mst-p assignment_03/individual/tests/mst_10.txt
```

Use `mst-both` to run both algorithms for one input.

## Test Cases and Result Table

| Mode | Test File | Input Type | Input Size | Expected Output | Actual Output | Algorithm Time |
| --- | --- | --- | --- | --- | --- | --- |
| Individual | `mst_10.txt` | Weighted undirected graph | V=10, E=17 | Minimum spanning tree; same total weight for both methods | Kruskal weight=42; Prim weight=42 | Kruskal 0.315 ms; Prim 0.123 ms |
| Individual | `mst_100.txt` | Weighted undirected graph | V=100, E=197 | Minimum spanning tree; same total weight for both methods | Kruskal weight=854; Prim weight=854 | Kruskal 1.080 ms; Prim 0.329 ms |
| Individual | `mst_10000.txt` | Weighted undirected graph | V=10000, E=19997 | Minimum spanning tree; same total weight for both methods | Kruskal weight=88339; Prim weight=88339 | Kruskal 7.881 ms; Prim 6.698 ms |
| Individual | `mst_50000.txt` | Weighted undirected graph | V=50000, E=99997 | Minimum spanning tree; same total weight for both methods | Kruskal weight=441850; Prim weight=441850 | Kruskal 41.189 ms; Prim 32.216 ms |
| Individual | `mst_100000.txt` | Weighted undirected graph | V=100000, E=199997 | Minimum spanning tree; same total weight for both methods | Kruskal weight=883684; Prim weight=883684 | Kruskal 85.411 ms; Prim 71.345 ms |


## Complexity

- Kruskal: `O(E log E)` time.
- Prim with the supplied priority-queue implementation: `O(E log V)` for sparse graphs.
- CSR conversion: `O(V+E)` preprocessing.

## Raw Outputs

Full stdout for each run is retained in `execution_outputs/`.

## References

- CS509 Lab Work Guidelines.
- Supplied Assignment 3 Individual source and tests.


---

# Assignment 03 — Buddy

## Assignment Mode

Double / Buddy submission.

### Members

- Harshita Mahor — 2026CSM1016
- Sejal Gupta — 2026CSM1043

## Objective

Implement and execute:

1. Gradient Descent
2. Maxflow-Mincut

The supplied Buddy report format was used as the model for the PDF report.

## Algorithms / Approach

### Gradient Descent

The supplied implementation minimizes a generic one-variable polynomial using the derivative and learning-rate update. It stops when the derivative magnitude is within tolerance or the maximum iteration count is reached.

### Maxflow-Mincut

The graph is converted from adjacency-list form to CSR before the timer starts. Dinic's method is used for maximum flow. The final residual graph identifies the source side of the minimum cut.

## Input Formats

Gradient Descent:
```text
DEGREE d
COEFFICIENTS c0 c1 ... cd
INITIAL_X x0
LEARNING_RATE alpha
TOLERANCE epsilon
MAX_ITERATIONS n
```

Maxflow-Mincut:
```text
V E
u degree v capacity ...
...
SOURCE s
SINK t
```

## Compilation

```bash
g++ -std=c++17 -O2 assignment_03/buddy/src/gradient_descent.cpp assignment_03/buddy/src/csr.cpp assignment_03/buddy/src/maxflow.cpp assignment_03/buddy/driver/assignment3_driver.cpp -Iassignment_03/buddy/src -o assignment_03_buddy.exe
```

## Execution

```bash
./assignment_03_buddy.exe gd assignment_03/buddy/tests/gd_01.txt
./assignment_03_buddy.exe maxflow assignment_03/buddy/tests/maxflow_10.txt
```

## CSR / Timing

For Maxflow-Mincut, CSR conversion is preprocessing and is outside the reported algorithm time. The timer surrounds the supplied algorithm call.

## Test Cases and Result Table

| Mode | Test File | Input Type | Input Size | Expected Output | Actual Output | Algorithm Time |
| --- | --- | --- | --- | --- | --- | --- |
| Buddy | `gd_01.txt` | Polynomial | Degree=2 | x*=3 | x=2.9999995063; f(x)=0.0000000000; iterations=70; converged=true | 0.000959 ms |
| Buddy | `gd_02.txt` | Polynomial | Degree=4 | x*=0 | x=0.0000002379; f(x)=0.0000000000; iterations=180; converged=true | 0.002510 ms |
| Buddy | `gd_03.txt` | Polynomial | Degree=6 | x*=0 | x=0.0000004835; f(x)=0.0000000000; iterations=349; converged=true | 0.347575 ms |
| Buddy | `gd_04.txt` | Polynomial | Degree=8 | x*=0 | x=0.0000000050; f(x)=0.0000000000; iterations=948; converged=true | 0.020334 ms |
| Buddy | `gd_05.txt` | Polynomial | Degree=10 | x*=0 | x=0.0000000000; f(x)=0.0000000000; iterations=2364; converged=true | 0.313698 ms |
| Buddy | `maxflow_10.txt` | Directed graph → CSR | V=10, E=10 | Maximum flow = minimum cut capacity | Max flow=23; min-cut capacity=23 | 0.219 ms |
| Buddy | `maxflow_100.txt` | Directed graph → CSR | V=100, E=196 | Maximum flow = minimum cut capacity | Max flow=20; min-cut capacity=20 | 0.559 ms |
| Buddy | `maxflow_1000.txt` | Directed graph → CSR | V=1000, E=1996 | Maximum flow = minimum cut capacity | Max flow=20; min-cut capacity=20 | 1.355 ms |
| Buddy | `maxflow_10000.txt` | Directed graph → CSR | V=10000, E=19996 | Maximum flow = minimum cut capacity | Max flow=20; min-cut capacity=20 | 21.071 ms |
| Buddy | `maxflow_50000.txt` | Directed graph → CSR | V=50000, E=99996 | Maximum flow = minimum cut capacity | Max flow=20; min-cut capacity=20 | 79.612 ms |


## Complexity

- Gradient Descent: `O(dI)` for polynomial degree `d` and `I` iterations.
- Maxflow using Dinic: general worst-case `O(V^2E)`; practical runtime depends on the residual network.
- CSR conversion: `O(V+E)` preprocessing.

## Observations

All completed Gradient Descent tests converged. Every completed Maxflow-Mincut test satisfied Maximum Flow = Minimum Cut Capacity.

## Raw Outputs

Full stdout is stored in `execution_outputs/`.

## References

- CS509 Lab Work Guidelines.
- Supplied Assignment 3 Buddy source/tests.
- Supplied Assignment 3 Buddy report template.
