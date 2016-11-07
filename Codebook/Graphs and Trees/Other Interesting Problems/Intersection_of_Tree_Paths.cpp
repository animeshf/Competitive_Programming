/*
        June Lunchtime 2016
	Intersection of Tree Paths
	Counts number of Predefined-Paths in a Tree which intersect with a given path
*/

#include "bits/stdc++.h"
using namespace std;

const int N = 2e5 + 5;
const int LN = 19;

int n, q, cur_time;
int parent[N], dp[LN][N], tin[N], tout[N], depth[N];
int bit[2][N];
vector < int > adj[N];

inline void dfs(int u, int p){
	parent[u] = dp[0][u] = p;
	for(int i = 1; i < LN; i++){
		dp[i][u] = dp[i - 1][dp[i - 1][u]];
	}
	tin[u] = ++cur_time;
	for(int v : adj[u]){
		if(v != p){
			depth[v] = depth[u] + 1;
			dfs(v, u);
		}
	}
	tout[u] = cur_time;
}

inline int lca(int x, int y){
	if(depth[x] < depth[y]) swap(x, y);
	for(int i = LN - 1; i >= 0; i--){
		if((depth[x] - (1 << i)) >= depth[y])
			x = dp[i][x];
	}
	if(x == y) return x;
	for(int i = LN - 1; i >= 0; i--){
		if(dp[i][x] != dp[i][y]){
			x = dp[i][x];
			y = dp[i][y];
		}
	}
	return parent[x];
}

inline void update_tree(int id, int idx, int val){
	for(int i = idx; i <= n; i += i & -i) bit[id][i] += val;
}

inline int query_tree(int id, int idx, int res = 0){
	for(int i = idx; i > 0;  i -= i & -i) res += bit[id][i];
	return res;
}

inline int solve(int u, int v){
	int lc = lca(u, v), res = 0;
	res += query_tree(0, tin[u]) + query_tree(0, tin[v]) - query_tree(0, tin[lc]) - query_tree(0, tin[parent[lc]]);
	res += query_tree(1, tout[lc]) - query_tree(1, tin[lc] - 1);
	update_tree(0, tin[lc], 1), update_tree(0, tout[lc] + 1, -1);
	update_tree(1, tin[u], 1),  update_tree(1, tin[v], 1), update_tree(1, tin[lc], -2);
	return res;
}

int main(){
	scanf("%d", &n);
	for(int i = 1; i < n; i++){
		int u, v;
		scanf("%d %d", &u, &v);
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	dfs(1, 0);
	scanf("%d", &q);
	while(q--){
		int u, v;
		scanf("%d %d", &u, &v);
		printf("%d\n", solve(u, v));
	}
}
