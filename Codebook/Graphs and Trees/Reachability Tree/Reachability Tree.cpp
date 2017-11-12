// Undirected Graph: Each node has a color, each edge has a weight.
// Q Queries: (u, v, k): Compute a (not necessarily simple) path from
// u to v, that visits at least k different colors, and minimizes the
// weight of the maximum edge on the path. N, Q <= 100000

// Solution: Construct the "reachability tree" of the given tree.
// We define the reachability tree as a rooted binary tree with 2N−1
// nodes such that each leaf represents a node in the original tree,
// and each internal node represents a set of nodes in the original tree
// that are reachable from each other using edges up to a certain length.
// In other words, each internal node is associated with some value, say k
// such that the leaves under it represent a maximal set of nodes in the
// original tree that are reachable from each other using edges of length <= k
// This tree can be constructed in O(N log N)


#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 5;
const int LN = 21;

struct edge{
	int u, v, w;
	friend bool operator < (edge x, edge y){
		return (x.w < y.w);
	}
}edges[N];

int n, m, q, root;
vector < int > adj[N];
int t[N], val[N];
int tin[N], distinct[N], deg[N], parent[N];
int timer, head[N];
vector < int > values;
vector < int > nodes[N];
int depth[N], dp[N][LN];

inline int find(int x){
	if(parent[x] == x) return x;
	return parent[x] = find(parent[x]);
}

inline int lca(int x, int y){
	if(depth[x] < depth[y]) swap(x, y);
	for(int i = LN - 1; i >= 0; i--){
		if(depth[x] - (1 << i) >= depth[y])
			x = dp[x][i];
	}
	if(x == y) return x;
	for(int i = LN - 1; i >= 0; i--){
		if(dp[x][i] != dp[y][i]){
			x = dp[x][i];
			y = dp[y][i];
		}
	}
	return dp[x][0];
}

inline void dfs_init(int u, int p, int rt){
	dp[u][0] = p;
	tin[u] = ++timer;
	head[u] = rt;
	for(int i = 1; i < LN; i++) dp[u][i] = dp[dp[u][i - 1]][i - 1];
	for(int v : adj[u]){
		if(v != p){
			depth[v] = depth[u] + 1;
			dfs_init(v, u, rt);
		}
	}
}

inline void dfs_find(int u, int p){
	for(int v : adj[u]){
		if(v != p){
			dfs_find(v, u);
			distinct[u] += distinct[v];
		}
	}
}

inline bool cmp(int x, int y){
	return (tin[x] < tin[y]);
}

inline void build(){
	sort(edges + 1, edges + 1 + m);
	for(int i = 1; i <= 3 * n; i++) parent[i] = i;
	root = n;
	for(int i = 1; i <= m; i++){
		int u = find(edges[i].u), v = find(edges[i].v), w = edges[i].w;
		if(u == v) continue;
		++root;
		parent[u] = parent[v] = root;
		val[root] = w;
		adj[root].push_back(u);
		adj[root].push_back(v);
		deg[u]++, deg[v]++;
	}
	for(int i = root; i >= 1; i--){
		if(!deg[i]) dfs_init(i, i, i);
	}
	for(int i = 1; i <= n; i++){
		if(nodes[i].size()){
			sort(nodes[i].begin(), nodes[i].end(), cmp);
			for(int j = 0; j < (int) nodes[i].size() - 1; j++){
				int x = nodes[i][j], y = nodes[i][j + 1];
				if(head[x] == head[y]) distinct[lca(x, y)]--;
				distinct[x]++;
			}
			distinct[nodes[i][(int) nodes[i].size() - 1]]++;
		}
	}
	for(int i = root; i >= 1; i--){
		if(!deg[i]) dfs_find(i, i);
	}
}

void solve(int u, int v, int k){
	if((head[u] != head[v]) || (distinct[head[u]] < k)){
		printf("-1\n");
		return;
	}
	int lc = lca(u, v);
	if(distinct[lc] >= k){
		printf("%d\n", val[lc]);
		return;
	}
	int node = lc;
	for(int i = LN - 1; i >= 0; i--){
		if(distinct[dp[node][i]] < k){
			node = dp[node][i];
		}
	}
	node = dp[node][0];
	printf("%d\n", val[node]);
}

int main(){
    scanf("%d %d %d", &n, &m, &q);
    for(int i = 1; i <= n; i++){
    	scanf("%d", t + i);
    	values.push_back(t[i]);
    }
    sort(values.begin(), values.end());
    values.resize(unique(values.begin(), values.end()) - values.begin());
    for(int i = 1; i <= n; i++){
    	t[i] = lower_bound(values.begin(), values.end(), t[i]) - values.begin() + 1;
    	nodes[t[i]].push_back(i);
    }
    for(int i = 1; i <= m; i++){
    	scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].w);
    }
    build();
    for(int i = 1; i <= q; i++){
    	int u, v, k;
    	scanf("%d %d %d", &u, &v, &k);
    	solve(u, v, k);
    }
}