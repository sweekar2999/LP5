#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

vector<vector<int>> graph;
vector<bool> visited_bfs;
vector<bool> visited_dfs;

void addEdge(int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

void parallelBFS(int start) {
    queue<int> q;
    visited_bfs[start] = true;
    q.push(start);

    cout << "Parallel BFS: ";

    while (!q.empty()) {
        int levelSize = q.size();

        #pragma omp parallel for
        for (int i = 0; i < levelSize; ++i) {
            int node;
            #pragma omp critical
            {
                node = q.front();
                q.pop();
                cout << node << " ";
            }

            for (int neighbor : graph[node]) {
                if (!visited_bfs[neighbor]) {
                    #pragma omp critical
                    {
                        if (!visited_bfs[neighbor]) {
                            visited_bfs[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
    }
    cout << endl;
}

void parallelDFS(int start) {
    stack<int> s;
    visited_dfs[start] = true;
    s.push(start);

    cout << "Parallel DFS: ";

    while (!s.empty()) {
        int node;
        #pragma omp critical
        {
            node = s.top();
            s.pop();
            cout << node << " ";
        }

        #pragma omp parallel for
        for (int i = 0; i < graph[node].size(); ++i) {
            int neighbor = graph[node][i];
            if (!visited_dfs[neighbor]) {
                #pragma omp critical
                {
                    if (!visited_dfs[neighbor]) {
                        visited_dfs[neighbor] = true;
                        s.push(neighbor);
                    }
                }
            }
        }
    }
    cout << endl;
}

int main() {
    int nodes, edges;
    cout << "Enter number of nodes: ";
    cin >> nodes;

    cout << "Enter number of edges: ";
    cin >> edges;

    graph.resize(nodes);
    visited_bfs.assign(nodes, false);
    visited_dfs.assign(nodes, false);

    cout << "Enter each edge (u v):" << endl;
    for (int i = 0; i < edges; ++i) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
    }

    int startNode;
    cout << "Enter starting node for traversal: ";
    cin >> startNode;

    parallelBFS(startNode);
    parallelDFS(startNode);

    return 0;
}
