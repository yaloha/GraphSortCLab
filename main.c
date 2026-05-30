#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/graph.h"
#include "src/fl_wrsh.c"

void print_help(const char* prog_name) {
    printf("Usage: %s [OPTIONS]\n", prog_name);
    printf("Options:\n");
    printf("  --input <file>     Input file with graph matrix (required unless --benchmark)\n");
    printf("  --output <file>    Output file for Floyd result (default: result.txt)\n");
    printf("  --save-graph <file> Save loaded graph to this file (optional)\n");
    printf("  --benchmark        Run benchmarks instead of processing a single graph\n");
    printf("  --help             Show this help\n");
}

int main(int argc, char* argv[]) {
    char* input_file = "input.txt";
    char* output_file = "result.txt";
    char* save_graph_file = "result.txt";
    int benchmark_mode = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--input") == 0 && i + 1 < argc) {
            input_file = argv[++i];
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "--save-graph") == 0 && i + 1 < argc) {
            save_graph_file = argv[++i];
        } else if (strcmp(argv[i], "--benchmark") == 0) {
            benchmark_mode = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        } else {
            printf("Unknown option: %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }

    if (benchmark_mode) {
        printf("Running benchmarks...\n");
        return 0;
    }

    if (input_file == NULL) {
        printf("Error: --input is required (or --benchmark, or --help)\n");
        print_help(argv[0]);
        return 1;
    }

    GraphMatrix* gm = load_graph_from_file(input_file);
    if (gm == NULL) {
        printf("Failed to load graph from %s\n", input_file);
        return 1;
    }
    printf("Graph loaded: %d vertices\n", gm->size);

    if (save_graph_file != NULL) {
        save_graph_to_file(gm, save_graph_file);
        printf("Graph saved to %s\n", save_graph_file);
    }

    int** dist = floyd_warshall(gm);
    if (dist == NULL) {
        printf("Floyd–Warshall failed\n");
        free_gm(&gm);
        return 1;
    }

    save_fl_wrsh_to_file(dist, gm->size, output_file);
    printf("Shortest distances saved to %s\n", output_file);

    free_dist_matrix(dist, gm->size);
    free_gm(&gm);

    return 0;
}