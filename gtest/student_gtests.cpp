#include <gtest/gtest.h>

#include "graph.h"

// Test fixture to set up the Small Graph 
class SmallGraphTest : public ::testing::Test {
protected:
    Graph G;

    void SetUp() override {
        
        G.numVertices = 4;
        G.push_back(Edge{0, 3, 1});
        G.push_back(Edge{3, 0, 3});
        G.push_back(Edge{3, 1, 2});
        G.push_back(Edge{1, 0, 5});
        G.push_back(Edge{1, 2, 3});
        G.push_back(Edge{2, 3, 8});
    }
};


// DisjointSet Tests

TEST(DisjointSetTest, Operations) {
    DisjointSet ds(4);

    // Initial roots should be themselves 
    EXPECT_EQ(ds.findSet(0), 0);
    EXPECT_EQ(ds.findSet(1), 1);

    // Union two sets and verify representation 
    ds.unionSets(0, 1);
    EXPECT_EQ(ds.findSet(0), ds.findSet(1));

    // Union another independent element
    ds.unionSets(2, 3);
    EXPECT_EQ(ds.findSet(2), ds.findSet(3));
    EXPECT_NE(ds.findSet(0), ds.findSet(2)); 

    // Merge everything together
    ds.unionSets(1, 2);
    EXPECT_EQ(ds.findSet(0), ds.findSet(3));
}


// Graph Utility Methods Tests

TEST_F(SmallGraphTest, SortEdges) {
    Graph sortedG = G.sort_edges();
    
    ASSERT_EQ(sortedG.size(), 6);
    EXPECT_EQ(sortedG[0].weight, 1); // Edge (0,3,1)
    EXPECT_EQ(sortedG[1].weight, 2); // Edge (3,1,2)
    EXPECT_EQ(sortedG[5].weight, 8); // Edge (2,3,8)
}

TEST_F(SmallGraphTest, EdgesFrom) {
    VertexList neighborsOf3 = G.edges_from(3);
    
    // Vertex 3 has outgoing edges to 0 and 1
    ASSERT_EQ(neighborsOf3.size(), 2);
    EXPECT_EQ(neighborsOf3[0], 0);
    EXPECT_EQ(neighborsOf3[1], 1);
}


// Graph Algorithm Test

TEST_F(SmallGraphTest, KruskalsMST) {
    EdgeList mst = Kruskals(G);
    
    // Target MST edges given by output: (0,3,1) (3,1,2) (1,2,3) 
    ASSERT_EQ(mst.size(), 3);
    EXPECT_EQ(sum_weights(mst), 6);
    
    EXPECT_EQ(mst[0].u, 0); EXPECT_EQ(mst[0].v, 3); EXPECT_EQ(mst[0].weight, 1);
    EXPECT_EQ(mst[1].u, 3); EXPECT_EQ(mst[1].v, 1); EXPECT_EQ(mst[1].weight, 2);
    EXPECT_EQ(mst[2].u, 1); EXPECT_EQ(mst[2].v, 2); EXPECT_EQ(mst[2].weight, 3);
}

TEST_F(SmallGraphTest, DepthFirstSearch) {
    VertexList expected_dfs = {0, 3, 1, 2};
    EXPECT_EQ(dfs(G, 0), expected_dfs);
}

TEST_F(SmallGraphTest, BreadthFirstSearch) {
    VertexList expected_bfs = {0, 3, 1, 2}; 
    EXPECT_EQ(bfs(G, 0), expected_bfs);
}

