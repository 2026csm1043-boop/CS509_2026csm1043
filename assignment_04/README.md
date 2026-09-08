# Assignment 04 — Individual

## Assignment Mode

Individual submission — Sejal Gupta (2026CSM1043).

## Objective

Implement Greedy Vertex Coloring and PageRank for the supplied graph inputs.

## Algorithms / Approach

### Greedy Vertex Coloring

The supplied implementation orders vertices using a Welsh-Powell degree ordering, assigns the lowest available color, and verifies the resulting coloring.

### PageRank

The supplied implementation initializes ranks uniformly, distributes rank using the damping factor, handles dangling vertices uniformly, and iterates until the rank change is within tolerance or the maximum iteration count is reached.

CSR conversion is preprocessing for the graph inputs and is outside the timed algorithm call in the supplied driver.

## Input Formats

Vertex Coloring:
```text
V E
u degree neighbor1 neighbor2 ...
...
```

PageRank:
```text
V E
u outDegree v1 v2 ...
...
DAMPING d
TOLERANCE epsilon
MAX_ITERATIONS n
```

## Compilation

```bash
g++ -std=c++17 -O2 assignment_04/individual/src/csr.cpp assignment_04/individual/src/vertex_coloring.cpp assignment_04/individual/src/pagerank.cpp assignment_04/individual/driver/main.cpp -Iassignment_04/individual/src -o assignment_04_ind.exe
```

## Execution

```bash
./assignment_04_ind.exe color assignment_04/individual/tests/color_10.txt
./assignment_04_ind.exe pagerank assignment_04/individual/tests/pagerank_1000.txt
```

## Test Cases and Result Table

| Mode | Test File | Input Type | Input Size | Expected Output | Actual Output | Algorithm Time |
| --- | --- | --- | --- | --- | --- | --- |
| Individual | `color_example.txt` | Undirected unweighted graph | V=5, E=6 | Valid greedy coloring | Colors used=3; valid=true | 0.001827 ms |
| Individual | `color_10.txt` | Undirected unweighted graph | V=10, E=15 | Valid greedy coloring | Colors used=3; valid=true | 0.002784 ms |
| Individual | `color_100.txt` | Undirected unweighted graph | V=100, E=150 | Valid greedy coloring | Colors used=3; valid=true | 0.013308 ms |
| Individual | `color_10000.txt` | Undirected unweighted graph | V=10000, E=15000 | Valid greedy coloring | Colors used=4; valid=true | 2.396537 ms |
| Individual | `color_50000.txt` | Undirected unweighted graph | V=50000, E=75000 | Valid greedy coloring | Colors used=4; valid=true | 20.579124 ms |
| Individual | `color_100000.txt` | Undirected unweighted graph | V=100000, E=150000 | Valid greedy coloring | Colors used=4; valid=true | 23.059124 ms |
| Individual | `pagerank_example.txt` | Directed graph with damping | V=4, E=5 | Ranks sum to 1 and converge | Sum=1.000000; iterations=18; converged=true | 0.001755 ms |
| Individual | `pagerank_10.txt` | Directed graph with damping | V=10, E=30 | Ranks sum to 1 and converge | Sum=1.000000; iterations=12; converged=true | 0.091354 ms |
| Individual | `pagerank_100.txt` | Directed graph with damping | V=100, E=300 | Ranks sum to 1 and converge | Sum=1.000000; iterations=14; converged=true | 0.112256 ms |
| Individual | `pagerank_1000.txt` | Directed graph with damping | V=1000, E=3000 | Ranks sum to 1 and converge | Sum=1.000000; iterations=15; converged=true | 0.283307 ms |
| Individual | `pagerank_10000.txt` | Directed graph with damping | V=10000, E=30000 | Ranks sum to 1 and converge | Sum=1.000000; iterations=15; converged=true | 3.117244 ms |
| Individual | `pagerank_50000.txt` | Directed graph with damping | V=50000, E=150000 | Ranks sum to 1 and converge | Sum=1.000000; iterations=15; converged=true | 28.889191 ms |


## Complexity

- Greedy coloring: dominated by vertex ordering and adjacency checks; the supplied implementation uses CSR.
- PageRank: `O(I(V+E))` for non-dangling sparse graphs, with additional work for dangling vertices; `I` is the number of iterations.
- CSR conversion: `O(V+E)` preprocessing.

