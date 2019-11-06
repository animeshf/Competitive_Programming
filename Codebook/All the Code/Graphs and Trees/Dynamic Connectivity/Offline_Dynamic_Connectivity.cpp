// Offline Dynamic Connectivity
// Build a segment tree indexed on time, each edge is present
// in some contiguous subinterval. Build answer by top down traversal
// Invariant: All edges are "considered" when building the answer.
// The question specific logic is contingent on 'join' and 'rollback'.
#include "bits/stdc++.h"
using namespace std;

const int N = 333333;

// Edge between u and v is active from time [L, R]
int n, k, num_components;
int ans[N], parent[N], sz[N];
bool need[N];
map<int, map<int, int>> edge_st;
vector<tuple<int, int, int, int>> boost;
vector<pair<int, int>> tree[N * 4];

inline int root(int x) {
	if (parent[x] == x) return x;
	return root(parent[x]);
}

inline bool join(int u, int v) {
	u = root(u);
	v = root(v);
	if (u == v) return false;
	if (sz[u] > sz[v]) swap(u, v);
	num_components--;
	boost.push_back(make_tuple(u, parent[u], v, sz[v]));
	parent[u] =  v;
	sz[v] += sz[u];
	return true;
}

inline void rollback() {
	auto [u, pu, v, szv] = boost.back();
	boost.pop_back();
	parent[u] = pu;
	sz[v] = szv;
	num_components++;
}

void update(int i, int l, int r, int qs, int qe, int u, int v) {
	if (l > qe || r < qs) return;
	if (l >= qs && r <= qe) {
		tree[i].push_back(make_pair(u, v));
		return;
	}
	int mid = (l + r) >> 1;
	update(i * 2, l, mid, qs, qe, u, v);
	update(i * 2 + 1, mid + 1, r, qs, qe, u, v);
}

void query(int i, int l, int r) {
	int num_joins = 0;
	for (auto [u, v]: tree[i]) {
		num_joins += join(u, v);
	}
	if (l == r) {
		ans[l] = num_components;
	} else {
		int mid = (l + r) >> 1;
		query(i * 2, l, mid);
		query(i * 2 + 1, mid + 1, r);
	}
	for (int j = 1; j <= num_joins; j++) {
		rollback();
	}
}

int main() {
	//freopen ("connect.in", "r", stdin);
	//freopen ("connect.out", "w", stdout);
	ios :: sync_with_stdio(false);
	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		parent[i] = i;
		sz[i] = 1;
	}
	num_components = n;
	for (int i = 1; i <= k; i++) {
		char type; cin >> type;
		int u, v;
		if (type == '+') {
			cin >> u >> v; if (u > v) swap(u, v);
			edge_st[u][v] = i;
		} else if (type == '-') {
			cin >> u >> v; if (u > v) swap(u, v);
			update(1, 1, k, edge_st[u][v], i - 1, u, v);
			edge_st[u][v] = -1;
		} else {
			need[i] = true;
		}
	}
	for (auto [u, mp]: edge_st) {
		for (auto [v, st]: edge_st[u])
			if (st != -1) update(1, 1, k, st, k, u, v);
	}
	if (k > 0) query(1, 1, k);
	for (int i = 1; i <= k; i++) {
		if (need[i]) {
			cout << ans[i] << endl;
		}
	}
}
