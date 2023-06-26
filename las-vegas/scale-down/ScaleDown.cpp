#include "ScaleDown.hpp"

double ScaleDown(Graph &G, int64_t detal, int64_t B)
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
        Graph GB = G.clone();
        GB.doBFilter(B);
        // line 4
        vector<iii> Erem = LDD(GB, d * B);
        // line 5
        // TODO: implement this
        // line 6
        
    }
}