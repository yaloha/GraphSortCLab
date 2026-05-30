#include <stdio.h>
#include <string.h>
#include "src/graph.h"
#include "src/fl_wrsh.h"
#include <stdlib.h>

void print_help(const char* prog_name) {
    printf("Usage: %s [OPTIONS]\n", prog_name);
    printf("Options:\n");
    printf("  --input <file>      (required unless --benchmark; default: ../input.txt)\n");
    printf("  --output <file>     (default: ../result.txt)\n");
    printf("  --save-graph <file> Save loaded graph to this file (optional)\n");
    printf("  --benchmark         Run benchmarks\n");
    printf("  --test         Run benchmarks\n");
    printf("  --dont-clear        Append to output file\n");
    printf("  --help              Show this help\n");
}

void run_benchmark() {
    printf("\nrunning benchmarks...\n");
    int result = system("../build/benchmark_run");
    if (result != 0) {
        printf("benchmark execution failed!\n");
    }
}

void run_tests() {
    printf("\nrunning tests...\n");
    int result = system("../build/tests");
    if (result != 0) {
        printf("test execution failed!\n");
    }
}

int main(int argc, char* argv[]) {
    char* input_file = "../input.txt";
    char* output_file = "../result.txt";
    char* save_graph_file = "../result.txt";
    int benchmark_mode = 0, dont_clear = 0, test_mode = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--input") == 0 && i + 1 < argc) {
            input_file = argv[++i];
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "--save-graph") == 0 && i + 1 < argc) {
            save_graph_file = argv[++i];
        } else if (strcmp(argv[i], "--benchmark") == 0) {
            benchmark_mode = 1;
        } else if (strcmp(argv[i], "--test") == 0) {
            test_mode = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--dont-clear") == 0) {
            dont_clear = 1;
        } else {
            printf("unknown option: %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }

    if (benchmark_mode) {
        run_benchmark();
        if (test_mode != 1)
            return 0;
    }

    if (test_mode) {
        run_tests();
        return 0;
    }

    if (input_file == NULL) {
        printf("error: --input is required (or --benchmark, or --help)\n");
        print_help(argv[0]);
        return 1;
    }

    if (dont_clear != 1){
        FILE* f = fopen(output_file, "w");
        if (f != NULL) {
            fclose(f);
        }
    }

    GraphMatrix* gm = load_graph_from_file(input_file);
    if (gm == NULL) {
        printf("failed to load graph from %s\n", input_file);
        return 1;
    }
    printf("graph loaded: %d vertices\n", gm->size);

    if (save_graph_file != NULL) {
        save_graph_to_file(gm, save_graph_file);
        printf("graph saved to %s\n", save_graph_file);
    }

    int** dist = floyd_warshall(gm);
    if (dist == NULL) {
        printf("floyd–warshall failed\n");
        free_gm(&gm);
        return 1;
    }

    save_fl_wrsh_to_file(dist, gm->size, output_file);
    printf("shortest distances saved to %s file\n", output_file);

    free_dist_matrix(dist, gm->size);
    free_gm(&gm);

    return 0;
}