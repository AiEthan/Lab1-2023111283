#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>
#include <algorithm>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

class Graph {
private:
    struct Edge {
        int to;
        int weight;
    };
    struct Node {
        string word;
        vector<Edge> outEdges;
    };
    vector<Node> nodes;
    unordered_map<string, int> wordToIndex;
    vector<double> pageRank;
    bool pageRankComputed;

    static string toLower(const string& s);
    void computePageRank(double d = 0.85, int maxIter = 100, double tol = 1e-8);
    pair<int, vector<int>> dijkstra(int src, int dest) const;

public:
    Graph() : pageRankComputed(false) {}
    int addWord(const string& word);
    void addEdge(int fromIdx, int toIdx);
    bool buildFromFile(const string& filename);
    void showDirectedGraph() const;
    string queryBridgeWords(const string& word1, const string& word2) const;
    string generateNewText(const string& inputText) const;
    string calcShortestPath(const string& word1, const string& word2) const;
    double calPageRank(const string& word);
    string randomWalk();
};

#endif