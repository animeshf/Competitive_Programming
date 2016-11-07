// Can you add one edge to a connected graph (with multiedges), to remove all bridges in it?
#include "bits/stdc++.h"
using namespace std;

const int N = 1e5 + 5;

int n, m, cur_time, component_id;
int a[N * 2], b[N * 2], is_bridge[N * 2];
int disc[N], low[N], component[N];
vector < int > adj[N], tree[N];

inline void find_bridges(int u, int p){
	disc[u] = low[u] = ++cur_time;
	for(int i = 0; i < (int) adj[u].size(); i++){
		int edge_id = adj[u][i];
		int v = a[edge_id] ^ b[edge_id] ^ u;
		if(edge_id == p) continue;
		if(!disc[v]){
			find_bridges(v, edge_id);
			low[u] = min(low[u], low[v]);
			if(low[v] > disc[u]) is_bridge[edge_id] = true;
		}
		else low[u] = min(low[u], disc[v]);
	}
}

inline void explore(int u){
	component[u] = component_id;
	for(int i = 0; i < (int) adj[u].size(); i++){
		int edge_id = adj[u][i];
		int v = a[edge_id] ^ b[edge_id] ^ u;
		if(component[v] || is_bridge[edge_id])  continue;
		explore(v);
	}
}

int main(){
	cin >> n >> m;
	for(int i = 1; i <= m; i++){
		cin >> a[i] >> b[i];
		adj[a[i]].push_back(i);
		adj[b[i]].push_back(i);
	}
	find_bridges(1, 0);
	for(int i = 1; i <= n; i++){
		if(!component[i]){
			++component_id;
			explore(i);
		}
	}
	for(int i = 1; i <= m; i++){
		if(is_bridge[i]){
			int u = component[a[i]], v = component[b[i]];
			tree[u].push_back(v);
			tree[v].push_back(u);
		}
	}
	int cnt_leaves = 0;
	for(int i = 1; i <= component_id; i++){
		cnt_leaves += (((int) tree[i].size()) == 1);
	}
	if(cnt_leaves <= 2) cout << "YES\n";
	else cout << "NO\n";
}
