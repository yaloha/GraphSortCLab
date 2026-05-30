#include "graph.h"
#include <stdlib.h>
#include <stdio.h>

int min(int a, int b){
    return (a < b) ? a : b;
}

int** floyd_warshall(GraphMatrix* gm){
    if (gm == NULL || gm->matrix == NULL) {
        printf("floyd_warshall: graph is not initialized\n");
        return NULL;
    }
    int n = gm->size;
    int** dist = malloc(sizeof(int*) * n);
    if (dist == NULL){
        printf("floyd_warshall: error allocating dist matrix\n");
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        dist[i] = malloc(sizeof(int) * n);
        if (dist[i] == NULL) {
            for (int j = 0; j < i; j++) free(dist[j]);
            free(dist);
            printf("floyd_warshall: failed to allocate dist rows\n");
            return NULL;
        }
        for (int j = 0; j < n; j++)
            dist[i][j] = gm->matrix[i][j];
    }

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
    return dist;
}

void free_dist_matrix(int** dist, int n) {
    if (dist == NULL) return;
    for (int i = 0; i < n; i++) free(dist[i]);
    free(dist);
}

void save_fl_wrsh_to_file(int** res_matrix, int n, const char* filename) {
    if (res_matrix == NULL) {
        printf("save_fl_wrsh: no matrix is given\n");
        return;
    }

    if (filename == NULL){
        printf("save_fl_wrsh: filename is not set\n");
    }

    FILE* f = fopen(filename, "a");
    if (f == NULL) {
        printf("save_fl_wrsh: cannot open file %s for writing\n", filename);
        return;
    }

    fprintf(f, "floyd-warshall applied\n\n");
    fprintf(f, "dimensions: %d\n", n);
    fprintf(f, "resulting distance matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (res_matrix[i][j] == INF)
                fprintf(f, "INF");
            else
                fprintf(f, "%d", res_matrix[i][j]);
            if (j < n - 1) fprintf(f, " ");
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
    fclose(f);
}