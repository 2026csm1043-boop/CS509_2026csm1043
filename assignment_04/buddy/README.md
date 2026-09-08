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
