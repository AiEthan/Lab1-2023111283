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
        int to;          // 目标节点索引
        int weight;      // 权重
    };

    struct Node {
        string word;            // 单词（小写形式）
        vector<Edge> outEdges;  // 出边
    };

    vector<Node> nodes;                     // 节点列表
    unordered_map<string, int> wordToIndex; // 单词到索引的映射
    vector<double> pageRank;                 // 存储计算好的 PageRank 值
    bool pageRankComputed;                   // 标记是否已计算 PageRank

    // 辅助函数：将字符串转换为小写
    static string toLower(const string& s);

    // 计算所有节点的 PageRank（迭代方法）
    void computePageRank(double d = 0.85, int maxIter = 100, double tol = 1e-8);

    // Dijkstra 算法辅助函数，返回从 src 到 dest 的距离和前驱数组
    pair<int, vector<int>> dijkstra(int src, int dest) const;

public:
    // 构造函数
    Graph() : pageRankComputed(false) {}

    // 添加单词，返回索引
    int addWord(const string& word);

    // 添加边（从 fromIdx 到 toIdx，增加权重）
    void addEdge(int fromIdx, int toIdx);

    // 从文件构建图
    bool buildFromFile(const string& filename);

    // 展示有向图（在命令行打印）
    void showDirectedGraph() const;

    // 查询桥接词
    string queryBridgeWords(const string& word1, const string& word2) const;

    // 根据桥接词生成新文本
    string generateNewText(const string& inputText) const;

    // 计算两个单词之间的最短路径
    string calcShortestPath(const string& word1, const string& word2) const;

    // 计算单词的 PageRank 值
    double calPageRank(const string& word);

    // 随机游走，返回路径字符串并保存到文件
    string randomWalk();
};

bool Graph::buildFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    vector<string> words;  // 存储当前行的单词（临时）
    while (getline(file, line)) {
        // 将非字母字符替换为空格
        for (char& c : line) {
            if (!isalpha(c)) c = ' ';
            else c = tolower(c);
        }
        // 使用 stringstream 提取单词
        stringstream ss(line);
        string word;
        while (ss >> word) {
            words.push_back(word);
        }
    }
    file.close();

    // 构建图：相邻单词建立边
    for (size_t i = 0; i + 1 < words.size(); ++i) {
        int fromIdx = addWord(words[i]);
        int toIdx = addWord(words[i+1]);
        addEdge(fromIdx, toIdx);
    }
    return true;
}

string Graph::toLower(const string& s) {
    string result = s;
    for (char &c : result) c = tolower(c);
    return result;
}

int Graph::addWord(const string& word) {
    auto it = wordToIndex.find(word);
    if (it != wordToIndex.end()) return it->second;
    int idx = nodes.size();
    nodes.push_back({word, {}});
    wordToIndex[word] = idx;
    return idx;
}

void Graph::addEdge(int fromIdx, int toIdx) {
    for (auto& e : nodes[fromIdx].outEdges) {
        if (e.to == toIdx) {
            e.weight++;
            return;
        }
    }
    nodes[fromIdx].outEdges.push_back({toIdx, 1});
}

void Graph::showDirectedGraph() const {
    for (size_t i = 0; i < nodes.size(); ++i) {
        cout << nodes[i].word << " -> ";
        for (const auto& e : nodes[i].outEdges) {
            cout << nodes[e.to].word << "(" << e.weight << ") ";
        }
        cout << endl;
    }
}

