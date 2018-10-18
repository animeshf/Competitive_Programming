// Codeforces - Two Shortest
// Given an undirected graph with integer weights on edges, find two disjoint shortest paths from 1 to n.
// Algorithm runs in O((m + n) log n)

#include "bits/stdc++.h"
using namespace std;

const int N = 405;
const int INF = 1000000000;

int n, m;
vector<pair<int, int>> adj[N];
vector<int> dag[N];
vector<int> rev_dag[N];
int dist[2][N];
map<int, int> residual[N];
int parent[N];

inline void shortest_paths(int st, int id) {
    for (int i = 1; i <= n; i++) {
        dist[id][i] = INF;
    }
    dist[id][st] = 0;
    priority_queue<pair<int, int>> dijkstra;
    dijkstra.push(make_pair(0, st));
    while (!dijkstra.empty()) {
        auto cur = dijkstra.top();
        dijkstra.pop();
        int u = cur.second;
        int sum = -cur.first;
        if (dist[id][u] < sum) continue;
        for (auto e: adj[u]) {
            int v = e.first;
            int w = e.second;
            if (sum + w < dist[id][v]) {
                dist[id][v] = sum + w;
                dijkstra.push(make_pair(-dist[id][v], v));
            }
        }
    }
}

inline bool aug() {
    queue<int> bfs;
    bfs.push(1);
    memset(parent, -1, sizeof parent);
    parent[1] = 0;
    while (!bfs.empty()) {
        int u = bfs.front();
        bfs.pop();
        // Edges in the residual graph can be either actual edges or reverse of the actual edges
        for (int v: dag[u]) {
            if ((parent[v] == -1) && (residual[u][v] > 0)) {
                parent[v] = u;
                bfs.push(v);
            }
        }
        for (int v: rev_dag[u]) {
            if ((parent[v] == -1) && (residual[u][v] > 0)) {
                parent[v] = u;
                bfs.push(v);
            }
        }
    }
    return (parent[n] > 0);
}

int main() {
    freopen ("inp.in", "r", stdin);
    ios :: sync_with_stdio(false);
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        int u, v, w; cin >> u >> v >> w;
        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
    }
    shortest_paths(1, 0);
    shortest_paths(n, 1);
    for (int i = 1; i <= n; i++) {
        for (auto e: adj[i]) {
            int j = e.first;
            int w = e.second;
            if (dist[0][n] == dist[0][i] + w + dist[1][j]) {
                dag[i].push_back(j);
                rev_dag[j].push_back(i);
                residual[i][j] = 1;
            }
        }
    }
    int count = 0;
    while (count < 2 && aug()) {
        count++;
        int i = n;
        while (parent[i] > 0) {
            int pi = parent[i];
            residual[pi][i] -= 1;
            residual[i][pi] += 1;
            i = pi;
        }
    }
    if (count < 2) {
        cout << "No solution" << endl;
    } else {
        for (int it = 0; it < 2; it++) {
            vector<int> path;
            int i = 1;
            while (i != n) {
                path.push_back(i);
                for (int j: dag[i]) {
                    if (residual[i][j] == 0) {
                        residual[i][j] = -1;
                        i = j;
                        break;
                    }
                }
            }
            path.push_back(n);
            for (int vertex: path) {
                cout << vertex << ' ';
            }
            cout << endl;
        }
    }
}


