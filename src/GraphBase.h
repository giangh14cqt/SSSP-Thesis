#ifndef GRAPHBASE_H
#define GRAPHBASE_H

#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

// Low diameter decomposition implementation

using namespace std;

const int INF = INT_MAX; // Set initial distance to infinity

enum class Weight
{
    IN_LIGHT,
    OUT_LIGHT,
    HEAVY
};

class Graph
{
private:
    int V;
    vector<vector<pair<int, int>>> *adj_list;
    vector<vector<pair<int, int>>> *adj_list_rev;
    vector<int> *active_nodes;

public:
    // Constructor
    Graph(int V)
    {
        this->V = V;
        (*adj_list).resize(V);
        (*adj_list_rev).resize(V);
    }

    Graph(Graph G, vector<int> active_nodes_)
    {
        this->V = G.get_V();
        this->active_nodes = &active_nodes_;
        this->adj_list = G.get_adj_list();
        this->adj_list_rev = G.get_adj_list_rev();
    }

    // Add a directed edge from node u to node v with weight w
    void add_edge(int u, int v, int w)
    {
        (*adj_list)[u].push_back({v, w});
        (*adj_list_rev)[v].push_back({u, w});
    }

    int get_V()
    {
        return V;
    }

    vector<int> *get_active_nodes()
    {
        return active_nodes;
    }

    vector<vector<pair<int, int>>> *get_adj_list()
    {
        return adj_list;
    }

    vector<vector<pair<int, int>>> *get_adj_list_rev()
    {
        return adj_list_rev;
    }

    void remove(vector<int> &ball)
    {
        for (int i = 0; i < ball.size(); i++)
        {
            (*active_nodes)[ball[i]] = 0;
        }
    }
};

#endif // GRAPHBASE_H