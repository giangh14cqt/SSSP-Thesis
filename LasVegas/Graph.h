#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <map>
#include <stack>
#include <set>
#include <fstream>

using namespace std;

// typedef pair<int, int> Node;
class Node
{
public:
    int node;
    int cost;

    Node(int n, int c)
    {
        node = n;
        cost = c;
    }

    bool operator<(const Node &n) const
    {
        return cost < n.cost;
    }
};

class Graph
{
public:
    int v_max;            // max number of vertices
    vector<int> vertices; // specifies the vertices in the graph
    vector<bool> containsVertex;

    int n;                             // number of vertices
    vector<vector<int>> adjacencyList; // adjacency list of the graph
    vector<vector<int>> weights;       // weights of the edges

    int time; // Used for SCCs by Tarjan's algorithm

    Graph(int numVertices, bool withAllVertices)
    {
        v_max = numVertices;
        vertices = vector<int>();
        containsVertex = vector<bool>(v_max, false);

        if (withAllVertices)
        {
            for (int v = 0; v < numVertices; v++)
            {
                vertices.push_back(v);
                containsVertex[v] = true;
            }
            n = numVertices;
        }
        else
            n = 0;
        adjacencyList = vector<vector<int>>(numVertices, vector<int>());
        weights = vector<vector<int>>(numVertices, vector<int>());

        time = 0;
    }

    void addVertices(vector<int> &vertices)
    {
        for (int v : vertices)
        {
            addVertex(v);
        }
    }

    void addVertex(int v)
    {
        if (0 <= v && v < v_max && !containsVertex[v])
        {
            vertices.push_back(v);
            containsVertex[v] = true;
            n++;
        }
        else
            throw "Vertex out of bounds or already in graph";
    }

    void addEdges(int v, vector<int> &outVertices, vector<int> &outWeights)
    {
        if (outVertices.size() != outWeights.size())
            throw "Number of out vertices and weights must be equal";

        for (int i = 0; i < outVertices.size(); i++)
        {
            adjacencyList[v].push_back(outVertices[i]);
            weights[v].push_back(outWeights[i]);
        }
    }

    void initNullAdjListElts()
    {
        // for (int i=0; i<adjacencyList.size(); i++)
        // {
        //     if (adjacencyList[i].size() == 0)
        //     {
        //         adjacencyList[i].push_back(-1);
        //         weights[i].push_back(-1);
        //     }
        // }
    }

    void displayGraph()
    {
        cout << "Displaying graph:" << endl;
        for (int i = 0; i < v_max; i++)
        {
            if (containsVertex[i])
            {
                cout << "Vertex " << i << " has out edges: ";
                for (int j = 0; j < adjacencyList[i].size(); j++)
                {
                    cout << adjacencyList[i][j] << " with weight " << weights[i][j] << ", ";
                }
                cout << endl;
            }
        }
    }

    // Run Tarjan's algorithm to find SCCs
    // SCC only run on graphs where n == v_max, so needs to create mapping from vertices to [n] first to work
    vector<vector<int>> SCC()
    {
        cout << "Running SCC" << endl;
        vector<int> vertsToN = vector<int>(v_max, 0);
        vector<int> NtoVerts = vector<int>(n, 0);

        for (int i = 0; i < vertices.size(); ++i)
        {
            vertsToN[vertices[i]] = i;
            NtoVerts[i] = vertices[i];
        }

        vector<int> disc = vector<int>(n, -1);
        vector<int> low = vector<int>(n, -1);
        time = 0;

        vector<bool> stackMember = vector<bool>(n, false);
        stack<int> st;

        vector<vector<int>> SCCverts = vector<vector<int>>();

        this->displayGraph();

        for (int i = 0; i < n; ++i)
        {
            if (disc[i] == -1)
            {
                vector<vector<int>> SCCTmp = SCCUtil(i, low, disc, stackMember, st, vertsToN);
                for (vector<int> SCC : SCCTmp)
                    SCCverts.push_back(SCC);
                // SCCUtil(SCCverts, i, low, disc, stackMember, st, vertsToN);
            }
        }
        cout << "Finished SCC" << endl;
        return SCCverts;
    }

