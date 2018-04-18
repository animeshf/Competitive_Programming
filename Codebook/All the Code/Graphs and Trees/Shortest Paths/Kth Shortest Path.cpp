// SPOJ - MKTHPATH: Computes the lexicographically smallest Kth shortest path from A to B
// A path P is different from Q if the edges visited on the path are different.
// If there are K paths with length 1, then the answer is 1, and you must print the lexico
// smallest path with length 1.

#include "bits/stdc++.h"
using namespace std;

const int N = 55;
const int K = 205;
const int INF = 1e9;

struct state {
    int dist, node, par;
    long long vis;
    state(int d = 0, int n = 0, long long v = 0, int p = 0) {
        dist = d;
        node = n;
        vis = v;
        par = p;
    }
    inline friend bool operator < (state x, state y) {
        return (x.dist > y.dist);
    }
};

vector < pair < int, int > > adj[N], rev[N];
int n, m, k, a, b;
int dist[N][K], cnt[N];
vector < int > bef[N][K];

void solve() {
    // Dijkstra in O(M * K * log(MK))
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= k; j++) {
            dist[i][j] = INF;
            bef[i][j].clear();
        }
    }
    priority_queue < state > dijkstra;
    dijkstra.push(state(0, b, 1LL << b, -1));
    while (!dijkstra.empty()) {
        state cur = dijkstra.top();
        dijkstra.pop();
        int distance = cur.dist;
        int u = cur.node;
        long long on_path = cur.vis;
        int p = cur.par;
        if (cnt[u] == k) {
            if (dist[u][k] == distance)
                bef[u][k].push_back(p);
            continue;
        }
        ++cnt[u];
        dist[u][cnt[u]] = distance;
        bef[u][cnt[u]].push_back(p);
        for (auto edge : rev[u]) {
            int v = edge.first;
            int w = edge.second;
            if (on_path & (1LL << v)) {
                continue;
            }
            int newDistance = min(INF, distance + w);
            if ((cnt[v] == k) && (dist[u][k] < newDistance)) {
                continue;
            }
            dijkstra.push(state(newDistance, v, on_path | (1LL << v), u));
        }
    }
    if (dist[a][k] == INF) {
        puts("None");
        return;
    }
    // Constructing answer in O(M * K)
    vector < int > ans;
    int u = a;
    int cur_dist = 0;
    long long seen = 0;
    while (u != b) {
        ans.push_back(u);
        seen |= (1LL << u);
        int min_v = INF;
        for (int i = 1; i <= k; i++) {
            if (dist[u][i] + cur_dist == dist[a][k]) {
                for (int x : bef[u][i]) {
                    if (seen & (1LL << x)) {
                        continue;
                    }
                    min_v = min(min_v, x);
                }
            }
        }
        for (auto edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;
            if (v == min_v) {
                cur_dist += w;
                break;
            }
        }
        u = min_v;
    }
    for (int x : ans) {
        printf("%d-", x);
    }
    printf("%d\n", b);
}

int main() {
    freopen ("inp.in", "r", stdin);
    while (true) {
        scanf("%d %d %d %d %d", &n, &m, &k, &a, &b);
        if (n == 0) {
            break;
        }
        for (int i = 1; i <= n; i++) {
            adj[i].clear();
            rev[i].clear();
            cnt[i] = 0;
        }
        for (int i = 1; i <= m; i++) {
            int u, v, d; scanf("%d %d %d", &u, &v, &d);
            adj[u].push_back({v, d});
            rev[v].push_back({u, d});
        }
        solve();
    }
}