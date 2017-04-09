/*
   Given Directed Graph G = (V, E), and a source s.
   f(s, x) : Number of nodes y such that all shortest paths from s to x pass through y.
   Problem : Remove a node x (x != s) from G such that the function f(s, x) is maximized.

   Solution uses concept of Dominators but is simpler since the shortest path DAG we consider
   is acyclic. For general graphs, we will need a Dominator Tree.

   Problem Source : CodeCraft 2017 'F'.
*/
#include "bits/stdc++.h"
using namespace std;

const int N = 200000 + 10;
const int M = 300000 + 10;
const long long INF = (1LL << 60LL);

int n, m, s;
int u[M], v[M], w[M];
vector < pair < int, int > > adj[N];
vector < int > parents[N];
int topological_order[N];
long long dist[N];
int hdom[N], ans[N];

inline void dijkstra() {
    set < pair < long long, int > > dijkstra;
    for (int i = 1; i <= n; i++) {
        dist[i] = INF;
    }
    dijkstra.insert({0, s});
    int ticks = 0;
    while (!dijkstra.empty()) {
        long long curDistance = dijkstra.begin() -> first;
        int curNode = dijkstra.begin() -> second;
        dijkstra.erase(dijkstra.begin());
        if (dist[curNode] == INF) {
            dist[curNode] = curDistance;
            topological_order[++ticks] = curNode;
            for (auto edge : adj[curNode]) {
                dijkstra.insert({curDistance + edge.second, edge.first});
            }
        }
    }
    for (int i = 1; i <= m; i++) {
        if (dist[u[i]] + w[i] == dist[v[i]]) {
            parents[v[i]].push_back(u[i]);
        }
        if (dist[v[i]] + w[i] == dist[u[i]]) {
            parents[u[i]].push_back(v[i]);
        }
    }
}

inline void solve() {
    int res = 0;
    for (int i = 2; i <= n; i++) {
        int cur = topological_order[i];
        if (cur == 0) {
            break;
        }
        bool ok = true;
        int prev = -1;
        for (int p : parents[cur]) {
            if (p == s) {
                ok = false;
            } else if ((prev != -1) && (hdom[p] != prev)) {
                ok = false;
            }
            prev = hdom[p];
        }
        if (ok) {
            hdom[cur] = prev;
        } else {
            hdom[cur] = cur;
        }
        ans[hdom[cur]]++;
        res = max(res, ans[hdom[cur]]);
    }
    cout << res << "\n";
}

int main() {
    cin >> n >> m >> s;
    for (int i = 1; i <= m; i++) {
        cin >> u[i] >> v[i] >> w[i];
        adj[u[i]].push_back({v[i], w[i]});
        adj[v[i]].push_back({u[i], w[i]});
    }
    dijkstra();
    solve();
}