    vector<vector<int>> SCCUtil(int u, vector<int> &low, vector<int> &disc, vector<bool> &stackMember, stack<int> &st, vector<int> &vertsToN)
    {
        vector<vector<int>> SCCverts = vector<vector<int>>();
        disc[u] = low[u] = time;
        time++;
        stackMember[u] = true;
        st.push(u);

        for (int v_true : adjacencyList[vertsToN[u]])
        {
            int v = vertsToN[v_true];
            if (disc[v] == -1)
            {
                // SCCverts = SCCUtil(v, low, disc, stackMember, st, vertsToN);
                // SCCUtil(v, low, disc, stackMember, st, vertsToN);
                vector<vector<int>> SCCTmp = SCCUtil(v, low, disc, stackMember, st, vertsToN);
                for (vector<int> SCC : SCCTmp)
                    SCCverts.push_back(SCC);
                low[u] = min(low[u], low[v]);
            }
            else if (stackMember[v])
                low[u] = min(low[u], disc[v]);
        }

        int w = -1;
        if (low[u] == disc[u])
        {
            vector<int> SCC = vector<int>();
            while (w != u)
            {
                w = st.top();
                st.pop();
                SCC.push_back(w);
                stackMember[w] = false;
            }
            SCCverts.push_back(SCC);
        }
        return SCCverts;
    }

    // Returns true if the graph has a negative weight cycle.
    // Assumes every vertex is being used.
    bool hasNegCycle()
    {
        vector<int> vertsToN = vector<int>(v_max, -1);
        vector<int> NtoVerts = vector<int>(n, -1);

        for (int i = 0; i < vertices.size(); ++i)
        {
            vertsToN[vertices[i]] = i;
            NtoVerts[i] = vertices[i];
        }

        vector<bool> visited = vector<bool>(n, false);
        vector<int> dist = vector<int>(n, INT_MAX);

        for (int i = 0; i < n; ++i)
        {
            if (!visited[i])
            {
                if (isNegCycleBellmanFord(i, dist, vertsToN, NtoVerts))
                    return true;
            }

            for (int j = 0; j < n; ++j)
                if (dist[j] != INT_MAX)
                    visited[j] = true;
        }

        return false;
    }

    bool isNegCycleBellmanFord(int src, vector<int> &dist, vector<int> &vertsToN, vector<int> &NtoVerts)
    {
        for (int i = 0; i < n; ++i)
            dist[i] = INT_MAX;
        dist[src] = 0;

        for (int i = 1; i <= n - 1; ++i)
        {
            for (int u = 0; u < n; ++u)
            {
                int u_real = NtoVerts[u];
                for (int j = 0; j < adjacencyList[u_real].size(); ++j)
                {
                    int v_real = adjacencyList[u_real][j];
                    int v = vertsToN[v_real];
                    int weight = weights[u_real][j];
                    if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                        dist[v] = dist[u] + weight;
                }
            }
        }

        for (int u = 0; u < n; ++u)
        {
            int u_real = NtoVerts[u];
            for (int j = 0; j < adjacencyList[u_real].size(); ++j)
            {
                int v_real = adjacencyList[u_real][j];
                int v = vertsToN[v_real];
                int weight = weights[u_real][j];
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                    return true;
            }
        }

        return false;
    }

    void BellmanFord(int src_real)
    {
        vector<int> vertsToN = vector<int>(v_max, -1);
        vector<int> NtoVerts = vector<int>(n, -1);

        for (int i = 0; i < vertices.size(); ++i)
        {
            vertsToN[vertices[i]] = i;
            NtoVerts[i] = vertices[i];
        }

        vector<int> dist = vector<int>(n, INT_MAX);

        int src = vertsToN[src_real];
        dist[src] = 0;

        for (int i = 1; i < n; ++i)
        {
            for (int u = 0; u < n; ++u)
            {
                int u_real = NtoVerts[u];
                for (int j = 0; j < adjacencyList[u_real].size(); ++j)
                {
                    int v_real = adjacencyList[u_real][j];
                    int v = vertsToN[v_real];
                    int weight = weights[u_real][j];
                    if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                        dist[v] = dist[u] + weight;
                }
            }
        }
    }

    bool hasNoNegativeEdgeWeights()
    {
        for (int v : vertices)
        {
            for (int i = 0; i < adjacencyList[v].size(); ++i)
            {
                if (weights[v][i] < 0)
                    return false;
            }
        }
        return true;
    }
};

#endif // GRAPH_H