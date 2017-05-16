#include "bits/stdc++.h"
using namespace std;

const int N = 105;
const int K = 1005;
const long long INF = 1e10;
const int MAX_ANS = 1e9 + 1;

int n, m, items;
int buy[N][K], sell[N][K];
long long dist[N][N], profit[N][N];
long long adj[N][N], adj2[N][N];

inline bool check(long long x) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (dist[i][j] == INF) {
                adj[i][j] = -INF;
            } else {
                adj[i][j] = profit[i][j] - dist[i][j] * x;
                adj[i][j] = max(-INF, adj[i][j]);
            }
        }
        adj[i][i] = -INF;
    }
    long long res = -INF;
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                adj[i][j] = max(adj[i][j], adj[i][k] + adj[k][j]);
                res = max(res, adj[i][i]);
                if (res >= 0) {
                    return true;
                }
                adj[i][j] = min(adj[i][j], INF);
                adj2[i][j] = adj[i][j];
            }
        }
    }
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                adj2[i][j] = max(adj2[i][j], adj2[i][k] + adj2[k][j]);
                if (adj2[i][j] > adj[i][j]) {
                    return true;
                }
            }
        }
    }
    return false;
}

int main() {
    ios :: sync_with_stdio(false);
    cin >> n >> m >> items;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= items; j++) {
            cin >> buy[i][j] >> sell[i][j];
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            dist[i][j] = INF;
        }
        dist[i][i] = 0;
    }
    for (int i = 1; i <= m; i++) {
        int u, v, w; cin >> u >> v >> w;
        dist[u][v] = min(dist[u][v], (long long) w);
    }
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            profit[i][j] = 0;
            for (int k = 1; k <= items; k++) {
                if (buy[i][k] == -1 || sell[j][k] == -1) continue;
                profit[i][j] = max(profit[i][j], (long long) sell[j][k] - (long long) buy[i][k]);
            }
        }
    }
    long long l = 1, r = MAX_ANS;
    long long ans = 0;
    while (l <= r) {
        long long mid = (l + r) >> 1;
        if (check(mid)) {
            l = mid + 1;
            ans = mid;
        } else {
            r = mid - 1;
        }
    }
    cout << ans << "\n";
}