// Given a rooted tree of  nodes, where each node is uniquely numbered in between [1..N].
// The node 1 is the root of the tree. Each node has an integer value which is initially 0.
// You need to perform the following two kinds of queries on the tree:
// add t value: Add value to all nodes in subtree rooted at t
// max a b: Report maximum value on the path from a to b

#include <bits/stdc++.h>
using namespace std;

// Adjust appropriately.
const int MAX = 100005;
vector < int > adj[MAX];
int n, q, cur = 0;
int parent[MAX], root[MAX], heavy[MAX], depth[MAX], sub[MAX];
int tin[MAX], tout[MAX], pos[MAX];
int tree[MAX << 2], lazy[MAX << 2];

void dfs(int u, int p){
	sub[u] = 1;
	int mx = 0;
	for(int v : adj[u]){
		if(v == p) continue;
		parent[v] = u;
		depth[v]  = depth[u] + 1;
		dfs(v, u);
		sub[u] += sub[v];
		if(sub[v] > mx){
			mx = sub[v];
			heavy[u] = v;
		}
	}
}

void decompose(int u, int p){
	 if(parent[u] == -1 || heavy[parent[u]] != u) root[u] = u;
	 else root[u] = root[parent[u]];
	 tin[u] = pos[u] = ++cur;
	 if(heavy[u] != -1) decompose(heavy[u], u);
	 for(int v : adj[u]){
	 	if(v == p or v == heavy[u]) continue;
	 	decompose(v, u);
	 }
	 tout[u] = cur;
}

void hld(){
	for(int i = 1; i <= n; i++) parent[i] = heavy[i] = -1;
	dfs(1, 0);
	decompose(1, 0);
}

void push(int l, int r, int node){
	tree[node] += (lazy[node]);
	if(l != r){
		lazy[node + node] += lazy[node];
		lazy[node + node + 1] += lazy[node];
	}
	lazy[node] = 0;
}

void update(int node, int l, int r, int qs, int qe, int val){
	push(l, r, node);
	if(l > qe or r < qs) return;
	if(l >= qs and r <= qe){
		lazy[node] = val;
		push(l, r, node);
		return;
	}
	int mid = l + r >> 1;
	update(node + node, l, mid, qs, qe, val);
	update(node + node + 1, mid + 1, r, qs, qe, val);
	tree[node] = max(tree[node + node], tree[node + node + 1]);
}

int query(int node, int l, int r, int qs, int qe){
	push(l, r, node);
	if(l > qe or r < qs) return -1e9;
	if(l >= qs and r <= qe) return tree[node];
	int mid = l + r >> 1;
	return max(query(node + node, l, mid, qs, qe), query(node + node + 1, mid + 1, r, qs, qe));
}

int query(int u, int v){
	int res = -1e9;
	for(; root[u] != root[v]; v = parent[root[v]]){
		if(depth[root[u]] > depth[root[v]]) swap(u, v);
		res = max(res, query(1, 1, n, pos[root[v]], pos[v]));
	}
	if(depth[u] > depth[v]) swap(u, v);
	res = max(res, query(1, 1, n, pos[u], pos[v]));
	return res;
}

int main(){
	scanf("%d\n", &n);
	for(int i = 1; i < n; i++){
		int u, v;
		scanf("%d %d\n", &u, &v);
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	scanf("%d\n", &q);
	hld();
	char buf[5];
	int x, y;
	while(q--){
		scanf("%s %d %d\n", buf, &x, &y);
		if(buf[0] == 'a'){
			update(1, 1, n, tin[x], tout[x], y);
		}
		else{
			printf("%d\n", query(x, y));
		}
	}
}