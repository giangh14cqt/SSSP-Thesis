#include "Dijkstra.h"
#include <queue>
#include <limits>

// Constructor
Graph::Graph(int numNodes) {
    this->V = numNodes;
    adj_list.resize(numNodes);
}

// Destructor
Graph::~Graph() {
    delete &adj_list;
    delete &dist;
}

// Add a directed edge from node u to node v with weight w
void Graph::add_edge(int u, int v, int w) {
    adj_list[u].push_back({v, w});
}

// Get the shortest paths from a source node to all other nodes using Dijkstra's algorithm
void Graph::dijkstra(int source) {
    // Initialize the distance vector with infinity for all nodes except the source
    dist.assign(V, std::numeric_limits<int>::max());
    dist[source] = 0;

    // Create a priority queue to store nodes and their tentative distances
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push({0, source});

    // Traverse the graph using Dijkstra's algorithm
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        // Check if the current node has already been processed
        if (dist[u] != std::numeric_limits<int>::max()) {
            continue;
        }

        // Update the tentative distances of the neighbors of the current node
        for (auto neighbor : adj_list[u]) {
            int v = neighbor.first;
            int w = neighbor.second;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

// Return shortest distace to a vertex
int Graph::get_shortest_distance(int vertex) {
    return dist[vertex];
}
