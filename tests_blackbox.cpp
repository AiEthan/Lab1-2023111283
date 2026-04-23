#include <gtest/gtest.h>
#include "graph.h"

TEST(BridgeWordsTest, SingleBridgeWord) {
    Graph g;
    g.addWord("a"); g.addWord("b"); g.addWord("c");
    g.addEdge(0,1);  // a->b
    g.addEdge(1,2);  // b->c
    EXPECT_EQ(g.queryBridgeWords("a", "c"), 
              "The bridge words from \"a\" to \"c\" is: \"b\".");
}

TEST(BridgeWordsTest, MultipleBridgeWords) {
    Graph g;
    g.addWord("a"); g.addWord("b"); g.addWord("c"); g.addWord("d");
    g.addEdge(0,1); g.addEdge(0,2);  // a->b, a->c
    g.addEdge(1,3); g.addEdge(2,3);  // b->d, c->d
    EXPECT_EQ(g.queryBridgeWords("a", "d"), 
              "The bridge words from \"a\" to \"d\" are: \"b\" and \"c\".");
}

TEST(BridgeWordsTest, NoBridgeWord) {
    Graph g;
    g.addWord("a"); g.addWord("c");
    g.addEdge(0,1);  // a->c
    EXPECT_EQ(g.queryBridgeWords("a", "c"), 
              "No bridge words from \"a\" to \"c\"!");
}

TEST(BridgeWordsTest, NoEdgeBetweenWords) {
    Graph g;
    g.addWord("a");
    g.addWord("c");
    // 注意：没有添加任何边
    EXPECT_EQ(g.queryBridgeWords("a", "c"), 
              "No bridge words from \"a\" to \"c\"!");
}

TEST(BridgeWordsTest, WordNotInGraph) {
    Graph g;
    g.addWord("a");
    EXPECT_EQ(g.queryBridgeWords("a", "x"), 
              "No \"x\" in the graph!");
}

TEST(BridgeWordsTest, BothWordsNotInGraph) {
    Graph g;
    EXPECT_EQ(g.queryBridgeWords("x", "y"), 
              "No \"x\" and \"y\" in the graph!");
}

TEST(BridgeWordsTest, EmptyStringAsWord1) {
    Graph g;
    g.addWord("a");
    EXPECT_EQ(g.queryBridgeWords("", "a"), 
              "No \"\" in the graph!");
}

TEST(BridgeWordsTest, SelfToSelfSingleNode) {
    Graph g;
    g.addWord("a");
    // 图中只有 a，没有任何边
    EXPECT_EQ(g.queryBridgeWords("a", "a"),
              "No bridge words from \"a\" to \"a\"!");
}

TEST(BridgeWordsTest, SelfToSelfWithBidirectionalEdge) {
    Graph g;
    int a = g.addWord("a");
    int b = g.addWord("b");
    g.addEdge(a, b);  // a -> b
    g.addEdge(b, a);  // b -> a
    // 存在路径 a -> b -> a，因此 b 是桥接词
    EXPECT_EQ(g.queryBridgeWords("a", "a"),
              "The bridge words from \"a\" to \"a\" is: \"b\".");
}