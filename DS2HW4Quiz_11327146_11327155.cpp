/*11327146 莊立聖 11327155 黃宇謙*/
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <map>
#include <queue>
#include <cstring>
#include <set>
#include <limits>
#include <chrono>
#include <cctype>

using namespace std;

#define SIZE_OF_ID 12

struct Edge {
    string receiverId;
    float weight;
    
    bool operator<(const Edge& other) const {
        return receiverId < other.receiverId;
    }
};

struct EdgeRecord {
    char sender[SIZE_OF_ID];
    char receiver[SIZE_OF_ID];
    float weight;
};

class Graph {
private:
    struct Node {
        string senderId;
        vector<Edge> edges;
        
        bool operator<(const Node& other) const {
            return senderId < other.senderId;
        }
    };
    
    vector<Node> adjList;
    string fileName;

public:
    Graph() {}
    
    void clear() {
        adjList.clear();
        fileName = "";
    }

    bool task1() {
        cout << "\nInput a file number ([0] Quit): ";
        string num;
        if (!(cin >> num)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return false;
        }
        if (num == "0") {
            cout << endl;
            return false;
        }
        fileName = "pairs" + num + ".bin";
        ifstream file(fileName, ios::binary);
        if (!file.is_open()) {
            cout << "\n### " << fileName << " does not exist! ###\n\n";
            return true;
        }

        map<string, vector<Edge>> graphMap;
        set<string> allNodes;

        // Read binary file and build graph map
        EdgeRecord record;
        while (file.read((char*)&record, sizeof(EdgeRecord))) {
            string sender = record.sender;
            string receiver = record.receiver;
            graphMap[sender].push_back({receiver, record.weight});
            allNodes.insert(sender);
            allNodes.insert(receiver);
        }
        file.close();

        adjList.clear();

        // Build adjacency list from graph map
        for (const string& id : allNodes) {
            Node node;
            node.senderId = id;
            if (graphMap.count(id)) {
                node.edges = graphMap[id];
                sort(node.edges.begin(), node.edges.end());
            }
            adjList.push_back(node);
        }
        sort(adjList.begin(), adjList.end());

        // Write adjacency list to output file
        string outName = "pairs" + num + ".adj";
        ofstream outFile(outName);
        if (outFile.is_open()) {
            outFile << "<<< There are " << adjList.size() << " IDs in total. >>>\n";
            int idx = 1;
            int totalNodes = 0;
            for (const auto& node : adjList) {
                totalNodes += node.edges.size();
                outFile << "[" << setw(3) << idx << "] " << node.senderId << ": \n";
                int count = 1;
                for (int j = 0; j < (int)node.edges.size(); j++) {
                    if (j % 12 == 0) outFile << "\t";
                    outFile << "(" << setw(2) << count << ") " << node.edges[j].receiverId << ", " << setw(6) << node.edges[j].weight;
                    if (j % 12 == 11) outFile << "\n";
                    else if (j != (int)node.edges.size() - 1) outFile << "\t";
                    count++;
                }
                outFile << "\n";
                idx++;
            }
            outFile << "<<< There are " << totalNodes << " nodes in total. >>>\n";
            outFile.close();
        }
        
        cout << "\n<<< There are " << adjList.size() << " IDs in total. >>>\n";
        int nodeCount = 0;
        for (const auto& n : adjList) nodeCount += n.edges.size();
        cout << "\n<<< There are " << nodeCount << " nodes in total. >>>\n\n";
        
        return true;
    }

    struct Result {
        string senderId;
        int connectionCount;
        set<string> connectedNodes;
        
        bool operator<(const Result& other) const {
            if (connectionCount != other.connectionCount)
                return connectionCount > other.connectionCount;
            return senderId < other.senderId; 
        }
    };

