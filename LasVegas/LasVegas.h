#ifndef LasVegas_H
#define LasVegas_H

#include "LDD.hpp"

using namespace std;

// class LasVegas
// {
double startTime;
// double CALCULATE_SCC_PROB = 1;
int NUM_FLAGS = 10;

// flags
int SRC = 0;
bool CHECKS = false;
bool WITH_LDD = false;
double WEIGHT_METHOD = 0;
int MAX_VERTEX = 0;
int RANDOM_SEED = 0;
bool DISPLAY_TREE = false;
bool PRINT_LDD_SIZE = false;
bool MAKE_CONNECTED = false;

// void main();

Graph readInput(ifstream &inputFile)
{
    int g_size;
    int maxWeight;
    inputFile >> g_size >> maxWeight;
    ++g_size;
    // vector<int> phi = createRandomPriceFunction(g_size, maxWeight);

    Graph g(g_size, false);
    vector<vector<int>> edges(g_size);
    vector<vector<int>> weights(g_size);

    vector<vector<bool>> edge_exists(g_size, vector<bool>(g_size, false));
    int u, v, w;
    while (inputFile >> u >> v >> w)
    {
        if (u > g_size || v > g_size)
        {
            cout << "Error: vertex out of bounds" << endl;
            exit(1);
        }
        if (!g.containsVertex[u])
            g.addVertex(u);

        if (!g.containsVertex[v])
            g.addVertex(v);

        if (!edge_exists[u][v])
        {
            edges[u].push_back(v);
            weights[u].push_back(w);
        }
    }
    for (int i = 0; i < g_size; i++)
        g.addEdges(i, edges[i], weights[i]);
    return g;
}

vector<int> createRandomPriceFunction(int g_size, int maxWeight);

void setFlags();

vector<int> getEdgeFromLine(string line);

int getWeight(int weight, int u, int v, vector<int> &phi);

vector<int> getMaxSizeWeight();

Graph getConnectedSubgraph(Graph g);

void findReachable(Graph g, int u, vector<bool> reachable);

void runBellmanFord(Graph g);

vector<int> bitScaling(Graph g);

void verifyTree(Graph g, vector<int> tree, vector<int> dist, int src);

bool containCyle(Graph g, vector<vector<bool>> adjList, int src, vector<bool> visited);

void getDistances(Graph g, vector<int> tree, int curDis, int curVertex, vector<bool> visited);

vector<int> SPmain(Graph g, int src);

Graph getScaledGraph(Graph g_in, int scaleFactor);

bool validTree(Graph g, int s, vector<int> tree);

int roundPower2(int n);

double logBase2(int n);

vector<int> ScaleDown(Graph g, int delta, int B);

vector<vector<int>> SPmainLLD(Graph g, int diameter);

bool hasNegativeEdges(Graph g, vector<int> phi, int B);

Graph createModifiedGB(Graph g, int B, bool nneg, set<vector<int>> remEdges, vector<int> phi);

set<vector<int>> getEdgesBetweenSCCs(Graph g, vector<int> vertexToSCCMap);

vector<int> getVertexToSCCMap(vector<vector<int>> SCCs, int numVertices);

vector<int> addPhi(vector<int> phi_1, vector<int> phi_2);

vector<int> FixDAGEdges(Graph g, vector<vector<int>> SCCs,
                        vector<int> vertexToSCCMap,
                        set<vector<int>> edgesBetweenSCCs);

vector<vector<int>> createSCCAdjList(vector<vector<int>> SCCs,
                                     vector<int> vertexToSCCMap,
                                     set<vector<int>> edgesBetweenSCCs);

vector<int> topSort(int n, vector<vector<int>> adjList);

void topSortUtil(int u, vector<bool> visited, stack<int> &Stack, vector<vector<int>> adjList);

vector<int> ElimNeg(Graph g);

Graph createGs(Graph g);

vector<int> getShortestPathTree(Graph g, int src);

void updateTreeNeighbors(Graph g, int u, vector<int> tree, set<int> settled,
                         priority_queue<Node> pq, vector<int> dist);
// };

#endif // LasVegas_H