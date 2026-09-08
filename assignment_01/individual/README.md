# Assignment 01 — Individual

See `../../README.md` and `../README.md` for complete repository and Assignment 1 documentation.

## Test Cases

The Individual GEMM task now contains five test files. The four added tests follow the supplied Assignment 1 question and exercise a 1×1 case, negative values, rectangular matrices, and dimensions that are not multiples of the blocking size.

Blocking runs below use block size `2`.

| Test File | Dimensions (M×K, K×N) | Expected Result | Simple Time | Blocking Time |
| --- | --- | --- | ---: | ---: |
| `gemm_test_01.txt` | 2×3, 3×2 | `58 64 / 139 154` | 0.000591 ms | 0.00058 ms |
| `gemm_test_02.txt` | 1×1, 1×1 | `-42` | 0.001672 ms | 0.001702 ms |
| `gemm_test_03.txt` | 2×2, 2×2 | `4 -1 / -2 17` | 0.000571 ms | 0.000581 ms |
| `gemm_test_04.txt` | 3×4, 4×2 | `13 2 / -13 12 / 6 9` | 0.000661 ms | 0.000671 ms |
| `gemm_test_05.txt` | 4×3, 3×5 | `9 -16 -5 11 2 / 7 7 -4 10 -4 / -8 -1 7 -7 18 / 8 9 -3 14 5` | 0.000751 ms | 0.000841 ms |

## Compilation

```bash
g++ -std=c++17 -O2 src/gemm.cpp src/csr.cpp driver/main.cpp -Isrc -o assignment_01_ind.exe
```

## Execution

```bash
./assignment_01_ind.exe gemm-simple tests/gemm_test_01.txt
./assignment_01_ind.exe gemm-block tests/gemm_test_01.txt 2
./assignment_01_ind.exe csr tests/graph_10.txt
```

The supplied GEMM driver times only the GEMM call. The supplied CSR command does not print an execution-time value, so no CSR timing is fabricated.
