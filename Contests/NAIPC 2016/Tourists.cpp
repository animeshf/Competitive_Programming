#include "bits/stdc++.h"
using namespace std;

const int N = 200000 + 50;
const int LN = 20;

int n;
vector < int > adj[N];
int anc[LN][N], depth[N];

inline void dfs(int u, int p) {
    for (int i = 1; i < LN; i++) {
        anc[i][u] = anc[i - 1][anc[i - 1][u]];
    }
    for (int v : adj[u]) {
        if (v != p) {
            depth[v] = depth[u] + 1;
            anc[0][v] = u;
            dfs(v, u);
        }
    }
}

inline int lca(int x, int y) {
    if (depth[x] < depth[y]) {
        swap(x, y);
    }
    for (int i = LN - 1; i >= 0; i--) {
        if (depth[x] - (1 << i) >= depth[y]) {
            x = anc[i][x];
        }
    }
    if (x == y) {
        return x;
    }
    for (int i = LN - 1; i >= 0; i--) {
        if (anc[i][x] != anc[i][y]) {
            x = anc[i][x];
            y = anc[i][y];
        }
    }
    return anc[0][x];
}

inline int dist(int u, int v) {
    return depth[u] + depth[v] - 2 * depth[lca(u, v)] + 1;
}

int main(){
    ios :: sync_with_stdio(false);
    cin >> n;
    for (int i = 1; i < n; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1, 0);
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = i + i; j <= n; j += i) {
            ans += dist(i, j);
        }
    }
    cout << ans << '\n';
}