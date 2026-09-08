# Assignment 02

## Assignment Mode

Individual submission.

## Objective

Implement shortest-path algorithms for weighted graphs: Bellman-Ford and Floyd-Warshall.

## Algorithms / Approach

### Bellman-Ford

The supplied implementation stores the weighted directed graph in CSR and performs repeated edge relaxation, stopping early when no distance changes. A final pass checks for a reachable negative cycle.

### Floyd-Warshall

The supplied implementation updates an all-pairs distance matrix using the standard `k-i-j` dynamic-programming recurrence and then checks the diagonal for a negative cycle.

## Input Formats

Bellman-Ford:
```text
V E
u degree v weight ...
...
SOURCE s
```

Floyd-Warshall:
```text
V
V x V distance matrix
```

`INF` represents an unreachable pair.

## Files

- `individual/src/algorithms.cpp`, `algorithms.h` — Bellman-Ford and Floyd-Warshall.
- `individual/src/csr.cpp`, `csr.h` — CSR conversion for Bellman-Ford.
- `individual/driver/main.cpp` — dedicated driver.
- `individual/tests/` — all supplied test cases.

## Compilation

```bash
g++ -std=c++17 -O2 assignment_02/individual/src/algorithms.cpp assignment_02/individual/src/csr.cpp assignment_02/individual/driver/main.cpp -Iassignment_02/individual/src -o assignment_02_ind.exe
```

## Execution

```bash
./assignment_02_ind.exe bf assignment_02/individual/tests/bf_10.txt
./assignment_02_ind.exe fw assignment_02/individual/tests/fw_10.txt
```

CSR conversion for Bellman-Ford and matrix loading for Floyd-Warshall are outside the timed algorithm call in the supplied driver.

## Test Cases and Result Table

| Mode | Test File | Input Type | Input Size | Expected Output | Actual Output | Algorithm Time |
| --- | --- | --- | --- | --- | --- | --- |
| Individual | `bf_10.txt` | Directed weighted graph | V=10, E=20 | Negative-cycle status for V=10, E=20 | Negative cycle: true | 0.002 ms |
| Individual | `bf_100.txt` | Directed weighted graph | V=100, E=250 | Negative-cycle status for V=100, E=250 | Negative cycle: true | 0.075 ms |
| Individual | `bf_10000.txt` | Directed weighted graph | V=10000, E=30000 | Negative-cycle status for V=10000, E=30000 | Negative cycle: true | 3356.293 ms |
| Individual | `bf_50000.txt` | Directed weighted graph | V=50000, E=150000 | Bellman-Ford result / negative-cycle status | No program output; bounded verification run timed out after 45 s | >45 s (timeout; no algorithm timing reported) |
| Individual | `bf_100000.txt` | Directed weighted graph | V=100000, E=300000 | Bellman-Ford result / negative-cycle status | No program output; bounded verification run timed out after 45 s | >45 s (timeout; no algorithm timing reported) |
| Individual | `fw_10.txt` | Distance matrix | V=10 | All-pairs shortest-path matrix; no negative cycle | Negative cycle: none; d(0,9)=18 | 0.004 ms |
| Individual | `fw_100.txt` | Distance matrix | V=100 | All-pairs shortest-path matrix; no negative cycle | Negative cycle: none; d(0,99)=40 | 1.598 ms |
| Individual | `fw_500.txt` | Distance matrix | V=500 | All-pairs shortest-path matrix; no negative cycle | Negative cycle: none; d(0,499)=45 | 274.918 ms |
| Individual | `fw_1000.txt` | Distance matrix | V=1000 | All-pairs shortest-path matrix; no negative cycle | Negative cycle: none; d(0,999)=48 | 1908.062 ms |
| Individual | `fw_2000.txt` | Distance matrix | V=2000 | All-pairs shortest-path matrix; no negative cycle | Negative cycle: none; d(0,1999)=54 | 18691.242 ms |


### Large Bellman-Ford Verification Note

The supplied Bellman-Ford implementation is the standard repeated-relaxation implementation. The `bf_50000.txt` and `bf_100000.txt` runs did not complete within the bounded 45-second verification window on the verification machine. No timing value or result was invented for these cases. The raw status files record the timeout.

## Complexity

- Bellman-Ford: `O(VE)` worst-case time and `O(V)` algorithmic auxiliary distance space, in addition to CSR storage.
- Floyd-Warshall: `O(V^3)` time and `O(V^2)` space.

## Raw Outputs

Complete stdout files are in the repository-level `execution_outputs/` directory.

## References

- CS509 Lab Work Guidelines supplied with the submission.
- Supplied Assignment 2 source and test files.


---

# Assignment 02 — Buddy

## Assignment Mode

Double / Buddy submission.

### Buddy Members

