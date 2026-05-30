#include <gtest/gtest.h>

extern "C" {
#include "graph.h"
#include "fl_wrsh.h"
}

TEST(GraphTest, InitGraph) {
GraphMatrix* g = init_gm(3);
ASSERT_NE(g, nullptr);
EXPECT_EQ(g->size, 3);
free_gm(&g);
}

TEST(GraphTest, AddAndGetEdge) {
GraphMatrix* g = init_gm(3);
add_edge(g, 0, 1, 5);
EXPECT_EQ(get_edge(g, 0, 1), 5);
free_gm(&g);
}

TEST(FloydTest, SimpleGraph) {
GraphMatrix* g = init_gm(3);
add_edge(g, 0, 1, 3);
add_edge(g, 1, 2, 1);
add_edge(g, 0, 2, 10);

int** dist = floyd_warshall(g);
EXPECT_EQ(dist[0][2], 4);
free_dist_matrix(dist, 3);
free_gm(&g);
}

TEST(FloydTest, UnreachableVertex) {
GraphMatrix* g = init_gm(2);
int** dist = floyd_warshall(g);
EXPECT_EQ(dist[0][1], INF);
free_dist_matrix(dist, 2);
free_gm(&g);
}

TEST(FloydTest, UndirectedGraph) {
GraphMatrix* g = init_gm(3);
add_edge(g, 0, 1, 4);
add_edge(g, 1, 0, 4);
add_edge(g, 1, 2, 2);
add_edge(g, 2, 1, 2);

int** dist = floyd_warshall(g);
EXPECT_EQ(dist[0][2], 6);
free_dist_matrix(dist, 3);
free_gm(&g);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}