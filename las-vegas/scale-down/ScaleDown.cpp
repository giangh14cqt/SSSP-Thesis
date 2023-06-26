#include "ScaleDown.hpp"

vector<int> ScaleDown(Graph &G, int64_t detal, int64_t B)
{
    // Set up
    vector<int> phi2(G.get_V(), 0);
    // line 3
    int64_t d = detal / 2;
    Graph GB = G.clone();
    GB.doBFilter(B);
    // line 4
    vector<iii> Erem = LDD(GB, d * B);
    // line 5
    Graph GSCCs = GB.remove_edges(Erem);
    vector<vector<int>> V = GSCCs.get_SCCs();
    // line 6
    Graph H = GSCCs.get_graph_inside_SCCs(V);
    // line 1, 2
    if (detal > 2)
    {
        // line 7
        vector<int> phi1 = ScaleDown(H, detal / 2, B);
        // line 8
        Graph GBPhi1 = GSCCs.do_phi_filter(phi1);
        vector<int> psi = FixDAGEdges(GBPhi1, V);
        // line 9
        for (int i = 0; i < phi2.size(); ++i)
            phi2[i] = phi1[i] + psi[i];
    }
    // line 10
    Graph GBPhi2 = GSCCs.do_phi_filter(phi2);
    vector<int> psi_prime = ElimNeg(GBPhi2);
    // line 11
    for (int i = 0; i < phi2.size(); ++i)
        phi2[i] += psi_prime[i];
    // line 12
    return phi2;
}