/*
	ans[u] = Longest path in a DAG after removing vertex (u)
	POI Rally
*/

#include "bits/stdc++.h"
using namespace std;

const int N = 5e5 + 5;
const int INF = 1e8 + 8;

int n, m;
int in_degree[N], order[N], pos[N];
int in[N], out[N], ans[N];
int tree[N * 4];
vector < int > adj[N];

inline void topological_sort(){
	int cur_time = 0;
	queue < int > q;
	for(int i = 1; i <= n; i++){
		if(!in_degree[i]) q.push(i);
	}

	while(!q.empty()){
		int u = q.front();
		q.pop();
		order[++cur_time] = u;
		pos[u] = cur_time;
		for(int j = 0; j < (int) adj[u].size(); j++){
			int v = adj[u][j];
			in_degree[v]--;
			if(!in_degree[v]) q.push(v);
		}
	}

	for(int i = 1; i <= n; i++){
		int u = order[i];
		for(int j = 0; j < (int) adj[u].size(); j++){
			int v = adj[u][j];
			in[v] = max(in[v], in[u] + 1);
		}
	}

	for(int i = n; i >= 1; i--){
		int u = order[i];
		for(int j = 0; j < (int) adj[u].size(); j++){
			int v = adj[u][j];
			out[u] = max(out[u], out[v] + 1);
		}
	}
}

inline void update(int i, int l, int r, int qs, int qe, int val){
	if(l > qe || r < qs) return;
	if(l >= qs && r <= qe){
		tree[i] = max(tree[i], val);
		return;
	}
	int mid = (l + r) >> 1;
	update(i * 2, l, mid, qs, qe, val);
	update(i * 2 + 1, mid + 1, r, qs, qe, val);
}

inline int query(int i, int l, int r, int pos){
	if(l == r) return tree[i];
	int mid = (l + r) >> 1;
	if(mid >= pos) return max(tree[i], query(i * 2, l, mid, pos));
	else return max(tree[i], query(i * 2 + 1, mid + 1, r, pos));
}

int main(){
	scanf("%d %d", &n, &m);
	for(int i = 1; i <= m; i++){
		int u, v;
		scanf("%d %d", &u, &v);
		adj[u].push_back(v);
		in_degree[v]++;
	}

	topological_sort();

	for(int u = 1; u <= n; u++){
		for(int j = 0; j < (int) adj[u].size(); j++){
			int v = adj[u][j];
			int l = pos[u] + 1, r = pos[v] - 1;
			if(l <= r) update(1, 1, n, l, r, in[u] + 1 + out[v]);
		}
	}

	for(int i = 1; i <= n; i++){
		ans[i] = query(1, 1, n, pos[i]);
	}

	int prefix_max = 0;
	for(int i = 2; i <= n; i++){
		int u = order[i];
		prefix_max = max(prefix_max, in[order[i - 1]]);
		ans[u] = max(ans[u], prefix_max);
	}

	int suffix_max = 0;
	for(int i = n - 1; i >= 1; i--){
		int u = order[i];
		suffix_max = max(suffix_max, out[order[i + 1]]);
		ans[u] = max(ans[u], suffix_max);
	}

	int min_val = INF, min_idx = -1;
	for(int i = 1; i <= n; i++){
		if(ans[i] < min_val){
			min_val = ans[i];
			min_idx = i;
		}
	}
	
	printf("%d %d\n", min_idx, min_val);
}
