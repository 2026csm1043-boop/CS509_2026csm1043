#include "vertex_coloring.h"

#include <algorithm>
#include <numeric>

ColoringResult greedyVertexColoring(const CSRGraph& graph) {
    const int n = graph.vertices;

    ColoringResult result;
    result.colors.assign(n, -1);
    result.colorsUsed = 0;
    result.valid = true;

    if (n == 0) {
        return result;
    }

    // Welsh-Powell: vertices in non-increasing degree order.
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);

    std::sort(order.begin(), order.end(),
              [&](int a, int b) {
                  int degreeA = graph.row_ptr[a + 1] - graph.row_ptr[a];
                  int degreeB = graph.row_ptr[b + 1] - graph.row_ptr[b];

                  if (degreeA != degreeB)
                      return degreeA > degreeB;

                  // Deterministic tie-breaking.
                  return a < b;
              });

    std::vector<int> used(n, -1);
    int stamp = 0;

    for (int v : order) {
        ++stamp;

        for (int p = graph.row_ptr[v]; p < graph.row_ptr[v + 1]; ++p) {
            int u = graph.col_idx[p];

            if (u >= 0 && u < n && result.colors[u] != -1) {
                used[result.colors[u]] = stamp;
            }
        }

        int color = 0;
        while (color < n && used[color] == stamp) {
            ++color;
        }

        result.colors[v] = color;
        result.colorsUsed = std::max(result.colorsUsed, color + 1);
    }

    // Verify the coloring.
    for (int v = 0; v < n && result.valid; ++v) {
        for (int p = graph.row_ptr[v]; p < graph.row_ptr[v + 1]; ++p) {
            int u = graph.col_idx[p];

            if (u < 0 || u >= n || u == v ||
                result.colors[v] == result.colors[u]) {
                result.valid = false;
                break;
            }
        }
    }

    return result;
}