## Raw Outputs

Complete stdout is retained in `execution_outputs/`.

## References

- CS509 Lab Work Guidelines.
- Supplied Assignment 4 Individual source and tests.


---

# Assignment 04 — Buddy

## Assignment Mode

Double / Buddy submission.

### Members

- Harshita Mahor — 2026CSM1016
- Sejal Gupta — 2026CSM1043

## Objective

Implement K-Means Clustering and FastMap.

The Buddy code in this folder was **copied verbatim from the separately supplied Assignment 4 Buddy solution archive**. No algorithm source was rewritten.

## Algorithms / Approach

### K-Means

The supplied implementation initializes the first `K` input points as centroids, assigns each point to its nearest centroid, updates centroids using cluster means, preserves a previous centroid for an empty cluster, and stops when assignments or centroid movement satisfy the stopping condition.

### FastMap

The supplied implementation selects pivots using the farthest-object heuristic, projects objects with the law-of-cosines formula, and deflates the residual distance matrix after each target dimension.

## Input Formats

K-Means:
```text
N D K
N points, each containing D values
MAX_ITERATIONS n
TOLERANCE epsilon
```

FastMap:
```text
N targetDimensions
N x N distance matrix
```

## Files

- `buddy/src/kmeans.cpp`, `kmeans.h` — supplied K-Means implementation.
- `buddy/src/fastmap.cpp`, `fastmap.h` — supplied FastMap implementation.
- `buddy/driver/assignment4_buddy.cpp` — dedicated driver copied from the supplied solution.
- `buddy/tests/` — supplied eight Buddy test files.

## Compilation

```bash
g++ -std=c++17 -O2 assignment_04/buddy/src/kmeans.cpp assignment_04/buddy/src/fastmap.cpp assignment_04/buddy/driver/assignment4_buddy.cpp -Iassignment_04/buddy/src -o assignment_04_buddy.exe
```

## Execution

```bash
./assignment_04_buddy.exe kmeans assignment_04/buddy/tests/km_01.txt
./assignment_04_buddy.exe fastmap assignment_04/buddy/tests/fm_01.txt
```

The supplied driver times only the algorithm call; input reading and validation are outside the timed region.

## Test Cases and Result Table

| Mode | Test File | Input Type | Input Size | Expected Output | Actual Output | Algorithm Time |
| --- | --- | --- | --- | --- | --- | --- |
| Buddy | `km_01.txt` | Point set | N=6, D=2, K=3 | Converged K-Means clustering | WCSS=97.617500; iterations=2; converged=true | 0.724660 ms |
| Buddy | `fm_01.txt` | Distance matrix | N=5, target dimensions=2 | Coordinates in target dimensionality | Pivots=4 0; 2 1 | 0.097443 ms |
| Buddy | `km_02.txt` | Point set | N=10, D=2, K=5 | Converged K-Means clustering | WCSS=65.493333; iterations=2; converged=true | 0.124933 ms |
| Buddy | `fm_02.txt` | Distance matrix | N=6, target dimensions=2 | Coordinates in target dimensionality | Pivots=5 0; 3 1 | 0.054733 ms |
| Buddy | `km_03.txt` | Point set | N=20, D=5, K=8 | Converged K-Means clustering | WCSS=92.430000; iterations=3; converged=true | 0.215803 ms |
| Buddy | `fm_03.txt` | Distance matrix | N=8, target dimensions=3 | Coordinates in target dimensionality | Pivots=7 0; 0 0 | 0.042093 ms |
| Buddy | `km_04.txt` | Point set | N=30, D=5, K=10 | Converged K-Means clustering | WCSS=95.120000; iterations=13; converged=true | 0.170438 ms |
| Buddy | `fm_04.txt` | Distance matrix | N=10, target dimensions=3 | Coordinates in target dimensionality | Pivots=9 0; 0 0 | 0.040866 ms |


## Complexity

- K-Means: `O(I N K D)` time, where `I` is the number of iterations.
- FastMap: approximately `O(D N^2)` time because each target dimension deflates the full residual matrix.
- Space: both algorithms retain input-sized data; FastMap uses an additional residual distance matrix.

## Raw Outputs

Complete stdout is retained in `execution_outputs/`.

## References

- CS509 Lab Work Guidelines.
- Supplied Assignment 4 Buddy solution archive.
