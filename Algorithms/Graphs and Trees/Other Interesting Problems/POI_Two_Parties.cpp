/*
  This is actually not the POI Two Parties problem, but instead the same problem
  on Codeforces: https://codeforces.com/contest/1070/problem/L
  Problem: Partition the vertices into minimum number of sets such that every
  vertex has even degree in the set that it belongs to.
  Solution:  It can be proven that the answer is at most two. For an explanation
  of this algorithm go to the link below.
  https://math.stackexchange.com/questions/1065448/a-partition-of-vertices-of-a-graph
  Complexity: (N^3 / 32) operations, optimized using bitsets.
*/
#include "bits/stdc++.h"
using namespace std;

const int MAXN = 2005;

int n;
int value[MAXN];
bitset<MAXN> adj[MAXN];

pair<vector<int>, vector<int>> solve(vector<int> graph) {
  int u = -1;
  for (int v : graph) {
    if (adj[v].count() & 1) {
      u = v;
      break;
    }
  }
  if (u == -1) {
    return {graph, {}};
  }
  bitset<MAXN> inv = adj[u];
  for (int i = 1; i <= n; i++) {
    if (adj[u][i]) {
      adj[i] ^= inv;
      adj[i][u] = false;
      adj[i][i] = false;
    }
  }
  vector<int> go;
  for (int v : graph) {
    if (v != u) {
      go.push_back(v);
    }
  }
  auto sol = solve(go);
  vector<int> lef = sol.first;
  vector<int> rig = sol.second;
  int neighbors = 0;
  for (int v: lef) {
    neighbors += adj[u][v];
  }
  if (neighbors & 1) {
    swap(lef, rig);
  }
  lef.push_back(u);
  return make_pair(lef, rig);
}

int main() {
  int z; cin >> z;
  while (z--) {
    int m; cin >> n >> m;
    for (int i = 1; i <= n; i++) {
      adj[i].reset();
    }
    for (int i = 1; i <= m; i++) {
      int x, y; cin >> x >> y;
      adj[x][y] = 1;
      adj[y][x] = 1;
    }
    vector<int> all;
    for (int i = 1; i <= n; i++) {
      all.push_back(i);
    }
    auto res = solve(all);
    for (int v: res.first) {
      value[v] = 1;
    }
    for (int v: res.second) {
      value[v] = 2;
    }
    int total = (res.second.size() > 0) + 1;
    cout << total << endl;
    for (int i = 1; i <= n; i++) {
      cout << value[i] << ' ';
    }
    cout << endl;
  }
}
