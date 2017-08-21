#include "bits/stdc++.h"
using namespace std;

const int N = 333333;

int n, m;
int d[N], vis[N];
vector < pair < int, int > > adj[N];
vector < int > sol;

inline void solve(int u) {
    vis[u] = 1;
    for (auto edge: adj[u]) {
        int v = edge.first;
        int i = edge.second;
        if (vis[v]) {
            continue;
        }
        solve(v);
        if (d[v]) {
            sol.push_back(i);
            d[u] ^= 1;
        }
    }
}

int main() {
    freopen ("inp.in", "r", stdin);
    ios :: sync_with_stdio(false);
    cin >> n >> m;
    int sum = 0;
    int special = 0;
    for (int i = 1; i <= n; i++) {
        cin >> d[i];
        if (d[i] >= 0) {
            sum += d[i];
        } else {
            special = i;
        }
    }
    for (int i = 1; i <= m; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }
    if ((sum & 1) && (!special)) {
        cout << -1 << endl;
        return 0;
    }
    d[special] = (sum & 1);
    for (int i = 1; i <= n; i++) {
        if (d[i] == -1) {
            d[i] = 0;
        }
    }
    solve(1);
    cout << (int) sol.size() << endl;
    for (int id: sol) {
        cout << id << ' ';
    }
    cout << endl;
}