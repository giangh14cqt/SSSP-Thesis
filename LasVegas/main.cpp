#include "LasVegas.h"

int main()
{
    Random::Get().Seed();
    ifstream inputFile("input.txt");
    Graph g = readInput(inputFile);
    g.displayGraph();
    vector<vector<int>> G_LDD = LDD(g, 200);
    cout << "LDD size: " << G_LDD.size() << endl;
    for (int i = 0; i < G_LDD.size(); i++)
    {
        cout << G_LDD[i][0] << " " << G_LDD[i][1] << endl;
    }
}