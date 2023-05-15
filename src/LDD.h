#include "GraphBase.h"

const int C = 100; // constant for LDD

// Randomized algorithm low diameter decomposition with radius r, return the list of Erem
vector<vector<pair<int, int>>> LDD(Graph G, int d)
{
    int n = G.get_V();
    int k = C * log1p(n);
    std::srand(std::time(nullptr)); // seed the random number generator
    vector<vector<pair<int, int>>> erem;
    vector<int> S;
    vector<Weight> weight(n, Weight::IN_LIGHT);
    vector<int> *active_nodes = G.get_active_nodes();
    // Parse 1: mark vertices as light or heavy
    // Randomly select k vertices from V
    for (int i = 0; i < k; i++)
    {
        int v = rand() % (*active_nodes).size();
        S.push_back((*active_nodes)[v]);
    }
    // for each v in V, compute Ball-in(v, d/4) and Ball-out(v, d/4)
    vector<vector<int>> ball_in(n, vector<int>());
    vector<vector<int>> ball_out(n, vector<int>());
    for (int i : *active_nodes)
    {
        ball_in[i] = computer_ball_in(G, S[i], d / 4);
        ball_out[i] = computer_ball_out(G, S[i], d / 4);
    }
    // for each v in V, compute Ball-in(v, d/4) cross S and Ball-out(v, d/4) cross S
    vector<vector<int>> ball_in_cross(n, vector<int>());
    vector<vector<int>> ball_out_cross(n, vector<int>());
    for (int i : *active_nodes)
    {
        ball_in_cross[i] = get_cross(ball_in[i], S);
        ball_out_cross[i] = get_cross(ball_out[i], S);
    }
    // mark in-light and out-light and heavy vertices
    for (int i : *active_nodes)
    {
        if (ball_in_cross[i].size() <= 6 * k)
        {
            weight[i] = Weight::IN_LIGHT;
        }
        else if (ball_out_cross[i].size() <= 6 * k)
        {
            weight[i] = Weight::OUT_LIGHT;
        }
        else
        {
            weight[i] = Weight::HEAVY;
        }
    }
    // Parse 2: carve out balls until no light vertices remain

    double p = min(1.0, 80 * log2(n) / k);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::geometric_distribution<int> distr(p);
    int V = active_nodes->size();
    int v = contain_light(weight);

    while (v != -1)
    {
        double rv = distr(gen);
        vector<int> ball_in_rv = computer_ball_in(G, v, rv);
        vector<int> ball_out_rv = computer_ball_out(G, v, rv);

        vector<vector<pair<int, int>>> e_boundary = get_boundary(G, ball_in_rv, ball_out_rv);

        if (rv > d / 4.0 || ball_in_rv.size() > 7 * V || ball_out_rv.size() > 7 * V)
            return *G.get_adj_list();

        vector<vector<pair<int, int>>> e_recurse;
        Graph G_ball = Graph(G, get_cross(ball_in_rv, ball_out_rv));
        e_recurse = LDD(G_ball, d);

        v = contain_light(weight);

        erem = get_cross(erem, e_boundary, e_recurse);

        G.remove(ball_in_rv);
        G.remove(ball_out_rv);
    }

    // TODO: line 19-20

    return erem;
}

vector<vector<pair<int, int>>> get_cross(vector<vector<pair<int, int>>> &a, vector<vector<pair<int, int>>> b, vector<vector<pair<int, int>>> c)
{
    a.insert(a.end(), b.begin(), b.end());
    a.insert(a.end(), c.begin(), c.end());
    sort(a.begin(), a.end());
    vector<vector<pair<int, int>>>::iterator it;
    it = unique(a.begin(), a.end());
    a.resize(distance(a.begin(), it));
}

vector<vector<pair<int, int>>> get_boundary(Graph &G, vector<int> ball_in, vector<int> ball_out)
{
    vector<vector<pair<int, int>>> boundary(G.get_V(), vector<pair<int, int>>());
    for (int u : *G.get_active_nodes())
    {
        for (auto inter : (*G.get_adj_list())[u])
        {
            auto v = inter.first;
            auto w = inter.second;
            if (find(ball_out.begin(), ball_out.end(), u) != ball_out.end() && find(ball_out.begin(), ball_out.end(), v) == ball_out.end())
            {
                boundary[u].push_back(make_pair(v, w));
            }
            else if (find(ball_in.begin(), ball_in.end(), u) == ball_in.end() && find(ball_in.begin(), ball_in.end(), v) != ball_in.end())
            {
                boundary[u].push_back(make_pair(v, w));
            }
        }
    }
    return boundary;
}

int contain_light(vector<Weight> &weight)
{
    for (int i = 0; i < weight.size(); i++)
        if (weight[i] == Weight::IN_LIGHT || weight[i] == Weight::OUT_LIGHT)
            return i;
    return -1;
}

vector<int> get_cross(vector<int> a, vector<int> b)
{
    a.insert(a.end(), b.begin(), b.end());
    sort(a.begin(), a.end());
    vector<int>::iterator it;
    it = unique(a.begin(), a.end());
    a.resize(distance(a.begin(), it));
    return a;
}

vector<int> computer_ball_in(Graph &G, int s, int d)
{
    int n = G.get_V();
    vector<vector<pair<int, int>>> *adj_list = G.get_adj_list();
    vector<int> ball_in;
    vector<int> dist(n, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[s] = 0;
    pq.push(make_pair(0, s));
    while (!pq.empty())
    {
        int w = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (dist[u] > d)
            break;
        ball_in.push_back(u);
        for (auto iter : (*adj_list)[u])
        {
            auto v = iter.first;
            if (find(*G.get_active_nodes()->begin(),
                     *G.get_active_nodes()->end(), v) == *G.get_active_nodes()->end())
                continue;
            auto w = iter.second;
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    return ball_in;
}

vector<int> computer_ball_out(Graph &G, int s, int d)
{
    int n = G.get_V();
    vector<vector<pair<int, int>>> *adj_list_rev = G.get_adj_list_rev();
    vector<int> ball_out;
    vector<int> dist(n, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[s] = 0;
    pq.push(make_pair(0, s));
    while (!pq.empty())
    {
        int w = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (dist[u] > d)
            break;
        ball_out.push_back(u);
        for (auto iter : (*adj_list_rev)[u])
        {
            auto v = iter.first;
            if (find(*G.get_active_nodes()->begin(),
                     *G.get_active_nodes()->end(), v) == *G.get_active_nodes()->end())
                continue;
            auto w = iter.second;
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    return ball_out;
}