#include "FixDAGEdges.hpp"

int64_t FixDAGEdges(Graph &G, vector<vector<int>> &P_)
{
    // line 1: relabel
    vector<vector<int>> P;
    Graph SCCs(G, P_);
    vector<int> topoOrder = SCCs.topological_sort();
    for (auto u : topoOrder)
        P.push_back(P_[u]);
    // line 2: define mu
    vector<int> mu(G.get_V());
    for (int i = 0; i < G.get_V(); i++)
    {
        mu[i] = 
    }
}