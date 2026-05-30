#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

GraphMatrix* init_gm(int n){
    GraphMatrix* new_gm = (GraphMatrix*) malloc(sizeof(GraphMatrix));
    if (new_gm == NULL){
        printf("error allocating memory to GraphMatrix object\n");
        return NULL;
    }
    new_gm->size = n;
    int** new_mtx = (int**) malloc(sizeof(int*) * n);
    if (new_mtx == NULL){
        printf("error allocating memory to matrix columns\n");
        free(new_gm);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        new_mtx[i] = (int*) malloc(sizeof(int) * n);
        if (new_mtx[i] == NULL) {
            printf("error allocating memory for matrix rows\n");
            for (int j = 0; j < i; j++) free(new_mtx[j]);
            free(new_mtx);
            free(new_gm);
            return NULL;
        }
    }
    for(int i = 0; i < n * n; i++)
        if ((i / n) == (i % n))
            new_mtx[i / n][i % n] = 0;
        else
            new_mtx[i / n][i % n] = INF;
    new_gm->matrix = new_mtx;
    return new_gm;
}

void free_gm(GraphMatrix** p_gm){
    if (p_gm == NULL || *p_gm == NULL) {
        return;
    }
    GraphMatrix* gm = *p_gm;
    for (int i = 0; i < gm->size; i++) {
        free(gm->matrix[i]);
    }
    free(gm->matrix);
    free(gm);
    *p_gm = NULL;
}

void add_edge(GraphMatrix* gm, int i, int j, int dist){
    if (gm == NULL || gm->matrix == NULL) {
        printf("add_edge: graph is not initialized\n");
        return;
    }
    if ((i < 0 || i >= gm->size) || (j < 0 || j >= gm->size)) {
        printf("add_edge: vertex index out of range [%d,%d]\n", i, j);
        return;
    }
    if (i == j) {
        printf("add_edge: loops are not allowed (ignored)\n");
        return;
    }
    gm->matrix[i][j] = dist;
}

void remove_edge(GraphMatrix* gm, int i, int j){
    if (gm == NULL || gm->matrix == NULL) {
        printf("remove_edge: graph is not initialized\n");
        return;
    }
    if ((i < 0 || i >= gm->size) || (j < 0 || j >= gm->size)) {
        printf("remove_edge: vertex index out of range [%d,%d]\n", i, j);
        return;
    }
    if (i == j) {
        return;
    }
    gm->matrix[i][j] = INF;
}

int get_edge(GraphMatrix* gm, int i, int j){
    if (gm == NULL || gm->matrix == NULL) {
        printf("get_edge: graph is not initialized\n");
        return INF;
    }
    if ((i < 0 || i >= gm->size) || (j < 0 || j >= gm->size)) {
        printf("get_edge: vertex index out of range [%d,%d]\n", i, j);
        return INF;
    }
    return gm->matrix[i][j];
}

void save_graph_to_file(const GraphMatrix* gm, const char* filename) {
    if (gm == NULL || gm->matrix == NULL) {
        printf("save_graph: graph parameter is empty\n");
        return;
    }

    if (filename == NULL){
        printf("save_graph: filename is not set\n");
    }

    FILE* f = fopen(filename, "a");
    if (f == NULL) {
        printf("save_graph: can't open file %s for writing\n", filename);
        return;
    }

    int n = gm->size;
    fprintf(f, "dimensions: %d\n", n);
    fprintf(f, "adjacency matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(f, "%d", gm->matrix[i][j]);
            if (j < n - 1) fprintf(f, " ");
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
    fclose(f);
}

GraphMatrix* load_graph_from_file(const char* filename) {
    if (filename == NULL) {
        printf("load_graph: filename is not set\n");
        return NULL;
    }

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("load_graph: can't open file %s\n", filename);
        return NULL;
    }

    int n;
    if (fscanf(f, "%d", &n) != 1 || n <= 0) {
        printf("load_graph: invalid matrix size\n");
        fclose(f);
        return NULL;
    }

    GraphMatrix* gm = init_gm(n);
    if (gm == NULL) {
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int val;
            if (fscanf(f, "%d", &val) != 1) {
                printf("load_graph: not enough data in file\n");
                free_gm(&gm);
                fclose(f);
                return NULL;
            }
            gm->matrix[i][j] = val;
        }
    }

    fclose(f);
    return gm;
}