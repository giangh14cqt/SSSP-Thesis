#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int INF = INT_MAX;  // Set initial distance to infinity

class Graph {
 private:
  int V;                                    // Number of vertices
  vector<vector<pair<int, int> > > adj_list;  // Adjacency list to store the graph
 public:
  Graph(int V) {
    this->V = V;
    adj_list.resize(V);
  }

  void add_edge(int u, int v, int w) {
    adj_list[u].push_back(make_pair(v, w));
    adj_list[v].push_back(
        make_pair(u, w));  // Comment this line for directed graphs
  }

  void dijkstra(int source) {
    vector<int> dist(V, INF);    // Distance from source to each vertex
    vector<bool> vis(V, false);  // Visited flag for each vertex
    dist[source] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int> >,
                   greater<pair<int, int> > >
        pq;
    pq.push(make_pair(0, source));
    while (!pq.empty()) {
      int u = pq.top().second;
      pq.pop();
      if (vis[u]) continue;
      vis[u] = true;
      for (auto iter : adj_list[u]) {
        auto v = iter.first;
        auto w = iter.second;
        if (dist[u] + w < dist[v]) {
          dist[v] = dist[u] + w;
          pq.push(make_pair(dist[v], v));
        }
      }
    }
    for (int i = 0; i < V; i++) {
      cout << "Distance from " << source << " to " << i << ": ";
      if (dist[i] == INF)
        cout << "INF\n";
      else
        cout << dist[i] << '\n';
    }
  }
};

int main() {
  int n, m;  // Number of vertices and edges
  cin >> n >> m;
  Graph g(n);
  for (int i = 0; i < m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    g.add_edge(u, v, w);
  }
  int source;
  cin >> source;
  g.dijkstra(source);
  return 0;
}
