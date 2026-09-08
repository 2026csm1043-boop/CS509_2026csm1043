# Assignment 01

## Assignment Mode

This assignment contains both the Individual and Double / Buddy submissions supplied for CS509.

## Objective

- **Individual:** implement matrix multiplication using simple GEMM and blocked GEMM, while retaining the supplied CSR conversion support.
- **Buddy:** implement BFS, DFS, and positive-weight single-source shortest path (SSSP) using the supplied CSR graph representation.

## Language and Environment

C++17, `g++ 14.2.0`, compiled with `-std=c++17 -O2`.

## Individual

### Algorithm / Approach

**GEMM Simple:** triple-loop matrix multiplication with `i-k-j` loop ordering.

**GEMM Blocking:** tiled matrix multiplication using the supplied block size.

**CSR Conversion:** adjacency-list input is converted to CSR containing row pointers, column indices, and values.

### Input Format

GEMM:
```text
M K N
M rows of A with K integers each
K rows of B with N integers each
```

Graph/CSR:
```text
V E
u degree v weight ...
...
```

### Files

- `individual/src/gemm.cpp`, `gemm.h` — GEMM implementations.
- `individual/src/csr.cpp`, `csr.h` — CSR conversion.
- `individual/driver/main.cpp` — dedicated driver.
- `individual/tests/` — supplied test inputs.

### Compilation

```bash
g++ -std=c++17 -O2 assignment_01/individual/src/gemm.cpp assignment_01/individual/src/csr.cpp assignment_01/individual/driver/main.cpp -Iassignment_01/individual/src -o assignment_01_ind.exe
```

### Execution

```bash
./assignment_01_ind.exe gemm-simple assignment_01/individual/tests/gemm_test_01.txt
./assignment_01_ind.exe gemm-block assignment_01/individual/tests/gemm_test_01.txt 2
./assignment_01_ind.exe csr assignment_01/individual/tests/graph_10.txt
```

### Result Table

| Mode | Test File | Input Type | Input Size | Expected Output | Actual Output | Algorithm Time |
| --- | --- | --- | --- | --- | --- | --- |
| Individual | `gemm_test_01.txt` | Matrix / GEMM | M=2, K=3, N=2 | `58 64 / 139 154` | `58 64 / 139 154` | 0.000591 ms (simple); 0.00058 ms (blocking) |
| Individual | `gemm_test_02.txt` | Matrix / GEMM | M=1, K=1, N=1 | `-42` | `-42` | 0.001672 ms (simple); 0.001702 ms (blocking) |
| Individual | `gemm_test_03.txt` | Matrix / GEMM | M=2, K=2, N=2 | `4 -1 / -2 17` | `4 -1 / -2 17` | 0.000571 ms (simple); 0.000581 ms (blocking) |
| Individual | `gemm_test_04.txt` | Matrix / GEMM | M=3, K=4, N=2 | `13 2 / -13 12 / 6 9` | `13 2 / -13 12 / 6 9` | 0.000661 ms (simple); 0.000671 ms (blocking) |
| Individual | `gemm_test_05.txt` | Matrix / GEMM | M=4, K=3, N=5 | `9 -16 -5 11 2 / 7 7 -4 10 -4 / -8 -1 7 -7 18 / 8 9 -3 14 5` | `9 -16 -5 11 2 / 7 7 -4 10 -4 / -8 -1 7 -7 18 / 8 9 -3 14 5` | 0.000751 ms (simple); 0.000841 ms (blocking) |
| Individual | `graph_10.txt` | Adjacency list → CSR | V=10, E=11 (22 stored adjacency entries) | CSR conversion completes | V=10, stored edges=22; row_ptr 0..22 | Not reported by supplied driver |


The CSR command in the supplied driver does not report a timing value; its conversion output is preserved exactly in the raw execution log.

## Buddy

### Algorithms

- **BFS:** queue-based breadth-first traversal and unweighted source distances.
- **DFS:** iterative depth-first traversal using a stack.
- **SSSP:** priority-queue shortest paths for the positive-weight graph.

CSR conversion is performed before timing in the supplied Buddy driver.

### Compilation

```bash
g++ -std=c++17 -O2 assignment_01/buddy/src/graph.cpp assignment_01/buddy/src/bfs.cpp assignment_01/buddy/src/dfs.cpp assignment_01/buddy/src/sssp.cpp assignment_01/buddy/driver/main.cpp -Iassignment_01/buddy/src -o assignment_01_buddy.exe
```

### Execution