    bool task2() {
        if (adjList.empty()) {
            cout << "### There is no graph and choose 1 first. ###\n\n";
            return true;
        }

        map<string, int> idToIndex;
        for (int i = 0; i < adjList.size(); ++i) {
            idToIndex[adjList[i].senderId] = i;
        }

        vector<Result> bfsResults;

        for (int i = 0; i < adjList.size(); ++i) {
            if (adjList[i].edges.empty()) continue; // skip those who never sent if required, or still run BFS

            set<string> visited;
            queue<string> q;
            q.push(adjList[i].senderId);
            visited.insert(adjList[i].senderId);
            set<string> reachable;

            while (!q.empty()) {
                string curr = q.front();
                q.pop();

                if (idToIndex.count(curr)) {
                    int currIdx = idToIndex[curr];
                    for (const auto& edge : adjList[currIdx].edges) {
                        if (visited.find(edge.receiverId) == visited.end()) {
                            visited.insert(edge.receiverId);
                            reachable.insert(edge.receiverId);
                            q.push(edge.receiverId);
                        }
                    }
                }
            }
            
            Result res;
            res.senderId = adjList[i].senderId;
            res.connectionCount = reachable.size();
            res.connectedNodes = reachable;
            bfsResults.push_back(res);
        }

        sort(bfsResults.begin(), bfsResults.end());

        string outName = fileName.substr(0, fileName.find_last_of('.')) + ".cnt";
        ofstream outFile(outName);
        if (outFile.is_open()) {
            outFile << "<<< There are " << bfsResults.size() << " IDs in total. >>>\n";
            int idx = 1;
            for (const auto& res : bfsResults) {
                outFile << "[" << setw(3) << idx << "] " << res.senderId << "(" << res.connectionCount << "): \n";
                if (!res.connectedNodes.empty()) {
                    int count = 1;
                    int j = 0;
                    for (const auto& node : res.connectedNodes) {
                        if (j % 12 == 0) outFile << "\t";
                        outFile << "(" << setw(2) << count << ") " << node;
                        if (j % 12 == 11) outFile << "\n";
                        else if (j != (int)res.connectedNodes.size() - 1) outFile << "\t";
                        j++;
                        count++;
                    }
                }
                outFile << "\n";
                idx++;
            }
            outFile.close();
        }
        
        cout << "\n<<< There are " << bfsResults.size() << " IDs in total. >>>\n\n";

        return true;
    }

