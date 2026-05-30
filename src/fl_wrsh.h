#include "graph.h"
#ifndef GRAPHSORTCLAB_FL_WRSH_H
#define GRAPHSORTCLAB_FL_WRSH_H

int min(int a, int b);
int** floyd_warshall(GraphMatrix* gm);
void free_dist_matrix(int** dist, int n);
void save_fl_wrsh_to_file(int** res_matrix, int n, const char* filename);

#endif //GRAPHSORTCLAB_FL_WRSH_H
