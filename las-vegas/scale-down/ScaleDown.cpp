#include "ScaleDown.hpp"

Graph ScaleDown(Graph &G, int64_t detal, int64_t B)
{
    // Set up
    vector<int> phi(G.get_V(), -1);
    // line 1
    if (detal <= 2)
    {
        // line 2
        phi[2] = 0;
    }
    else 
    {
        // line 3
        int64_t d = detal / 2;
        Graph GB = G.BFilter(B);
    }
}