#include <iostream>
#include <fstream>
#include "LDD.hpp"

using namespace std;

int main(int argc, char *args[])
{
    ifstream infile(args[1]);
    ofstream outfile("output.txt");
    int n, m;
    infile >> n >> m;
    Graph G(n);
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        infile >> u >> v >> w;
        G.add_edge(u, v, w);
    }
    int d = 200;
    vector<iii> ldd = LDD(G, d);
    for (auto tup : ldd)
    {
        outfile << get<0>(tup) << " " << get<1>(tup) << " " << get<2>(tup) << endl;
    }
}