#ifndef GRAPHBASE_H
#define GRAPHBASE_H

#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

#define ii pair<int, int> // (node, distance_from_source) or (node, weight)
#define iii tuple<int, int, int> // (node_u, node_v, weight)

using namespace std;

const int INF = INT_MAX; // Set initial distance to infinity

enum class Weight
{
    NONE,
    IN_LIGHT,
    OUT_LIGHT,
    HEAVY
};

class Graph
{
private:
    // number of nodes in the (origin) graph
    int V;
    // list of edges in the (origin) graph by adjacency list
    vector<vector<ii>> adj_list;
    // list of reversed edges in the (origin) graph by adjacency list
    vector<vector<ii>> adj_list_rev;
    // list of active nodes in the current graph
    vector<int> active_nodes;

public:
/**
 * @brief Construct a new Graph object
 * @param V The number of nodes in the graph
*/
    Graph(int V)
    {
        this->V = V;
        adj_list.resize(V);
        adj_list_rev.resize(V);
        active_nodes.resize(V, 1);
        for (int i = 0; i < V; i++)
        {
            active_nodes[i] = i;
        }
    }

/**
 * @brief Construct a new Graph object from a given graph and a set of active nodes
 * @param G The origin graph
 * @param active_nodes_ The set of active nodes
 * @return Graph based on the origin graph and the set of active nodes
*/
    Graph(Graph G, vector<int> active_nodes_)
    {
        this->V = G.get_V();
        this->active_nodes = active_nodes_;
        this->adj_list = G.get_adj_list();
        this->adj_list_rev = G.get_adj_list_rev();
    }

/***
 * @brief Add an edge to the graph
 * @param u The node from
 * @param v The node to
 * @param w The weight of the edge
*/
    void add_edge(int u, int v, int w)
    {
        adj_list[u].push_back({v, w});
        adj_list_rev[v].push_back({u, w});
    }

/**
 * @brief Get the number of vertices in the original graph
 * @return The number of vertices in the original graph
*/
    int get_V()
    {
        return V;
    }

/**
 * @brief Get the list of active nodes
*/
    vector<int> get_active_nodes()
    {
        return active_nodes;
    }

/**
 * @brief Get the size of the list of active nodes
*/
    int get_active_size()
    {
        return active_nodes.size();
    }

/**
 * @brief Get the list of edges
*/
    vector<vector<pair<int, int>>> get_adj_list()
    {
        return adj_list;
    }

/**
 * @brief Get the reversed edges
*/
    vector<vector<pair<int, int>>> get_adj_list_rev()
    {
        return adj_list_rev;
    }

/**
 * @brief Remove nodes from the graph by removing them from the list of active nodes
 * @param ball The set of nodes to be removed
*/
    void remove(vector<int> &ball)
    {
        vector<int> new_active_nodes;
        for (int i = 0; i < active_nodes.size(); i++)
        {
            if (binary_search(ball.begin(), ball.end(), active_nodes[i]))
                continue;
            new_active_nodes.push_back(active_nodes[i]);
        }
        active_nodes = new_active_nodes;
    }

/**
 * @brief Check if a node is active
 * @param v The node to be checked
 * @return True if the node is active, false otherwise
*/
    bool is_active(int v)
    {
        return binary_search(active_nodes.begin(), active_nodes.end(), v);
    }

/**
 * @brief Clone a graph
 * @return A clone of the graph
*/
    Graph clone()
    {
        Graph G(V);
        G.adj_list = adj_list;
        G.adj_list_rev = adj_list_rev;
        G.active_nodes = active_nodes;
        return G;
    }

/**
 * @brief Get the list of edges as a vector of tuples (u, v, w) - from node u to node v with weight w
 * @return The list of edges as a vector of tuples (u, v, w)
*/
    vector<iii> get_adj_list_as_iii()
    {
        vector<iii> adj_list_as_iii;
        for (int u = 0; u < V; u++)
        {
            if (!is_active(u))
                continue;
            for (auto [v, w] : adj_list[u])
            {
                if (!is_active(v) || v > u)
                    continue;
                adj_list_as_iii.push_back({u, v, w});
            }
        }
        return adj_list_as_iii;
    }
};

#endif // GRAPHBASE_H