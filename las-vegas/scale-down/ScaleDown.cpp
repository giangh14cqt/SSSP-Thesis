#include "ScaleDown.hpp"

vector<int> ScaleDown(Graph &G, int64_t detal, int64_t B)
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
        Graph GSCCs = GB.removeEdges(Erem);
        vector<vector<int>> V = GSCCs.get_SCCs();
        // line 6
        Graph H = GSCCs.get_graph_inside_SCCs(V);
        // line 7
        vector<int> phi1 = ScaleDown(GB, d, B);

    }
}