- Harshita Mahor — 2026CSM1016
- Sejal Gupta — 2026CSM1043

## Objective

Implement Triangle Counting, Betweenness Centrality, and Connected Components for the supplied undirected unweighted graphs.

## Algorithms / Approach

- **Triangle Counting:** intersection-based counting on sorted CSR adjacency lists.
- **Betweenness Centrality:** Brandes' algorithm for unweighted undirected graphs.
- **Connected Components:** traversal over CSR to assign component IDs.

The supplied driver sorts adjacency lists and converts them to CSR before the timed algorithm call.

## Input Format

```text
V E
u degree neighbor1 neighbor2 ...
...
```

The supplied driver validates the undirected representation.

## Compilation

```bash
g++ -std=c++17 -O2 assignment_02/buddy/src/algorithms.cpp assignment_02/buddy/src/csr.cpp assignment_02/buddy/driver/main.cpp -Iassignment_02/buddy/src -o assignment_02_buddy.exe
```

## Execution

```bash
./assignment_02_buddy.exe tc assignment_02/buddy/tests/tc_10.txt
./assignment_02_buddy.exe bc assignment_02/buddy/tests/bc_100.txt
./assignment_02_buddy.exe cc assignment_02/buddy/tests/cc_10000.txt
```

## Test Cases and Result Table

| Mode | Test File | Input Type | Input Size | Expected Output | Actual Output | Algorithm Time |
| --- | --- | --- | --- | --- | --- | --- |
| Buddy | `tc_10.txt` | Undirected unweighted graph | V=10, E=15 | Triangle count for the supplied graph | Total triangles = 2 | 0.010 ms |
| Buddy | `tc_100.txt` | Undirected unweighted graph | V=100, E=250 | Triangle count for the supplied graph | Total triangles = 19 | 0.072 ms |
| Buddy | `tc_10000.txt` | Undirected unweighted graph | V=10000, E=30000 | Triangle count for the supplied graph | Total triangles = 34 | 4.730 ms |
| Buddy | `tc_50000.txt` | Undirected unweighted graph | V=50000, E=150000 | Triangle count for the supplied graph | Total triangles = 19 | 43.870 ms |
| Buddy | `tc_100000.txt` | Undirected unweighted graph | V=100000, E=300000 | Triangle count for the supplied graph | Total triangles = 27 | 32.397 ms |
| Buddy | `bc_10.txt` | Undirected unweighted graph | V=10, E=15 | Betweenness-centrality values for all vertices | Centrality vector produced; maximum at vertex 6 = 11.33 | 0.132 ms |
| Buddy | `bc_100.txt` | Undirected unweighted graph | V=100, E=250 | Betweenness-centrality values for all vertices | Centrality vector produced; maximum at vertex 97 = 427.62 | 1.555 ms |
| Buddy | `bc_1000.txt` | Undirected unweighted graph | V=1000, E=3000 | Betweenness-centrality values for all vertices | Centrality vector produced; maximum at vertex 495 = 8866.50 | 237.943 ms |
| Buddy | `bc_5000.txt` | Undirected unweighted graph | V=5000, E=15000 | Betweenness-centrality values for all vertices | Centrality vector produced; maximum at vertex 4416 = 61821.59 | 5984.766 ms |
| Buddy | `bc_10000.txt` | Undirected unweighted graph | V=10000, E=30000 | Betweenness-centrality values for all vertices | Centrality vector produced; maximum at vertex 8798 = 186829.04 | 23981.781 ms |
| Buddy | `cc_10.txt` | Undirected unweighted graph | V=10, E=15 | Connected-component count | Number of components = 1 | 0.035 ms |
| Buddy | `cc_100.txt` | Undirected unweighted graph | V=100, E=250 | Connected-component count | Number of components = 1 | 0.014 ms |
| Buddy | `cc_10000.txt` | Undirected unweighted graph | V=10000, E=30000 | Connected-component count | Number of components = 1 | 0.988 ms |
| Buddy | `cc_50000.txt` | Undirected unweighted graph | V=50000, E=150000 | Connected-component count | Number of components = 1 | 4.174 ms |
| Buddy | `cc_100000.txt` | Undirected unweighted graph | V=100000, E=300000 | Connected-component count | Number of components = 1 | 7.428 ms |


For the two smallest triangle-counting tests, the supplied driver also prints the individual triangles. The complete stdout is retained in `execution_outputs/`.

## Complexity

- Triangle Counting: sparse adjacency-intersection work; runtime depends on the degree distribution.
- Betweenness Centrality: `O(VE)` for unweighted graphs using Brandes' algorithm.
- Connected Components: `O(V+E)` time.
- CSR conversion and sorting: preprocessing outside the algorithm timer.

## References

- CS509 Lab Work Guidelines.
- Supplied Assignment 2 Buddy source and tests.
