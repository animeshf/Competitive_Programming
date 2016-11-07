#include <bits/stdc++.h>
#define rf freopen("inp.in", "r", stdin)
using namespace std;

const int MAX = 10005;

int t, n, x, y, a[MAX], b[MAX], c[MAX];
int depth[MAX], heavy[MAX], root[MAX], parent[MAX], sub[MAX];
int edgeToNode[MAX], nodeToEdge[MAX], pos[MAX], tree[MAX << 2];
vector < pair < int, int > > adj[MAX];
char str[MAX];

inline void dfs(int u, int p){
	sub[u] = 1;
	int mx = 0;
	for(int i = 0; i < adj[u].size(); i++){
		int v = adj[u][i].first;
		if(v == p) continue;
		edgeToNode[adj[u][i].second] = v;
		nodeToEdge[v] = adj[u][i].second;
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

inline void update(int node, int l, int r, int idx, int val){
	if(l == r){
		tree[node] = val;
		return;
	}
	int mid = l + r >> 1;
	if(mid >= idx) update(node + node, l, mid, idx, val);
	else update(node + node + 1, mid + 1, r, idx, val);
	tree[node] = max(tree[node + node], tree[node + node + 1]);
}

inline int query(int node, int l , int r, int qs, int qe){
	if(l > qe or r < qs)    return 0;
	if(l >= qs and r <= qe) return tree[node];
	int mid = l + r >> 1;
	return max( query(node + node, l, mid, qs, qe), query(node + node + 1, mid + 1, r, qs, qe) );
}

inline void hld(){
	dfs(1, 0);
	for(int i = 1, curPos = 0; i <= n; i++){
		if(parent[i] == -1 || heavy[parent[i]] != i){
			for(int j = i; j != -1; j = heavy[j])
				root[j] = i, pos[j]  = ++curPos;
		}
	}
	for(int i = 2; i <= n; i++) update(1, 1, n, pos[i], c[nodeToEdge[i]]);
}

inline int query(int u, int v){
	int mx = 0;
	for(; root[u] != root[v]; v = parent[root[v]]){
		if(depth[root[u]] > depth[root[v]]) swap(u, v);
		mx = max(mx, query(1, 1, n, pos[root[v]], pos[v]));
	}
	if(depth[u] > depth[v]) swap(u, v);
	mx = max(mx, query(1, 1, n, pos[u] + 1, pos[v]));
	return mx;
}

inline void solve(){
	scanf("%d", &n);
	memset(tree, 0, sizeof tree);
	for(int i = 1; i <= n; i++){
		adj[i].clear();
		heavy[i] = parent[i] = edgeToNode[i] = nodeToEdge[i] = -1;
		depth[i] = sub[i] = 0;
	}
	for(int i = 1; i < n; i++){
		scanf("%d %d %d\n", &a[i], &b[i], &c[i]);
		adj[a[i]].push_back(make_pair(b[i], i));
		adj[b[i]].push_back(make_pair(a[i], i));
	}
	hld();
	while(true){
		scanf("%s", str);
		if(str[0] == 'D') break;
		else if(str[0] == 'C'){
			scanf("%d %d\n", &x, &y);
			update(1, 1, n, pos[edgeToNode[x]], y);	
		}
		else{
			scanf("%d %d\n", &x, &y);
			printf("%d\n", query(x, y));
		}
	}
}

int main(){
	rf;
	scanf("%d", &t);
	while(t--) solve();
}