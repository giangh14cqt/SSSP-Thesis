# Single Source Shortest Path Algorithms

This repository contains implementations of three popular Single Source Shortest Path (SSSP) algorithms and a new randomized algorithm: Dijkstra's algorithm, Bellman-Ford algorithm, Floyd-Warshall algorithm and Las Vegas algorithm. These algorithms are commonly used in graph theory and have many applications in various fields, such as transportation networks, social networks, and computer networks.

## Getting Started

To use these algorithms, simply clone the repository or download the source code. The algorithms are implemented in C++ and can be compiled using any C++ compiler.

## Usage

To use the algorithms, create a graph object and add edges to it using the `add_edge` function. Then, call the algorithm function, passing in the source vertex as the parameter. The shortest distance to each vertex can be obtained using the `get_shortest_distance` function.

For example, to use Dijkstra's algorithm:

```c++
#include "dijkstra.h"

int main() {
  Graph g(5);
  g.add_edge(0, 1, 10);
  g.add_edge(0, 2, 5);
  g.add_edge(1, 2, 2);
  g.add_edge(1, 3, 1);
  g.add_edge(2, 1, 3);
  g.add_edge(2, 3, 9);
  g.add_edge(2, 4, 2);
  g.add_edge(3, 4, 4);

  g.dijkstra(0);

  cout << g.get_shortest_distance(4) << endl; // Output: 9
  return 0;
}
```