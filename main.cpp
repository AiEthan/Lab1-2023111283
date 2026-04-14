#include "graph.h"

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
        cin.ignore();
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
    return 0;
}