#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

// Sequential Depth First Search
void sequentialDFS(vector<vector<int>>& graph, int start) {
    int V = graph.size();
    vector<bool> visited(V, false);
    stack<int> stk;

    stk.push(start);
    while (!stk.empty()) {
        int node = stk.top();
        stk.pop();
        if (!visited[node]) {
            cout << node << " ";
            visited[node] = true;
            for (int i = graph[node].size() - 1; i >= 0; --i) {
                int neighbor = graph[node][i];
                if (!visited[neighbor]) {
                    stk.push(neighbor);
                }
            }
        }
    }
}

// Parallel Depth First Search
void parallelDFS(vector<vector<int>>& graph, int start) {
    int V = graph.size();
    vector<bool> visited(V, false);
    stack<int> stk;

    stk.push(start);
    while (!stk.empty()) {
        int current;
        #pragma omp critical
        {
            current = stk.top();
            stk.pop();
        }
        if (!visited[current]) {
            cout << current << " ";
            visited[current] = true;
            #pragma omp parallel for

            for (auto neighbor : graph[current]) {
                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        stk.push(neighbor);
                    }
                }
            }
        }
    }
}

int main() {
    vector<vector<int>> graph(6);
      graph[0] = {1, 2};
  graph[1] = {0, 3, 4};
  graph[2] = {0, 4};
  graph[3] = {1, 5};
  graph[4] = {1, 2, 5};
  graph[5] = {3, 4};

    double start = omp_get_wtime();
    cout << "Sequential DFS starting from node 0: ";
    sequentialDFS(graph, 0);
    double end = omp_get_wtime();
    double diff = end - start ;
    cout << "\nTime : " << diff ;
    
    cout<<"\n" ;
    start = omp_get_wtime();
    cout << "Parallel DFS starting from node 0: ";
    parallelDFS(graph, 0);
    end = omp_get_wtime();
    diff = end - start ;
    cout << "\nTime : " << diff ;

    return 0;
}