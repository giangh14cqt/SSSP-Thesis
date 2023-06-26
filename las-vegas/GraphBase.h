#ifndef GRAPHBASE_H
#define GRAPHBASE_H

#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <map>
#include <stack>

#define ii pair<int, int>        // (node, distance_from_source) or (node, weight)
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
    Graph(Graph &G, vector<int> active_nodes_)
    {
        this->V = G.get_V();
        this->active_nodes = active_nodes_;
        adj_list.resize(V);
        adj_list_rev.resize(V);
        // this->adj_list = G.get_adj_list();
        // this->adj_list_rev = G.get_adj_list_rev();
        for (auto u : active_nodes)
        {
            for (auto [v, w] : adj_list[u])
            {
                if (!is_active(v))
                    continue;
                add_edge(u, v, w);
            }
        }
    }

    /**
     * @brief Construct a new Graph based on a given graph and set of Components
     * @param G The origin graph
     * @param P The set of Components
     * @return The SCCs graph with SCC as nodes and labled as the index of the component in P
     */
    Graph(Graph &G, vector<vector<int>> &P)
    {
        V = P.size();
        adj_list.resize(V);
        adj_list_rev.resize(V);
        active_nodes.resize(P.size(), 1);
        for (int i = 0; i < P.size(); i++)
        {
            active_nodes[i] = i;
        }
        map<int, map<int, bool>> isAdded;
        map<int, int> comp_map;
        for (int i = 0; i < P.size(); i++)
            for (auto u : P[i])
                comp_map[u] = i;

        int u_comp, v_comp;
        for (auto u : G.get_active_nodes())
        {
            u_comp = comp_map[u];
            for (auto [v, w] : G.get_adj_list()[u])
            {
                if (!G.is_active(v))
                    continue;
                v_comp = comp_map[v];
                if (u_comp != v_comp && !isAdded[u_comp][v_comp])
                {
                    add_edge(u_comp, v_comp, 0);
                    isAdded[u_comp][v_comp] = true;
                }
            }
        }
    }

    /**
     * @brief Get the strongly connected components of the graph using Kosaraju's algorithm
     * @return The list of strongly connected components as a list of super nodes - each super node is a list of nodes
     */
    vector<vector<int>> get_SCCs()
    {
        vector<vector<int>> SCCs;
        vector<int> SCC;
        vector<bool> visited(V, false);
        stack<int> S;
        this->remove_redundant_edges();
        for (auto head : active_nodes)
        {
            if (visited[head])
                continue;
            kosaraju_dfs_first(head, visited, S);
        }
        visited.assign(V, false);
        while (!S.empty())
        {
            int u = S.top();
            S.pop();
            if (visited[u])
                continue;
            kosaraju_dfs_second(u, visited, SCC);
            sort(SCC.begin(), SCC.end());
            SCCs.push_back(SCC);
            SCC.clear();
        }
        return SCCs;
    }

    void kosaraju_dfs_second(int u, vector<bool> &visited, vector<int> &SCC)
    {
        visited[u] = true;
        SCC.push_back(u);
        for (auto [v, w] : adj_list_rev[u])
        {
            if (!visited[v])
                kosaraju_dfs_second(v, visited, SCC);
        }
    }

    void kosaraju_dfs_first(int u, vector<bool> &visited, stack<int> &S)
    {
        visited[u] = true;
        for (auto [v, w] : adj_list[u])
        {
            if (!visited[v])
                kosaraju_dfs_first(v, visited, S);
        }
        S.push(u);
    }

    /***
     * @brief Remove all edges that are not active
     */
    void remove_redundant_edges()
    {
        for (int u = 0; u < V; u++)
        {
            vector<ii> new_adj;
            if (is_active(u))
            {
                for (auto [v, w] : adj_list[u])
                {
                    if (!is_active(v))
                        continue;
                    new_adj.push_back({v, w});
                }
            }
            adj_list[u] = new_adj;
        }
        adj_list_rev.clear();
        adj_list_rev.resize(V);
        for (auto u : active_nodes)
            for (auto [v, w] : adj_list[u])
                adj_list_rev[v].push_back({u, w});
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
        for (auto u : active_nodes)
        {
            if (binary_search(ball.begin(), ball.end(), u))
                continue;
            new_active_nodes.push_back(u);
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
     * @brief Clone a graph with a set of active nodes only
     * @return A clone of the graph
     */
    Graph clone()
    {
        Graph G(V);
        G.active_nodes = active_nodes;
        // G.adj_list = adj_list;
        // G.adj_list_rev = adj_list_rev;
        for (auto u : active_nodes)
        {
            for (auto [v, w] : adj_list[u])
            {
                if (!is_active(v))
                    continue;
                G.add_edge(u, v, w);
            }
        }
        return G;
    }

    /**
     * @brief Get the list of edges as a vector of tuples (u, v, w) - from node u to node v with weight w
     * @return The list of edges as a vector of tuples (u, v, w)
     */
    vector<iii> get_adj_list_as_iii()
    {
        vector<iii> adj_list_as_iii;
        // for (int u = 0; u < V; u++)
        for (auto u : active_nodes)
        {
            for (auto [v, w] : adj_list[u])
            {
                if (!is_active(v))
                    continue;
                adj_list_as_iii.push_back({u, v, w});
            }
        }
        return adj_list_as_iii;
    }

    /**
     * @brief Apply the B-filter to the graph with get max edge with 0
     */
    void doBFilter(int64_t B)
    {
        int w;
        for (auto u : active_nodes)
        {
            for (int i = 0; i < adj_list[u].size(); i++)
            {
                w = adj_list[u][i].second;
                if (w < 0)
                    w += B;
                w = max(w, 0);
                adj_list[u][i].second = w;
            }
        }
    }

    /**
     * @brief Get the topological order of the graph
     * @return The topological order of the graph
     */
    vector<int> topological_sort()
    {
        vector<int> in_degree(V, 0);
        for (auto u : active_nodes)
        {
            for (auto [v, w] : adj_list[u])
            {
                if (!is_active(v))
                    continue;
                in_degree[v]++;
            }
        }
        queue<int> q;
        for (auto u : active_nodes)
        {
            if (in_degree[u] == 0)
                q.push(u);
        }
        vector<int> topo_order;
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            topo_order.push_back(u);
            for (auto [v, w] : adj_list[u])
            {
                if (!is_active(v))
                    continue;
                in_degree[v]--;
                if (in_degree[v] == 0)
                    q.push(v);
            }
        }
        return topo_order;
    }

    /**
     * @brief Get a Graph based on a given graph by removing a set of edges
     * @param G The origin graph
     * @param P The set of edges to be removed
     * @return The SCCs graph with SCC as nodes and labled as the index of the component in P
     */
    Graph removeEdges(vector<iii> &Erem)
    {
        Graph G(V);
        G.active_nodes = active_nodes;
        for (auto u : active_nodes)
        {
            for (auto [v, w] : adj_list[u])
            {
                if (!is_active(v))
                    continue;
                if (binary_search(Erem.begin(), Erem.end(), make_tuple(u, v, w)))
                    continue;
                G.add_edge(u, v, w);
            }
        }
        return G;
    }

    /**
     * @brief Get a Graph based on a given graph with edges that only inside each SCC
     * @param SCCs The list of SCCs
     * @return The graph with edges that only inside each SCC
     */
    Graph get_graph_inside_SCCs(vector<vector<int>> SCCs)
    {
        Graph G(V);
        G.active_nodes = active_nodes;
        for (int i = 0; i < SCCs.size(); ++i)
        {
            for (auto u : SCCs[i])
            {
                for (auto [v, w] : adj_list[u])
                {
                    if (!is_active(v))
                        continue;
                    if (binary_search(SCCs[i].begin(), SCCs[i].end(), v))
                        G.add_edge(u, v, w);
                }
            }
        }
        return G;
    }
};

#endif // GRAPHBASE_H