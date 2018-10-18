// SPOJ: TREEISO
// Solution is O(n log n) due to sorting the children's hashes
#include "bits/stdc++.h"
using namespace std;

const int N = 123456;
const int P = 137;
const int Q = 139;
const int MOD = 1000000007;

int n;
vector<int> adj[2][N];

inline int add(int x, int y) {
  return (x + y) % MOD;
}

inline int prod(int x, int y) {
  return (1LL * x * y) % MOD;
}

inline int dfs(int u, int id, int p = -1) {
  vector<int> h;
  for (int v: adj[id][u]) {
    if (v != p) {
      h.push_back(prod(Q, dfs(v, id, u)));
    }
  }
  sort(h.begin(), h.end());
  int res = adj[id][u].size();
  for (int val: h) {
    res = add(prod(res, P), val);
  }
  return res;
}

vector<int> get_centers(int id) {
  vector<int> degree(n + 1);
  vector<bool> removed(n + 1);
  vector<int> sol;
  if (n <= 2) {
    for (int i = 1; i <= n; i++)
      sol.push_back(i);
    return sol;
  }
  queue<int> bfs;
  int rem = n;
  for (int i = 1; i <= n; i++) {
    degree[i] = adj[id][i].size();
    removed[i] = false;
    if (degree[i] == 1) {
      bfs.push(i);
      removed[i] = true;
      rem--;
    }
  }
  while (rem > 2) {
    vector<int> next_set_of_leaves;
    while (!bfs.empty()) {
      int u = bfs.front();
      bfs.pop();
      for (int v: adj[id][u]) {
        if (!removed[v]) {
          degree[v]--;
          if (degree[v] == 1)
            next_set_of_leaves.push_back(v);
        }
      }
    }
    for (int v: next_set_of_leaves) {
      removed[v] = true;
      rem--;
      bfs.push(v);
    }
  }
  for (int i = 1; i <= n; i++) {
    if (!removed[i])
      sol.push_back(i);
  }
  return sol;
}

int main() {
  ios :: sync_with_stdio(false);
  int z; cin >> z;
  while (z--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      adj[0][i].clear();
      adj[1][i].clear();
    }
    for (int id = 0; id < 2; id++) {
      for (int i = 1; i < n; i++) {
        int u, v; cin >> u >> v;
        adj[id][u].push_back(v);
        adj[id][v].push_back(u);
      }
    }
    vector<int> x = get_centers(0);
    vector<int> y = get_centers(1);
    if (x.size() != y.size()) {
      cout << "NO" << endl;
    } else {
      bool found = false;
      for (int i = 0; i < (int) x.size(); i++) {
        for (int j = 0; j < (int) y.size() && !found; j++) {
          if (dfs(x[i], 0) == dfs(y[j], 1)) {
            cout << "YES" << endl;
            found = true;
          }
        }
      }
      if (!found) {
        cout << "NO" << endl;
      }
    }
  }
}
