# CS509 Lab Test 1 - Integrated Solution

This implementation extends the data structures and algorithms used in the previous assignments:
- Assignment 1 CSR/BFS/SSSP
- Assignment 2 Floyd-Warshall/TC/BC/CC

## Build

```bash
make
```

or

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic src/csr.cpp src/algorithms.cpp driver.cpp -Isrc -o cs509_labtest1
```

## Task 1

```bash
./cs509_labtest1 csr tests/task1_directed.txt
./cs509_labtest1 csr tests/task1_undirected.txt
```

## Task 2

Run a single block size:

```bash
./cs509_labtest1 fw tests/task2.txt 16
```

Run all required block sizes:

```bash
./cs509_labtest1 fw-bench tests/task2.txt
```

Recommended block sizes: 8, 16, 32, 64, 128.

Run the benchmark on at least two different input graphs with different V values.

## Task 3

For an undirected graph, choose any valid source vertex:

```bash
./cs509_labtest1 validate tests/task3.txt 0
```

The program reports:
- CC <-> BFS
- BFS <-> SSSP
- TC <-> CC
- BC <-> CC
- Overall PASS/FAIL
- runtime for every validation and total runtime

## Submission

From the directory containing `CS509_LabTest1`:

```bash
tar -czvf 2026CSM1043_FirstName.tgz CS509_LabTest1/
```

Replace `FirstName` with the required first name before submission.
