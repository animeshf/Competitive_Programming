// IOI 2011 - Race
#include "bits/stdc++.h"
using namespace std;

const int N = 1000000 + 50;
const int INF = 100000000;

int n, k, sub[N];
vector < pair < int, int > > adj[N];
vector < pair < int, int > > paths;
map < int, int > ans;
bool vis[N];

inline void dfs(int u, int p) {
    sub[u] = 1;
    for (int i = 0; i < (int) adj[u].size(); i++) {
        int v = adj[u][i].first;
        if ((v == p) or (vis[v])) {
            continue;
        }
        dfs(v, u);
        sub[u] += sub[v];
    }
}

inline int find_centroid(int u, int x, int p) {
    for (int i = 0; i < (int) adj[u].size(); i++) {
        int v = adj[u][i].first;
        if ((v == p) or (vis[v])) {
            continue;
        }
        if (sub[v] > x) {
            return find_centroid(v, x, u);
        }
    }
    return u;
}

inline void explore(int u, int p, int cost, int edges) {
    if (cost <= k) {
        paths.push_back(make_pair(cost, edges));
    }
    for (int i = 0; i < (int) adj[u].size(); i++) {
        int v = adj[u][i].first;
        int w = adj[u][i].second;
        if ((vis[v]) or (cost + w > k) or (v == p)) {
            continue;
        }
        explore(v, u, cost + w, edges + 1);
    }
}

inline int best_path(int u) {
    ans.clear();
    ans[0] = 0, ans[k] = INF;
    for (int i = 0; i < (int) adj[u].size(); i++) {
        int v = adj[u][i].first;
        int w = adj[u][i].second;
        if (vis[v]) {
            continue;
        }
        paths.clear();
        explore(v, u, w, 1);
        for (int j = 0; j < (int) paths.size(); j++) {
            int path_cost = paths[j].first;
            int edges_used = paths[j].second;
            if (ans.find(k - path_cost) != ans.end()) {
                ans[k] = min(ans[k], ans[k - path_cost] + edges_used);
            }
        }
        for (int j = 0; j < (int) paths.size(); j++) {
            int path_cost = paths[j].first;
            int edges_used = paths[j].second;
            if (ans.find(path_cost) == ans.end()) {
                ans[path_cost] = edges_used;
            } else {
                ans[path_cost] = min(ans[path_cost], edges_used);
            }
        }
    }
    return ans[k];
}

inline int decompose(int root, int p){
    int u = find_centroid(root, sub[root] >> 1, p);
    vis[u] = true;
    int res = best_path(u);
    for (int i = 0; i < (int) adj[u].size(); i++) {
        int v = adj[u][i].first;
        if (vis[v]) {
            continue;
        }
        dfs(v, u);
        res = min(res, decompose(v, u));
    }
    return res;
}

int main(){
    scanf("%d %d", &n, &k);
    for (int i = 1; i < n; i++) {
        int u, v, w; scanf("%d %d %d", &u, &v, &w);
        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
    }
    dfs(0, -1);
    int res = decompose(0, -1);
    if (res > n) {
        res = -1;
    }
    printf("%d\n", res);
}