#include "bits/stdc++.h"
using namespace std;

const int N = 105;
const int M = N * N;

int r, c;
int a[N][N], row_max[N], col_max[N];

/*
    Hopcroft Karp Max Matching in O(E * sqrt(V))
    N = Number of Nodes, M = Number of Edges
    n1 = Size of left partite, n2 = Size of right partite
    Nodes are numbered from [0, n1 - 1] and [0, n2 - 1]

    init(n1, n2) declares the two partite sizes and resets arrays
    addEdge(x, y) adds an edge between x in left partite and y in right partite
    maxMatching() returns the maximum matching

    Maximum Matching = Minimum Vertex Cover (Konig's Theorem)
    N - Maximum Matching = Maximal Independent Set
*/

int n1, n2, edges, last[N], previous[M], head[M];
int matching[N], dist[N], Q[N];
bool used[N], vis[N];

inline void init(int _n1, int _n2) {
    n1 = _n1;
    n2 = _n2;
    edges = 0;
    fill(last, last + n1, -1);
}

inline void addEdge(int u, int v) {
    head[edges] = v;
    previous[edges] = last[u];
    last[u] = edges++;
}

inline void bfs() {
    fill(dist, dist + n1, -1);
    int sizeQ = 0;
    for(int u = 0; u < n1; ++u){
        if(!used[u]){
            Q[sizeQ++] = u;
            dist[u] = 0;
        }
    }
    for(int i = 0; i < sizeQ; i++){
        int u1 = Q[i];
        for(int e = last[u1]; e >= 0; e = previous[e]){
            int u2 = matching[head[e]];
            if(u2 >= 0 && dist[u2] < 0){
                dist[u2] = dist[u1] + 1;
                Q[sizeQ++] = u2;
            }
        }
    }
}

inline bool dfs(int u1) {
    vis[u1] = true;
    for(int e = last[u1]; e >= 0; e = previous[e]){
        int v = head[e];
        int u2 = matching[v];
        if(u2 < 0 || !vis[u2] && dist[u2] == dist[u1] + 1 && dfs(u2)){
            matching[v] = u1;
            used[u1] = true;
            return true;
        }
    }
    return false;
}

inline int maxMatching() {
    fill(used, used + n1, false);
    fill(matching, matching + n2, -1);
    for(int res = 0; ;){
        bfs();
        fill(vis, vis + n1, false);
        int f = 0;
        for(int u = 0; u < n1; ++u)
            if(!used[u] && dfs(u))
                ++f;
        if(!f) return res;
        res += f;
    }
}

// End of Hopcroft Karp Template

inline long long solve(int x) {
    init(c, r);
    for (int col = 1; col <= c; col++) {
        if (col_max[col] != x) {
            continue;
        }
        for (int row = 1; row <= r; row++) {
            if (row_max[row] != x) {
                continue;
            }
            if (a[row][col]) {
                // Can satisfy both row and the column.
                addEdge(col - 1, row - 1);
            }
        }
    }
    return (x - 1) * 1LL * maxMatching();
}

int main() {
    ios :: sync_with_stdio(false);
    cin >> r >> c;
    long long sol = 0;
    for (int i = 1; i <= r; i++) {
        for (int j = 1; j <= c; j++) {
            cin >> a[i][j];
            col_max[j] = max(col_max[j], a[i][j]);
            row_max[i] = max(row_max[i], a[i][j]);
            sol += max(a[i][j] - 1, 0);
        }
    }
    // Now we need to satisfy the front and side view
    // Lets place back the highest in each row and column
    set < int > elems;
    for (int i = 1; i <= r; i++) {
        sol -= max(0, row_max[i] - 1);
    }
    for (int i = 1; i <= c; i++) {
        sol -= max(0, col_max[i] - 1);
        elems.insert(col_max[i]);
    }
    // We can steal some stuff while maintaining constraints.
    for (int x : elems) {
        if (x > 1) {
            sol += solve(x);
        }
    }
    cout << sol << "\n";
}