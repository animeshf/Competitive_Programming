#include "bits/stdc++.h"
using namespace std;

/*
    Template for Gomory Hu Tree Construction.
    NMAX : Number of Nodes in the Graph
    MMAX : Number of Edges in the Graph
    INF : INFINITY (Should be greater than maximum flow possible)

    We use the Gusfield's Algorithm to construct the Gomory Hu Tree.

    This uses N - 1 Max Flow computations. We use Edmonds' Karp Algorithm
    for the Max Flow. Substituting it with Dinics' would make code faster.
    The constructed Gomory Hu tree is stored in 'tree'

    Assumes Graph Nodes are indexed from 1 through n.
    Assumes everything, included all max flow values, fit in integer.
*/

const int NMAX = 205;
const int MMAX = 1005;
const int INF = 1e9;

int n, m, s, t;
int cap[NMAX][NMAX], flow[NMAX][NMAX], adj[NMAX][NMAX];
int min_cut[NMAX][NMAX];
int par[NMAX], par_in_tree[NMAX];
bool vis[NMAX];
vector < pair < int, int > > tree[NMAX];
vector < int > graph[NMAX];
queue < int > bfs;

// Finds several augmenting paths at once
inline bool augmenting_paths() {
    memset(par, 0, sizeof par);
    memset(vis, 0, sizeof vis);
    bfs.push(s);
    vis[s] = true;
    par[s] = -1;
    int node;
    while (!bfs.empty()) {
        node = bfs.front();
        bfs.pop();
        for (auto it: graph[node]) {
            if (cap[node][it] - flow[node][it] && !vis[it]) {
                par[it] = node;
                vis[it] = true;
                bfs.push(it);
            }
        }
    }
    return vis[t];
}

inline int edmonds_karp() {
    memset(flow, 0, sizeof flow);
    int ans = 0;
    while (augmenting_paths()) {
        for (int i = 1; i <= n; ++i) {
            if ((cap[i][t] - flow[i][t]) && (vis[i])) {
                int node = i;
                int cur_flow = cap[i][t] - flow[i][t];
                while (node != s) {
                    cur_flow = min(cur_flow, cap[par[node]][node] - flow[par[node]][node]);
                    node = par[node];
                }
                flow[i][t] += cur_flow;
                flow[t][i] -= cur_flow;
                node = i;
                ans += cur_flow;
                while (node != s) {
                    flow[par[node]][node] += cur_flow;
                    flow[node][par[node]] -= cur_flow;
                    node = par[node];
                }
            }
        }
    }
    return ans;
}

inline void constructGomoryHuTree(int cur[NMAX][NMAX]) {
    // Cleanup Graphs and Trees, Reset Necessary Stuff
    for (int i = 1; i <= n; i++) {
        graph[i].clear();
        tree[i].clear();
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            min_cut[i][j] = INF;
        }
    }
    for (int i = 2; i <= n; i++) {
        par_in_tree[i] = 1;
    }
    // Build Graph
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cap[i][j] = cur[i][j];
            if (cap[i][j]) {
                graph[i].push_back(j);
            }
        }
    }
    // Gusfield's Algorithm to Compute the Gomory Hu Tree
    for (int i = 2; i <= n; i++) {
        s = i, t = par_in_tree[i];
        min_cut[i][par_in_tree[i]] = min_cut[par_in_tree[i]][i] = edmonds_karp();
        int edge_weight = min_cut[i][par_in_tree[i]];
        tree[t].push_back(make_pair(s, edge_weight));
        tree[s].push_back(make_pair(t, edge_weight));
        for (int j = i + 1; j <= n; j++) {
            if ((vis[j]) && (par_in_tree[j] == t)) {
                par_in_tree[j] = i;
            }
        }
        for (int j = 1; j < i; j++) {
            min_cut[i][j] = min_cut[j][i] = min(min_cut[par_in_tree[i]][j], min_cut[i][par_in_tree[i]]);
        }
    }
}

// End of Template

int forbidden[NMAX];

inline void find_min_edge(int u, int p, int &min_val, int &min_u, int &min_v) {
    for (auto edge : tree[u]) {
        int v = edge.first;
        int w = edge.second;
        if ((forbidden[v]) || (v == p)) {
            continue;
        }
        find_min_edge(v, u, min_val, min_u, min_v);
        if (min_val > w) {
            min_val = w;
            min_u = u;
            min_v = v;
        }
    }
}

inline void solve(int node) {
    int min_u = INF, min_v = INF, min_val = INF;
    find_min_edge(node, 0, min_val, min_u, min_v);
    if (min_val == INF) {
        cout << node << ' ';
        return;
    }
    // Solve for one side
    forbidden[min_u] = true;
    solve(min_v);
    forbidden[min_u] = false;
    // Merge with other side
    forbidden[min_v] = true;
    solve(min_u);
    forbidden[min_v] = false;
}

int main() {
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        int u, v, w; cin >> u >> v >> w;
        adj[u][v] = w;
        adj[v][u] = w;
    }
    constructGomoryHuTree(adj);
    int cost = 0;
    for (int u = 1; u <= n; u++) {
        for (auto edge : tree[u]) {
            cost += edge.second;
        }
    }
    cost >>= 1;
    cout << cost << "\n";
    fill(forbidden + 1, forbidden + 1 + n, false);
    solve(1);
}