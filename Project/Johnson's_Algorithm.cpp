#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

// Structure to represent an edge
struct Edge {
    int u, v, weight;
};

// Bellman-Ford Algorithm to compute shortest path from source node
bool BellmanFord(const vector<Edge>& edges, int V, int source, vector<int>& dist) {
    dist[source] = 0;
    
    // Relax edges |V| - 1 times
    for (int i = 1; i < V; ++i) {
        for (const auto& edge : edges) {
            if (dist[edge.u] != INT_MAX && dist[edge.u] + edge.weight < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.weight;
            }
        }
    }

    // Check for negative weight cycles
    for (const auto& edge : edges) {
        if (dist[edge.u] != INT_MAX && dist[edge.u] + edge.weight < dist[edge.v]) {
            return false; // Negative cycle detected
        }
    }

    return true; // No negative cycle
}

// Dijkstra's Algorithm to find the shortest path from a source node
void Dijkstra(const vector<vector<int>>& adj, int V, int source, vector<int>& dist) {
    dist[source] = 0;
    vector<bool> visited(V, false);
    
    for (int i = 0; i < V - 1; ++i) {
        int u = -1;
        
        // Find the vertex with the smallest distance value
        for (int j = 0; j < V; ++j) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        visited[u] = true;

        // Update the distance values of the adjacent vertices
        for (int v = 0; v < V; ++v) {
            if (adj[u][v] != INT_MAX && dist[u] != INT_MAX && dist[u] + adj[u][v] < dist[v]) {
                dist[v] = dist[u] + adj[u][v];
            }
        }
    }
}

// Johnson's Algorithm for All-Pairs Shortest Path
vector<vector<int>> Johnson(int V, const vector<Edge>& edges) {
    vector<vector<int>> result(V, vector<int>(V, INT_MAX));

    // Add a new node to the graph
    vector<Edge> modifiedEdges = edges;
    for (int i = 0; i < V; ++i) {
        modifiedEdges.push_back({V, i, 0}); // Connect the new node to all other nodes with weight 0
    }

    vector<int> h(V + 1, INT_MAX);
    if (!BellmanFord(modifiedEdges, V + 1, V, h)) {
        cout << "Graph contains negative weight cycle!" << endl;
        return result; // If there's a negative weight cycle, return empty result
    }

    // Re-weight the graph
    vector<vector<int>> adj(V, vector<int>(V, INT_MAX));
    for (const auto& edge : edges) {
        adj[edge.u][edge.v] = edge.weight + h[edge.u] - h[edge.v];
    }

    // Run Dijkstra's Algorithm for each vertex
    for (int i = 0; i < V; ++i) {
        vector<int> dist(V, INT_MAX);
        Dijkstra(adj, V, i, dist);

        // Restore the distances by adding h[v] - h[u]
        for (int j = 0; j < V; ++j) {
            if (dist[j] != INT_MAX) {
                result[i][j] = dist[j] + h[j] - h[i];
            }
        }
    }

    return result;
}

int main() {
    // Number of vertices in the graph
    int V = 4;

    // List of edges (u, v, weight)
    vector<Edge> edges = {
        {0, 1, 2}, {1, 2, -5},
        {2, 3, 3}, {3, 0, 4}
    };

    // Run Johnson's Algorithm
    vector<vector<int>> shortestPaths = Johnson(V, edges);

    // Print the shortest path matrix
    cout << "Shortest paths between every pair of vertices:" << endl;
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            if (shortestPaths[i][j] == INT_MAX)
                cout << "INF ";
            else
                cout << shortestPaths[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}

