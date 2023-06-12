#include "LDD.hpp"

// Randomized algorithm low diameter decomposition with radius r, return the list of Erem
// The implementation is based on 2203.03456 page 16
vector<iii> LDD(Graph &G, int d)
{
    // Set up
    std::srand(std::time(nullptr)); // seed the random number generator
    // line 1
    int n = G.get_V();
    int nodes_num = G.get_active_nodes().size();
    vector<Weight> weight(n, Weight::NONE);
    // line 2
    vector<iii> erem;
    Graph G0 = G.clone();
    // line 3
    int k = C * log1p(n);
    // line 4
    vector<int> active_nodes = G.get_active_nodes();
    vector<int> S;
    for (int i = 0; i < k; i++)
    {
        int v = rand() % active_nodes.size();
        S.push_back(active_nodes[v]);
    }
    sort(S.begin(), S.end());
    // unique S
    S.erase(unique(S.begin(), S.end()), S.end());

    // line 5
    vector<vector<int>> ball_in(n, vector<int>());
    vector<vector<int>> ball_out(n, vector<int>());
    for (int si : S)
    {
        ball_in[si] = computer_ball_in(G, si, d / 4);
        ball_out[si] = computer_ball_out(G, si, d / 4);
    }
    // line 6
    vector<vector<int>> ball_in_cross(n, vector<int>());
    vector<vector<int>> ball_out_cross(n, vector<int>());
    for (int v : active_nodes)
    {
        ball_in_cross[v] = get_cross(computer_ball_in(G, v, d / 4), S);
        ball_out_cross[v] = get_cross(computer_ball_out(G, v, d / 4), S);
    }
    // line 7
    for (int v : active_nodes)
    {
        // line 8
        // if (ball_in_cross[v].size() <= 0.6 * k)
        if (ball_in_cross[v].size() <= 0.7 * nodes_num)
        {
            weight[v] = Weight::IN_LIGHT;
        }
        // line 9
        // else if (ball_out_cross[v].size() <= 0.6 * k)
        else if (ball_out_cross[v].size() <= 0.7 * nodes_num)
        {
            weight[v] = Weight::OUT_LIGHT;
        }
        // line 10
        else
        {
            weight[v] = Weight::HEAVY;
        }
    }
    // line 11
    double p = min(1.0, 80 * log2(n) / d);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::geometric_distribution<int> distr(p);
    int v = contain_light(weight, G);

    while (v != -1)
    {
        // line 12
        double rv = distr(gen);
        // line 13
        vector<int> ball_in_rv = computer_ball_in(G, v, rv);
        vector<int> ball_out_rv = computer_ball_out(G, v, rv);
        // line 14
        vector<iii> e_boundary;
        e_boundary = get_boundary(G, ball_in_rv, ball_out_rv);
        // line 15
        if (rv > d / 4.0 ||
            ball_in_rv.size() > 0.7 * G.get_active_nodes().size())
        {
            vector<iii> res = G.get_adj_list_as_iii();
            // todo
            return erem;
        }
        // line 16
        vector<iii> e_recurse;
        Graph G_ball = Graph(G, get_cross(ball_in_rv, ball_out_rv));

        e_recurse = LDD(G_ball, d);
        // line 17
        get_cross_e(erem, e_boundary);
        get_cross_e(erem, e_recurse);
        // line 18
        G.remove(ball_in_rv);
        G.remove(ball_out_rv);

        v = contain_light(weight, G);
    }
    // line 19
    v = G.get_active_nodes()[rand() % G.get_active_nodes().size()];
    // line 20
    vector<int> ball_in_g0 = computer_ball_in(G0, v, d / 2);
    vector<int> ball_out_g0 = computer_ball_out(G0, v, d / 2);
    if (!is_sub_set(ball_in_g0, G.get_active_nodes()) ||
        !is_sub_set(ball_out_g0, G.get_active_nodes()))
    {
        return G.get_adj_list_as_iii();
    }
    return erem;
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


void get_cross_e(vector<iii> &erem, vector<iii> e_other)
{
    erem.insert(erem.end(), e_other.begin(), e_other.end());
    sort(erem.begin(), erem.end());
    vector<iii>::iterator it;
    it = unique(erem.begin(), erem.end());
    erem.resize(distance(erem.begin(), it));
}

vector<iii> get_boundary(Graph &G, vector<int> &ball_in, vector<int> &ball_out)
{
    vector<vector<ii>> adj_list = G.get_adj_list();
    vector<iii> boundary;
    for (int u : G.get_active_nodes())
    {
        for (auto inter : adj_list[u])
        {
            auto v = inter.first;
            if (!G.is_active(v))
                continue;

            auto w = inter.second;

            if (is_in(u, ball_out) && !is_in(v, ball_out))
            {
                // boundary[u].push_back(make_pair(v, w));
                if (u < v)
                    boundary.push_back({u, v, w});
                else
                    boundary.push_back({v, u, w});
            }
            else if (!is_in(u, ball_in) && is_in(v, ball_in))
            {
                // boundary[u].push_back(make_pair(v, w));
                if (u < v)
                    boundary.push_back({u, v, w});
                else
                    boundary.push_back({v, u, w});
            }
        }
    }
    return boundary;
}

int contain_light(vector<Weight> &weight, Graph &G)
{
    for (int i = 0; i < weight.size(); i++)
    {
        if (!G.is_active(i))
            continue;
        if (weight[i] == Weight::IN_LIGHT || weight[i] == Weight::OUT_LIGHT)
            return i;
    }
    return -1;
}

vector<int> computer_ball_in(Graph &G, int s, int d)
{
    int n = G.get_V();
    vector<vector<ii>> adj_list = G.get_adj_list();
    vector<int> ball_in;
    vector<int> dist(n, INF);
    priority_queue<ii, vector<ii>, greater<ii>> pq;
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
        for (auto iter : (adj_list)[u])
        {
            auto v = iter.first;
            if (!G.is_active(v))
                continue;
            auto w = iter.second;
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    sort(ball_in.begin(), ball_in.end());
    return ball_in;
}

vector<int> computer_ball_out(Graph &G, int s, int d)
{
    int n = G.get_V();
    vector<vector<ii>> adj_list_rev = G.get_adj_list_rev();
    vector<int> ball_out;
    vector<int> dist(n, INF);
    priority_queue<ii, vector<ii>, greater<ii>> pq;
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
        for (auto iter : (adj_list_rev)[u])
        {
            auto v = iter.first;
            if (!G.is_active(v))
                continue;
            auto w = iter.second;
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    sort(ball_out.begin(), ball_out.end());
    return ball_out;
}

bool is_sub_set(const vector<int> &a, const vector<int> &b)
{
    for (int i : a)
        if (find(b.begin(), b.end(), i) == b.end())
            return false;
    return true;
}

bool is_in(int x, const vector<int> &a)
{
    return find(a.begin(), a.end(), x) != a.end();
}