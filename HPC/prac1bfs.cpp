#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

void sequentialbfs(vector<vector<int>> &graph, int start) {
    int nodes = graph.size();
    vector<bool> visited(nodes, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        cout << current << ",";
        for (auto neighbour : graph[current]) {
            if (!visited[neighbour]) {
                visited[neighbour] = true;
                q.push(neighbour);
            }
        }
    }
}

void parallelbfs(vector<vector<int>> &graph, int start) {
    int nodes = graph.size();
    vector<bool> visited(nodes, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        #pragma omp parallel shared(q, visited) // Parallelize the loop
        {
            #pragma omp for schedule(dynamic) // Dynamic scheduling for load balancing
            for (int i = 0; i < q.size(); ++i) {
                int current;
                #pragma omp critical // Critical section to access and update the queue
                {
                    current = q.front();
                    q.pop();
                }
                cout << current << ","; // Print node

                // Iterate over neighbors of the current node
                for (auto neighbour : graph[current]) {
                    #pragma omp critical // Critical section to access and update visited and queue
                    {
                        if (!visited[neighbour]) {
                            visited[neighbour] = true;
                            q.push(neighbour);
                        }
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
    cout << "\nSequential BFS: ";
    sequentialbfs(graph, 0);
    double end = omp_get_wtime();
    double diff = end - start ;
    cout << "\nTime : " << diff ;
    
    cout<<"\n" ;

    start = omp_get_wtime();
    cout << "\nParallel BFS: ";
    parallelbfs(graph, 0);
    end = omp_get_wtime();
    diff = end - start ;
    cout << "\nTime : " << diff ;

    return 0;
}
