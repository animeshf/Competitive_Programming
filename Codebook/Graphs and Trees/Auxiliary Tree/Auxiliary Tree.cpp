// Kingdom and Cities - Codeforces.
// Solution: Auxiliary Tree

#include "bits/stdc++.h"
using namespace std;

const int N   = 1e5 + 5;
const int LN  = 18;
const int INF = 1e8 + 8;

int n, q, cur_time, len, ans;
int tin[N], tout[N], depth[N], parent[N], val[N], dp[LN][N];
bool important[N];
vector < int > adj[N], aux[N];
vector < int > nodes;

inline void dfs_prep(int u, int p){
	tin[u] = ++cur_time;
	dp[0][u] = parent[u] = p;
	for(int i = 1; i < LN; i++) dp[i][u] = dp[i - 1][dp[i - 1][u]];
	for(int v : adj[u]){
		if(v != p){
			depth[v] = depth[u] + 1;
			dfs_prep(v, u);
		}
	}
	tout[u] = cur_time;
}

inline int lca(int u, int v){
	if(depth[u] < depth[v]) swap(u, v);
	for(int i = LN - 1; i >= 0; i--){
		if(depth[u] - (1 << i) >= depth[v])
			u = dp[i][u];
	}
	if(u == v) return u;
	for(int i = LN - 1; i >= 0; i--){
		if(dp[i][u] != dp[i][v])
			u = dp[i][u], v = dp[i][v];
	}
	return parent[u];
}

inline bool compare(int u, int v){
	return (tin[u] < tin[v]);
}

inline void clean(vector < int > &x){
	sort(x.begin(), x.end(), compare);
	x.resize(unique(x.begin(), x.end()) - x.begin());
	len = (int) nodes.size();
}

inline void dfs(int u){
	val[u] = INF;
	int min_val = INF, noob_child = 0;
	for(int v : aux[u]){
		dfs(v);
		if(val[v] != INF) ++noob_child;
		min_val = min(min_val, val[v]);
	}
	if(!important[u]){
		if(noob_child > 1) ans += 1;
		else val[u] = min_val;
	}
	else{
		val[u] = 1;
		ans += noob_child;
	}
}

inline bool is_ancestor(int u, int v){
	return ((tin[v] >= tin[u]) && (tin[v] <= tout[u]));
}

inline void solve(bool rekt){
	if(rekt){
		printf("-1\n");
		return;
	}
	clean(nodes);
	for(int i = 0; i < len - 1; i++){
		int lc = lca(nodes[i], nodes[i + 1]);
		nodes.push_back(lc);
	}
	clean(nodes);
	stack < int > ancestors;
	int root = nodes[0];
	ancestors.push(root);
	for(int i = 1; i < len; i++){
		while(!is_ancestor(ancestors.top(), nodes[i]))
			ancestors.pop();
		int p = ancestors.top();
		aux[p].push_back(nodes[i]);
		ancestors.push(nodes[i]);
	}
	ans = 0; dfs(root);
	printf("%d\n", ans);
	for(int node : nodes) aux[node].clear();
}

int main(){
	cin >> n;
	for(int i = 1; i < n; i++){
		int u, v;
		cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	dfs_prep(1, 1);
	cin >> q;
	while(q--){
		int k;
		cin >> k;
		for(int i = 1; i <= k; i++){
			int node;
			cin >> node;
			important[node] = true;
			nodes.push_back(node);
		}
		bool rekt = false;
		for(int node : nodes){
			if((parent[node] != node) && (important[parent[node]]))
				rekt = true;
		}
		solve(rekt);
		for(int node : nodes) important[node] = false;
		nodes.clear();
	}
}