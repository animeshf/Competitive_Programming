// Illuminate - SER 2016
#include "bits/stdc++.h"
using namespace std;

const int N = 1000;

int s, r, n, id;
int x[N], y[N], arr[N * 4];
vector < int > adj[N * 4];
bool mark[N * 4];

/*
    2-SAT Template
    Given an implication graph, this checks if a solution exists.

    addClause() adds the clause (x or y) to the set of clauses.
    You can also add additional implications yourself.

    init() initializes 2-SAT arrays.
    solve() checks if in the final implication graph, a valid solution exists.

    mark[u] stores the boolean value of the node (u). You can use mark[] to
    recover the final solution as well.

    Notes on Indexing Nodes :
    u = 2k, !u = 2k + 1
    Nodes are 0-indexed. [0, NUM_VERTICES)
*/

int NUM_VERTICES;

inline bool dfs(int node) {
    if (mark[node ^ 1]) {
        return false;
    }
    if (mark[node]) {
        return true;
    }
    mark[node] = true;
    arr[id++] = node;
    for (int i = 0; i < (int) adj[node].size(); i++) {
        if (!dfs(adj[node][i])) {
            return false;
        }
    }
    return true;
}

inline void init() {
    for (int i = 0; i < NUM_VERTICES; i++) {
        adj[i].clear();
    }
    memset(mark, 0, sizeof(mark));
}

// Adds the clause (u or v) to the set of clauses
inline void addClause(int u, int v) {
    adj[u ^ 1].push_back(v);
    adj[v ^ 1].push_back(u);
}

// Returns true if a solution exists.
inline bool solve() {
    for (int i = 0; i < NUM_VERTICES; i++) {
        sort(adj[i].begin(), adj[i].end());
        adj[i].resize(unique(adj[i].begin(), adj[i].end()) - adj[i].begin());
    }
    for (int i = 0; i < NUM_VERTICES; i += 2) {
        if ((!mark[i]) && (!mark[i + 1])) {
            id = 0;
            if(!dfs(i)) {
                while (id > 0) {
                    mark[arr[--id]] = false;
                }
                if(!dfs(i + 1)) {
                    return false;
                }
            }
        }
    }
    return true;
}

// End of 2-SAT Template.

int main() {
    cin >> s >> r >> n;
    NUM_VERTICES = n * 4;
    for (int i = 0; i < n; i++) {
        cin >> x[i] >> y[i];
    }
    init();
    for (int i = 0; i < n; i++) {
        /*
            4k -> Horizontal
            4k + 1 -> !Horizontal
            4k + 2 -> Vertical
            4k + 3 -> !Vertical
        */
        int uHorizontal = 4 * i;
        int uVertical = uHorizontal + 2;
        addClause(uHorizontal, uVertical); // horizontal OR vertical
        addClause(uHorizontal + 1, uVertical + 1); // !horizontal OR !vertical
        for (int j = 0; j < n; j++) {
            if (j == i) {
                continue;
            }
            int vHorizontal = 4 * j;
            int vVertical = vHorizontal + 2;
            if (x[j] == x[i]) {
                if (abs(y[j] - y[i]) <= 2 * r) {
                    adj[uHorizontal].push_back(vHorizontal + 1); // uHorizontal -> !vHorizontal
                }
            }
            if (y[j] == y[i]) {
                if (abs(x[j] - x[i]) <= 2 * r) {
                    adj[uVertical].push_back(vVertical + 1); // uVertical -> !vVertical
                }
            }
        }
    }
    cout << (solve()) << '\n';
}
