#include <benchmark/benchmark.h>
#include <vector>
#include <cstdlib>

extern "C" {
#include "graph.h"
#include "fl_wrsh.h"
}

static void BM_FloydWarshall(benchmark::State& state) {
    int n = state.range(0);
    GraphMatrix* g = init_gm(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                add_edge(g, i, j, (rand() % 100) + 1);
            }
        }
    }

    for (auto _ : state) {
        int** dist = floyd_warshall(g);
        free_dist_matrix(dist, n);
    }

    free_gm(&g);
}

BENCHMARK(BM_FloydWarshall)->RangeMultiplier(2)->Range(4, 512);

BENCHMARK_MAIN();