string Graph::queryBridgeWords(const string& word1, const string& word2) const {
    // 先转为小写
    string w1 = toLower(word1), w2 = toLower(word2);
    auto it1 = wordToIndex.find(w1);
    auto it2 = wordToIndex.find(w2);
    if (it1 == wordToIndex.end() && it2 == wordToIndex.end())
        return "No \"" + w1 + "\" and \"" + w2 + "\" in the graph!";
    else if (it1 == wordToIndex.end())
        return "No \"" + w1 + "\" in the graph!";
    else if (it2 == wordToIndex.end())
        return "No \"" + w2 + "\" in the graph!";

    int idx1 = it1->second, idx2 = it2->second;
    vector<string> bridges;
    // 遍历所有节点作为候选
    for (size_t i = 0; i < nodes.size(); ++i) {
        // 检查是否存在 word1 -> i 的边
        bool hasEdge1 = false;
        for (const auto& e : nodes[idx1].outEdges) {
            if (e.to == (int)i) { hasEdge1 = true; break; }
        }
        if (!hasEdge1) continue;
        // 检查是否存在 i -> word2 的边
        bool hasEdge2 = false;
        for (const auto& e : nodes[i].outEdges) {
            if (e.to == idx2) { hasEdge2 = true; break; }
        }
        if (hasEdge2) {
            bridges.push_back(nodes[i].word);
        }
    }

    if (bridges.empty())
        return "No bridge words from \"" + w1 + "\" to \"" + w2 + "\"!";
    // 构建返回字符串
    string result = "The bridge words from \"" + w1 + "\" to \"" + w2 + "\" ";
    if (bridges.size() == 1) {
        result += "is: \"" + bridges[0] + "\".";
    } else {
        result += "are: ";
        for (size_t i = 0; i < bridges.size(); ++i) {
            if (i > 0) {
                if (i == bridges.size() - 1) result += " and ";
                else result += ", ";
            }
            result += "\"" + bridges[i] + "\"";
        }
        result += ".";
    }
    return result;
}

string Graph::generateNewText(const string& inputText) const {
    // 处理输入文本，提取单词（小写）
    string processed = inputText;
    for (char& c : processed) {
        if (!isalpha(c)) c = ' ';
        else c = tolower(c);
    }
    stringstream ss(processed);
    vector<string> words;
    string w;
    while (ss >> w) words.push_back(w);

    if (words.empty()) return "";

    // 构建新文本
    string result = words[0];
    for (size_t i = 0; i + 1 < words.size(); ++i) {
        // 查找桥接词
        auto it1 = wordToIndex.find(words[i]);
        auto it2 = wordToIndex.find(words[i+1]);
        if (it1 != wordToIndex.end() && it2 != wordToIndex.end()) {
            int idx1 = it1->second, idx2 = it2->second;
            vector<int> bridgeIndices;
            // 找到所有满足条件的 bridge 节点索引
            for (size_t j = 0; j < nodes.size(); ++j) {
                bool hasEdge1 = false, hasEdge2 = false;
                for (const auto& e : nodes[idx1].outEdges)
                    if (e.to == (int)j) { hasEdge1 = true; break; }
                if (!hasEdge1) continue;
                for (const auto& e : nodes[j].outEdges)
                    if (e.to == idx2) { hasEdge2 = true; break; }
                if (hasEdge2) bridgeIndices.push_back(j);
            }
            if (!bridgeIndices.empty()) {
                // 随机选择一个
                int pick = bridgeIndices[rand() % bridgeIndices.size()];
                result += " " + nodes[pick].word;
            }
        }
        result += " " + words[i+1];
    }
    return result;
}

pair<int, vector<int>> Graph::dijkstra(int src, int dest) const {
    int n = nodes.size();
    vector<int> dist(n, INT_MAX);
    vector<int> prev(n, -1); // 记录前驱节点
    dist[src] = 0;
    using pii = pair<int, int>; // (距离, 节点)
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        if (u == dest) break; // 可选提前结束
        for (const auto& e : nodes[u].outEdges) {
            int v = e.to;
            int w = e.weight;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return {dist[dest], prev};
}

string Graph::calcShortestPath(const string& word1, const string& word2) const {
    // 转为小写
    string w1 = toLower(word1);
    string w2 = toLower(word2);
    auto it1 = wordToIndex.find(w1);
    auto it2 = wordToIndex.find(w2);
    if (it1 == wordToIndex.end() || it2 == wordToIndex.end()) {
        return "No word in graph!";
    }

    int src = it1->second, dest = it2->second;
    auto [dist, prev] = dijkstra(src, dest);
    if (dist == INT_MAX) return "No path from \"" + w1 + "\" to \"" + w2 + "\"!";

    // 重建路径
    vector<int> path;
    for (int v = dest; v != -1; v = prev[v]) path.push_back(v);
    reverse(path.begin(), path.end());
    string result = "The shortest path from \"" + w1 + "\" to \"" + w2 + "\" is: ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) result += " → ";
        result += nodes[path[i]].word;
    }
    result += " (length = " + to_string(dist) + ")";
    return result;
}

