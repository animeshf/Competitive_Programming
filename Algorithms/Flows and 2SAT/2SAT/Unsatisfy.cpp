// SER 2017 - Unsatisfy
#include "bits/stdc++.h"
using namespace std;

const int MAX = 5678;
const int OFF = 2345;

int n, m, root;
vector < int > adj[MAX];
int reach[MAX][MAX];

inline void dfs(int u) {
    reach[root][u] = true;
    for (int v: adj[u]) {
        if (reach[root][v] == false) {
            dfs(v);
        }
    }
}

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        adj[OFF - u].push_back(OFF + v);
        adj[OFF - v].push_back(OFF + u);
    }
    for (int i = 1; i <= n; i++) {
        root = OFF - i; dfs(OFF - i);
        root = OFF + i; dfs(OFF + i);
    }
    // Ans = 0
    for (int i = 1; i <= n; i++) {
        if (reach[OFF - i][OFF + i] + reach[OFF + i][OFF - i] == 2) {
            cout << 0 << endl;
            return 0;
        }
    }
    // Ans = 1
    for (int i = 1; i <= n; i++) {
        if (reach[OFF + i][OFF - i]) {
            cout << 1 << endl;
            return 0;
        }
    }
    // Ans = 2
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (reach[OFF + i][OFF - j]) {
                cout << 2 << endl;
                return 0;
            }
        }
    }
    cout << -1 << endl;
}