```bash
./assignment_01_buddy.exe BFS assignment_01/buddy/tests/bfs_10.txt
./assignment_01_buddy.exe DFS assignment_01/buddy/tests/dfs_100.txt
./assignment_01_buddy.exe SSSP assignment_01/buddy/tests/sssp_10000.txt
```

### Test Cases and Results

| Mode | Test File | Input Type | Input Size | Expected Output | Actual Output | Algorithm Time |
| --- | --- | --- | --- | --- | --- | --- |
| Buddy | `bfs_10.txt` | Undirected graph | V=10, E=11 | All reachable vertices visited from source 0; distances computed | All 10 vertices reached; max distance=5 | 0.005448 ms |
| Buddy | `bfs_100.txt` | Undirected graph | V=100, E=200 | All reachable vertices visited from source 0; distances computed | All 100 vertices reached; max distance=25 | 0.114651 ms |
| Buddy | `bfs_10000.txt` | Undirected graph | V=10000, E=20000 | All reachable vertices visited from source 0; distances computed | All 10000 vertices reached; max distance=2500 | 1.808279 ms |
| Buddy | `bfs_50000.txt` | Undirected graph | V=50000, E=100000 | All reachable vertices visited from source 0; distances computed | All 50000 vertices reached; max distance=12500 | 5.569288 ms |
| Buddy | `bfs_100000.txt` | Undirected graph | V=100000, E=200000 | All reachable vertices visited from source 0; distances computed | All 100000 vertices reached; max distance=25000 | 9.247913 ms |
| Buddy | `dfs_10.txt` | Undirected graph | V=10, E=11 | All 10 vertices visited from source 0 | Traversal visits 10/10 vertices; starts ['0', '1', '3']; ends ['5', '7', '9'] | 0.005687 ms |
| Buddy | `dfs_100.txt` | Undirected graph | V=100, E=200 | All 100 vertices visited from source 0 | Traversal visits 100/100 vertices; starts ['0', '1', '2']; ends ['97', '98', '99'] | 0.172074 ms |
| Buddy | `dfs_10000.txt` | Undirected graph | V=10000, E=20000 | All 10000 vertices visited from source 0 | Traversal visits 10000/10000 vertices; starts ['0', '1', '2']; ends ['9997', '9998', '9999'] | 1.167280 ms |
| Buddy | `dfs_50000.txt` | Undirected graph | V=50000, E=100000 | All 50000 vertices visited from source 0 | Traversal visits 50000/50000 vertices; starts ['0', '1', '2']; ends ['49997', '49998', '49999'] | 6.149944 ms |
| Buddy | `dfs_100000.txt` | Undirected graph | V=100000, E=200000 | All 100000 vertices visited from source 0 | Traversal visits 100000/100000 vertices; starts ['0', '1', '2']; ends ['99997', '99998', '99999'] | 10.442679 ms |
| Buddy | `sssp_10.txt` | Undirected graph | V=10, E=11 | Shortest-path distances from source 0 | All 10 vertices reached; max distance=12; d(0,9)=11 | 0.011787 ms |
| Buddy | `sssp_100.txt` | Undirected graph | V=100, E=200 | Shortest-path distances from source 0 | All 100 vertices reached; max distance=112; d(0,99)=1 | 0.113001 ms |
| Buddy | `sssp_10000.txt` | Undirected graph | V=10000, E=20000 | Shortest-path distances from source 0 | All 10000 vertices reached; max distance=11112; d(0,9999)=1 | 1.005497 ms |
| Buddy | `sssp_50000.txt` | Undirected graph | V=50000, E=100000 | Shortest-path distances from source 0 | All 50000 vertices reached; max distance=55555; d(0,49999)=5 | 6.260561 ms |
| Buddy | `sssp_100000.txt` | Undirected graph | V=100000, E=200000 | Shortest-path distances from source 0 | All 100000 vertices reached; max distance=111112; d(0,99999)=1 | 9.616972 ms |


## Complexity

- GEMM: `O(MKN)` time, `O(MN)` result space.
- BFS: `O(V+E)` time and `O(V)` auxiliary space.
- DFS: `O(V+E)` time and `O(V)` auxiliary space, excluding the CSR storage.
- Positive-weight SSSP with a binary heap: `O((V+E) log V)` in the usual sparse-graph analysis.
- CSR conversion: `O(V+E)` preprocessing.

## Raw Outputs

The complete program outputs are stored in `../execution_outputs/` at repository level, with one file per test/algorithm run.

## References

- CS509 Lab Work Guidelines supplied with the submission.
- Supplied assignment source and test files.
