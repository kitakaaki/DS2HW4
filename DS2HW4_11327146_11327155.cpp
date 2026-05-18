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
        cout << "\nInput a file number [0: quit]: ";
        string num;
        cin >> num;
        if (num == "0") return false;

        fileName = "pairs" + num + ".bin";
        ifstream file(fileName, ios::binary);
        if (!file.is_open()) {
            cout << "\n### " << fileName << " does not exist! ###\n";
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
            for (const auto& node : adjList) {
                outFile << "[" << node.senderId << "]\n";
                for (const auto& edge : node.edges) {
                    outFile << "\t" << edge.receiverId << "\t" << fixed << setprecision(2) << edge.weight << "\n";
                }
            }
            outFile.close();
            cout << "\n<" << outName << "> is generated...\n";
        }
        return true;
    }

    struct BFSResult {
        string senderId;
        int connectionCount;
        set<string> connectedNodes;
        
        bool operator<(const BFSResult& other) const {
            if (connectionCount != other.connectionCount)
                return connectionCount > other.connectionCount;
            return senderId > other.senderId; // arbitrary tie breaking or by ID
        }
    };

    bool task2() {
        if (adjList.empty()) {
            cout << "\n### There is no graph and choose 1 first. ###\n";
            return true;
        }

        map<string, int> idToIndex;
        for (int i = 0; i < adjList.size(); ++i) {
            idToIndex[adjList[i].senderId] = i;
        }

        vector<BFSResult> bfsResults;

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
            
            BFSResult res;
            res.senderId = adjList[i].senderId;
            res.connectionCount = reachable.size();
            res.connectedNodes = reachable;
            bfsResults.push_back(res);
        }

        sort(bfsResults.begin(), bfsResults.end());

        string outName = fileName.substr(0, fileName.find_last_of('.')) + ".cnt";
        ofstream outFile(outName);
        if (outFile.is_open()) {
            for (const auto& res : bfsResults) {
                outFile << "[" << res.senderId << "]\t" << res.connectionCount << "\n";
                if (!res.connectedNodes.empty()) {
                    int count = 1;
                    for (const auto& node : res.connectedNodes) {
                        outFile << "\t" << count << ": " << node << "\n";
                        count++;
                    }
                }
            }
            outFile.close();
            cout << "\n<" << outName << "> is generated...\n";
        }

        return true;
    }
};

int main() {
    int command = -1;
    Graph g;
    while (command != 0) {
        cout << "* Data Structures and Algorithms *\n"
             << "* 0. Quit                        *\n"
             << "* 1. Build adjacency lists       *\n"
             << "* 2. Compute connection counts   *\n"
             << "**********************************\n"
             << "Input a command (0, 1, 2): " << flush;
        cin >> command;
        if (command == 0) {
            break;
        } else if (command == 1) {
            g.clear();
            while (g.task1());
        } else if (command == 2) {
            g.task2();
        } else {
            cout << "\nCommand does not exist!\n";
        }
    }
    return 0;
}