void Graph::computePageRank(double d, int maxIter, double tol) {
    int N = nodes.size();
    if (N == 0) return;
    vector<double> pr(N, 1.0 / N); // 初始均匀
    vector<double> outDegree(N);
    for (int i = 0; i < N; ++i) {
        outDegree[i] = nodes[i].outEdges.size();
    }

    for (int iter = 0; iter < maxIter; ++iter) {
        vector<double> newPr(N, (1 - d) / N);
        for (int u = 0; u < N; ++u) {
            if (outDegree[u] == 0) {
                // 出度为0，均匀转移给所有节点
                for (int v = 0; v < N; ++v) {
                    newPr[v] += d * pr[u] / N;
                }
            } else {
                double contrib = d * pr[u] / outDegree[u];
                for (const auto& e : nodes[u].outEdges) {
                    newPr[e.to] += contrib;
                }
            }
        }
        // 检查收敛
        double diff = 0;
        for (int i = 0; i < N; ++i) diff += fabs(newPr[i] - pr[i]);
        pr = std::move(newPr);
        if (diff < tol) break;
    }
    // 保存结果
    pageRank = pr;
}

double Graph::calPageRank(const string& word) {
    auto it = wordToIndex.find(toLower(word));
    if (it == wordToIndex.end()) return -1.0; // 或抛出异常
    // 如果还没计算，先计算
    if (pageRank.empty()) computePageRank();
    return pageRank[it->second];
}

string Graph::randomWalk() {
    if (nodes.empty()) return "";
    srand(time(0));
    int current = rand() % nodes.size();
    string result;
    set<pair<int,int>> visitedEdges;
    while (true) {
        // 输出当前节点
        if (!result.empty()) result += " ";
        result += nodes[current].word;

        // 如果没有出边，停止
        if (nodes[current].outEdges.empty()) break;

        // 随机选择一条出边
        int edgeIdx = rand() % nodes[current].outEdges.size();
        int next = nodes[current].outEdges[edgeIdx].to;
        pair<int,int> edge = {current, next};

        // 检查这条边是否已经走过
        if (visitedEdges.count(edge)) {
            break;  // 遇到重复边，停止（当前节点已输出，不进入下一个）
        } else {
            visitedEdges.insert(edge);
            current = next;
        }
    }

    // 将结果写入文件
    ofstream out("random_walk.txt");
    if (out.is_open()) {
        out << result << endl;
        out.close();
    }
    return result;
}

int main() {
    Graph g;
    string filename;
    cout << "Enter file name: ";
    getline(cin, filename); 
    if (!g.buildFromFile(filename)) {
        cerr << "Failed to open file." << endl;
        return 1;
    }

    int choice;
    do {
        cout << "\nMenu:\n"
             << "1. Show directed graph\n"
             << "2. Query bridge words\n"
             << "3. Generate new text\n"
             << "4. Calculate shortest path\n"
             << "5. Calculate PageRank\n"
             << "6. Random walk\n"
             << "0. Exit\n"
             << "Enter choice: ";
        cin >> choice;
        cin.ignore(); // 忽略换行

        if (choice == 1) {
            g.showDirectedGraph();
        } else if (choice == 2) {
            string w1, w2;
            cout << "Enter two words: ";
            cin >> w1 >> w2;
            cout << g.queryBridgeWords(w1, w2) << endl;
        } else if (choice == 3) {
            string text;
            cout << "Enter text: ";
            getline(cin, text);
            cout << g.generateNewText(text) << endl;
        } else if (choice == 4) {
            string w1, w2;
            cout << "Enter two words: ";
            cin >> w1 >> w2;
            cout << g.calcShortestPath(w1, w2) << endl;
        } else if (choice == 5) {
            string w;
            cout << "Enter word: ";
            cin >> w;
            double pr = g.calPageRank(w);
            if (pr < 0) cout << "Word not in graph!" << endl;
            else cout << "PageRank of \"" << w << "\" = " << pr << endl;
        } else if (choice == 6) {
            cout << "Random walk result: " << g.randomWalk() << endl;
        }
    } while (choice != 0);

    // This is a test modification
    // First
    
    return 0;
}// B2 change X
