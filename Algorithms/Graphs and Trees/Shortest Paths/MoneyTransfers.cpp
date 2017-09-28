// SWERC 2014 - Problem H
// Given a undirected weighted graph G with N nodes, P edges, two nodes X, Y
// and a subset M <= V of vertices, what is the maximum weight you can add to
// every edge in G such that the shortest path from X to Y passes only through vertices in M.

#include "bits/stdc++.h"
using namespace std;

const int N = 2005;
const int P = 20050;
const long long INF = 1LL << 60LL;

int n, p, x, y;
bool black[N];
long long dist[2][N][N]; // (only_swerc?, node, num_edges)
vector < tuple < int, int , int > > edges;

int main() {
    cin >> n >> p >> x >> y;
    for (int i = 1; i <= p; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back(make_tuple(u, v, w));
    }
    int special; cin >> special;
    while (special--) {
        int foo; cin >> foo;
        black[foo] = true;
    }
    black[x] = black[y] = true;

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            dist[0][i][j] = dist[1][i][j] = INF;
        }
    }
    dist[1][x][0] = 0;
    for (int k = 1; k < n; k++) {
        for (auto e: edges) {
            int u = get<0>(e);
            int v = get<1>(e);
            int w = get<2>(e);
            if (black[u]) {
                if (black[v]) {
                    dist[1][v][k] = min(dist[1][v][k], dist[1][u][k - 1] + w);
                    dist[0][v][k] = min(dist[0][v][k], dist[0][u][k - 1] + w);
                } else {
                    dist[0][v][k] = min(dist[0][v][k], dist[1][u][k - 1] + w);
                    dist[0][v][k] = min(dist[0][v][k], dist[0][u][k - 1] + w);
                }
            } else {
                dist[0][v][k] = min(dist[0][v][k], dist[0][u][k - 1] + w);
            }
            swap(u, v);
            if (black[u]) {
                if (black[v]) {
                    dist[1][v][k] = min(dist[1][v][k], dist[1][u][k - 1] + w);
                    dist[0][v][k] = min(dist[0][v][k], dist[0][u][k - 1] + w);
                } else {
                    dist[0][v][k] = min(dist[0][v][k], dist[1][u][k - 1] + w);
                    dist[0][v][k] = min(dist[0][v][k], dist[0][u][k - 1] + w);
                }
            } else {
                dist[0][v][k] = min(dist[0][v][k], dist[0][u][k - 1] + w);
            }
        }
    }
    int A = n, B = n;
    for (int i = 1; i < n; i++) {
        if (dist[1][y][i] < INF) {
            if (A == n) A = i;
        }
        if (dist[0][y][i] < INF) {
            if (B == n) B = i;
        }
    }
    if (A < B) {
        cout << "Infinity\n";
        return 0;
    }
    if ((A == B) && (dist[1][y][A] < dist[0][y][B])) {
        cout << "Infinity\n";
        return 0;
    }
    long long result = 0;
    for (int i = 1; i < n; i++) {
        if (dist[1][y][i] >= INF) {
            continue;
        }
        long long high = INF;
        long long low = -INF;
        for (int j = 1; j < n; j++) {
            if (dist[0][y][j] >= INF) {
                continue;
            }
            if (i >= j) {
                if (dist[0][y][j] <= dist[1][y][i]) {
                    high = -1;
                    break;
                }
                if (i == j) {
                    continue;
                }
            }
            long long numerator = dist[0][y][j] - dist[1][y][i];
            long long denominator = i - j;
            long long value = numerator / denominator;
            if (i >= j) {
                if (abs(numerator) % abs(denominator) == 0) {
                    value--;
                }
                high = min(high, value);
            } else if (numerator < 0) {
                low = max(low, value + 1);
            }
        }
        if (low <= high) {
            result = max(result, high);
        }
    }
    if (result >= INF) {
        cout << "Infinity" << endl;
    } else if (result == 0) {
        cout << "Impossible" << endl;
    } else {
        cout << result << endl;
    }
}