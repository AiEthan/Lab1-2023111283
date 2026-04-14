#include <gtest/gtest.h>
#include "graph.h"

TEST(BridgeWordsTest, ValidBridgeWord) {
    Graph g;
    int a = g.addWord("a");
    int b = g.addWord("b");
    int c = g.addWord("c");
    g.addEdge(a, b);
    g.addEdge(b, c);
    std::string result = g.queryBridgeWords("a", "c");
    EXPECT_EQ(result, "The bridge words from \"a\" to \"c\" is: \"b\".");
}

TEST(BridgeWordsTest, NoBridgeWord) {
    Graph g;
    int a = g.addWord("a");
    int c = g.addWord("c");
    g.addEdge(a, c);
    std::string result = g.queryBridgeWords("a", "c");
    EXPECT_EQ(result, "No bridge words from \"a\" to \"c\"!");
}

TEST(BridgeWordsTest, WordNotInGraph) {
    Graph g;
    g.addWord("a");
    std::string result = g.queryBridgeWords("a", "b");
    EXPECT_EQ(result, "No \"b\" in the graph!");
}
