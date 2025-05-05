#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

const int N = 6;
vector<int> graph[N];
vector<bool> visited_bfs(N, false);
vector<bool> visited_dfs(N, false);



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
   
    addEdge(0, 1);
    addEdge(0, 2);
    addEdge(1, 3);
    addEdge(2, 4);
    addEdge(3, 5);

    parallelBFS(0);
    parallelDFS(0);

    return 0;
}
