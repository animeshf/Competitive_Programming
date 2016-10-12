#include "bits/stdc++.h"
using namespace std;

const int N = 100000 + 50;

int n, t[N];
vector < int > adj[N];
double dp[N], memo[N];
unordered_map < int, double > hashMap[N];
unordered_map < int, double > vis[N];

inline double dfs(int node, int parent) {
    if (hashMap[node].count(parent)) {
        return hashMap[node][parent];
    }
    hashMap[node][parent] = t[node];
    double res = 0.0;
    if (vis[node].size() == (int) adj[node].size()) {
        if (parent != -1) {
            res = memo[node] - vis[node][parent];
        } else {
            res = memo[node];
        }
    } else {
        for (int next : adj[node]) {
            if (next != parent) {
                double curVal = dfs(next, node);
                if (!vis[node].count(next)) {
                    vis[node][next] = curVal;
                    memo[node] += curVal;
                }
                res += curVal;
            }
        }
    }
    res /= max(1, ((int) adj[node].size() - (parent != -1)));
    hashMap[node][parent] += res;
    return hashMap[node][parent];
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", t + i);
    }
    for (int i = 1; i <= n - 1; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    double minVal = 1e18;
    double minIdx = 1;
    for (int i = 1; i <= n; i++) {
        double value = dfs(i, -1);
        if (value < minVal) {
            minVal = value;
            minIdx = i;
        }
    }
    cout << minIdx << '\n';
}