    bool task3() {
        if (adjList.empty()) {
            cout << "### There is no graph and choose 1 first. ###\n\n";
            return true;
        }

        string thresholdInput;
        float threshold = 0.0f;
        while (true) {
            cout << "\nInput a real number in [0.66,1.0]: ";
            if (!(cin >> thresholdInput)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (all_of(thresholdInput.begin(), thresholdInput.end(),
                       [](unsigned char c) { return c == '.'; })) {
                cout << "\n### It is NOT in [0.66,1.0] ###" << endl;
                continue;
            }
            try {
                size_t pos = 0;
                threshold = stof(thresholdInput, &pos);
                if (pos != thresholdInput.size()) {
                    // Treat inputs like ".." or "1.2.3" as numeric-ish and report range error.
                    if (all_of(thresholdInput.begin(), thresholdInput.end(),
                               [](unsigned char c) { return isdigit(c) || c == '.'; })) {
                        cout << "\n### It is NOT in [0.66,1.0] ###" << endl;
                    }
                    continue;
                }
                if (threshold < 0.66f || threshold > 1.0f) {
                    cout << "\n### It is NOT in [0.66,1.0] ###" << endl;
                    continue;
                }
                break;
            } catch (...) {
                continue;
            }
        }

        map<string, int> idToIndex;
        for (int i = 0; i < (int)adjList.size(); ++i) {
            idToIndex[adjList[i].senderId] = i;
        }

        vector<Result> results;
        for (int i = 0; i < (int)adjList.size(); ++i) {
            if (adjList[i].edges.empty()) continue;

            set<string> visited;
            set<string> reachable;
            vector<string> stack;

            stack.push_back(adjList[i].senderId);
            visited.insert(adjList[i].senderId);

            while (!stack.empty()) {
                string curr = stack.back();
                stack.pop_back();

                if (idToIndex.count(curr)) {
                    int currIdx = idToIndex[curr];
                    for (const auto& edge : adjList[currIdx].edges) {
                        if (edge.weight >= threshold && visited.find(edge.receiverId) == visited.end()) {
                            visited.insert(edge.receiverId);
                            reachable.insert(edge.receiverId);
                            stack.push_back(edge.receiverId);
                        }
                    }
                }
            }

            if (!reachable.empty()) {
                Result res;
                res.senderId = adjList[i].senderId;
                res.connectionCount = reachable.size();
                res.connectedNodes = reachable;
                results.push_back(res);
            }
        }

        sort(results.begin(), results.end());

        string outName = fileName.substr(0, fileName.find_last_of('.')) + ".inf";
        ofstream outFile(outName);
        if (outFile.is_open()) {
            outFile << "<<< There are " << results.size() << " IDs in total. >>>\n";
            int idx = 1;
            for (const auto& res : results) {
                outFile << "[" << setw(3) << idx << "] " << res.senderId << "(" << res.connectionCount << "): \n";
                if (!res.connectedNodes.empty()) {
                    int count = 1;
                    int j = 0;
                    for (const auto& node : res.connectedNodes) {
                        if (j % 12 == 0) outFile << "\t";
                        outFile << "(" << setw(2) << count << ") " << node;
                        if (j % 12 == 11) outFile << "\n";
                        else if (j != (int)res.connectedNodes.size() - 1) outFile << "\t";
                        j++;
                        count++;
                    }
                }
                outFile << "\n";
                idx++;
            }
            outFile.close();
        }

        cout << "\n<<< There are " << results.size() << " IDs in total. >>>\n\n";

        return true;
    }

    bool task4() {
        if (adjList.empty()) {
            cout << "### There is no graph and choose 1 first. ###\n\n";
            return true;
        }

        const float threshold = 0.66f;

        map<string, int> idToIndex;
        for (int i = 0; i < (int)adjList.size(); ++i) {
            idToIndex[adjList[i].senderId] = i;
        }

        vector<Result> results;

        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < (int)adjList.size(); ++i) {
            if (adjList[i].edges.empty()) continue;

            set<string> visited;
            set<string> reachable;
            vector<string> stack;

            stack.push_back(adjList[i].senderId);
            visited.insert(adjList[i].senderId);

            while (!stack.empty()) {
                string curr = stack.back();
                stack.pop_back();

                if (idToIndex.count(curr)) {
                    int currIdx = idToIndex[curr];
                    for (const auto& edge : adjList[currIdx].edges) {
                        if (edge.weight >= threshold && visited.find(edge.receiverId) == visited.end()) {
                            visited.insert(edge.receiverId);
                            reachable.insert(edge.receiverId);
                            stack.push_back(edge.receiverId);
                        }
                    }
                }
            }

            if (!reachable.empty()) {
                Result res;
                res.senderId = adjList[i].senderId;
                res.connectionCount = reachable.size();
                res.connectedNodes = reachable;
                results.push_back(res);
            }
        }
        auto end = chrono::high_resolution_clock::now();

        auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        sort(results.begin(), results.end());

        cout << "\n[Elapsed time] " << elapsed << " ms\n\n";

        if (results.empty()) {
            cout << "Input an integer to show top-K in [1,0]: ";
            cout << "\n";
            return true;
        }

        int k = 0;
        string kInput;
        while (true) {
            cout << "Input an integer to show top-K in [1," << results.size() << "]: ";
            if (!(cin >> kInput)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            try {
                size_t pos = 0;
                k = stoi(kInput, &pos);
                if (pos != kInput.size()) {
                    cout << "\n";
                    continue;
                }
                if (k < 1 || k > (int)results.size()) {
                    cout << "\n### " << k << " is NOT in [1," << results.size() << "] ###\n\n";
                    continue;
                }
                break;
            } catch (...) {
                cout << "\n";
                continue;
            }
        }

        int cutoff = results[k - 1].connectionCount;
        int rank = 1;
        cout << "\n";
        for (int i = 0; i < (int)results.size(); ++i) {
            if (i < k || results[i].connectionCount == cutoff) {
                cout << "<" << rank << "> " << results[i].senderId << ": " << results[i].connectionCount << "\n";
                rank++;
            } else {
                break;
            }
        }
        cout << "\n";
        return true;
    }
};

int main() {
    Graph g;
    string command;
    while (true) {
        cout << "* Data Structures and Algorithms *\n"
             << "**** Graph data manipulation *****\n"
             << "* 0. QUIT                        *\n"
             << "* 1. Build adjacency lists       *\n"
             << "* 2. Compute connection counts   *\n"
             << "* 3. Estimate influence values   *\n"
             << "* 4. Find top-k influence values *\n"
             << "**********************************\n"
             << "Input a choice(0, 1, 2, 3, 4): ";
        cin >> command;
        if (command == "0") {
            break;
        } else if (command == "1") {
            g.clear();
            g.task1();
        } else if (command == "2") {
            g.task2();
        } else if (command == "3") {
            g.task3();
        } else if (command == "4") {
            g.task4();
        } else {
            cout << "\nCommand does not exist!\n\n";
        }
    }
    return 0;
}
