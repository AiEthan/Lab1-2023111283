#include <gtest/gtest.h>
#include "graph.h"

TEST(ShortestPathTest, WordNotInGraph) {
    Graph g;
    g.addWord("a");
    EXPECT_EQ(g.calcShortestPath("a", "b"), "No word in graph!");
}

TEST(ShortestPathTest, PathExists) {
    Graph g;
    int a = g.addWord("a");
    int b = g.addWord("b");
    int c = g.addWord("c");
    g.addEdge(a, b);
    g.addEdge(b, c);
    EXPECT_EQ(g.calcShortestPath("a", "c"), 
              "The shortest path from \"a\" to \"c\" is: a → b → c (length = 2)");
}

TEST(ShortestPathTest, NoPath) {
    Graph g;
    int a = g.addWord("a");
    int b = g.addWord("b");
    int c = g.addWord("c");
    g.addEdge(a, b);  // 没有 b->c 或 a->c
    EXPECT_EQ(g.calcShortestPath("a", "c"), 
              "No path from \"a\" to \"c\"!");
}

TEST(ShortestPathTest, MultipleShortestPaths) {
    Graph g;
    int a = g.addWord("a");
    int b = g.addWord("b");
    int c = g.addWord("c");
    int d = g.addWord("d");
    g.addEdge(a, b);
    g.addEdge(a, c);
    g.addEdge(b, d);
    g.addEdge(c, d);
    string result = g.calcShortestPath("a", "d");
    // 两种可能的结果都是正确的
    EXPECT_TRUE(result == "The shortest path from \"a\" to \"d\" is: a → b → d (length = 2)" ||
                result == "The shortest path from \"a\" to \"d\" is: a → c → d (length = 2)");
}