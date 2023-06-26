#include "FixDAGEdges.hpp"

vector<int> FixDAGEdges(Graph &G, vector<vector<int>> &P_)
{
    // line 1: relabel
    vector<vector<int>> P;
    Graph SCCs(G, P_);
    vector<int> topoOrder = SCCs.topological_sort();
    for (auto u : topoOrder)
        P.push_back(P_[u]);
    // line 2: define mu
    map<int, int> comp_map;
    for (int i = 0; i < P.size(); i++)
        for (auto u : P[i])
            comp_map[u] = i;
    vector<int> mu(G.get_V(), INF);
    for (auto u : G.get_active_nodes())
    {
        int i = comp_map[u];
        for (auto [v, w] : G.get_adj_list()[u])
        {
            int j = comp_map[v];
            if (!G.is_active(v) || w >= 0 || i == j)
                continue;
            mu[j] = min(mu[j], w);
        }
    }
    for (int i = 0; i < mu.size(); ++i)
        if (mu[i] == INF)
            mu[i] = 0;
    // line 3: define M
    vector<int> M(P.size(), 0);
    M[0] = mu[0];
    // line 4: loop
    vector<int> phi(G.get_V(), -1);
    for (auto u : P[0])
        phi[u] = M[0];
    for (int i = 1; i < P.size(); ++i)
    {
        M[i] = M[i - 1] + mu[i];
        for (auto u : P[i])
            phi[u] = M[i];
    }
    // line 7: return
    return phi;
}