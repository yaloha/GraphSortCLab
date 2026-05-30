#ifndef GRAPHSORTCLAB_GRAPH_H
#define GRAPHSORTCLAB_GRAPH_H

#define INF 2147483647 / 2

typedef struct{
    int size;
    int** matrix;
}GraphMatrix;

GraphMatrix* init_gm(int n);

void free_gm(GraphMatrix** p_gm);

void add_edge(GraphMatrix* gm, int i, int j, int dist);

void remove_edge(GraphMatrix* gm, int i, int j);

int get_edge(GraphMatrix* gm, int i, int j);

void save_graph_to_file(const GraphMatrix* gm, const char* filename);

GraphMatrix* load_graph_from_file(const char* filename);

#endif
