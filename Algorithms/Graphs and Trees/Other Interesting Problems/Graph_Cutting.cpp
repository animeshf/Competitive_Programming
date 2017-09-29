// CF Graph Cutting: Given a connected, unweighted graph with N nodes and M edges, decompose it
// into edge-disjoint paths of length two. If a solution exists, print the decomposition.

// Solution works in O(N + M)

#include "bits/stdc++.h"
using namespace std;

const int N = 100005;
const int M = 100005;

int n, m;
int in_degree[N];
bool visited[N];
bool tree_edge[M];
bool directed[M];
vector < int > choose[N];
vector < pair < int, int > > adj[N];

inline void dfs(int u, int p) {
    visited[u] = true;
    for (auto edge: adj[u]) {
        int v = edge.first;
        int i = edge.second;
        if (v == p) {
            continue;
        }
        if (not visited[v]) {
            dfs(v, u);
            tree_edge[i] = true;
        } else if (not directed[i]) {
            in_degree[v]++;
            directed[i] = true;
            choose[v].push_back(u);
        }
    }
}

inline void solve(int u, int p) {
    for (auto edge: adj[u]) {
        int v = edge.first;
        int i = edge.second;
        if ((not tree_edge[i]) || (v == p)) {
            continue;
        }
        solve(v, u);
    }
    if (p != -1) {
        if (in_degree[u] & 1) {
            in_degree[u]++;
            choose[u].push_back(p);
        } else {
            in_degree[p]++;
            choose[p].push_back(u);
        }
    }
}


int main() {
    cin >> n >> m;
    if (m & 1) {
        cout << "No solution" << endl;
        return 0;
    }
    for (int i = 1; i <= m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }
    dfs(1, -1);
    solve(1, -1);
    for (int i = 1; i <= n; i++) {
        while ((int) choose[i].size() > 0) {
            int u = choose[i].back();
            choose[i].pop_back();
            int v = choose[i].back();
            choose[i].pop_back();
            cout << u << ' ' << i << ' ' << v << endl;
        }
    }
}