/*
	Snackdown 2016 Elimination Round
*/
#include "bits/stdc++.h"
#include "sys/resource.h"
using namespace std;

const int N   = 500005;
const int LN  = 20;
const int INF = 1000000000;

int n, k, q, t, capital, timer;
int tin[N], tout[N], depth[N], sz[N];
int dp[LN][N];
vector < int > adj[N];
vector < int > gang[N];
vector < int > gangs;

// Calculate the required tables
inline void dfs(int u, int p){
	tin[u] = ++timer;
	dp[0][u] = p;
	for(int i = 1; i < LN; i++)
		dp[i][u] = dp[i - 1][dp[i - 1][u]];
	for(int v : adj[u]){
		if(v != p){
			depth[v] = depth[u] + 1;
			dfs(v, u);
		}
	}
	tout[u] = timer;
}

// O(log N) lca function
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
	return dp[0][u];
}

// Returns (k)'th ancestor of u
inline int get_kth(int u, int k){
	int node = u;
	for(int i = LN - 1; i >= 0; i--){
		if(k >= (1 << i))
			node = dp[i][node], k -= (1 << i);
	}
	return node;
}

// Sorts nodes based on increasing start time
inline bool compare(int u, int v){
	return (tin[u] < tin[v]);
}

// Returns if some leaf of gang (g) is present in the subtree rooted at (u)
inline bool is_leaf_in_subtree(int u, int g){
	auto it = lower_bound(gang[g].begin(), gang[g].end(), u, compare);
	if(it == gang[g].end()) return false;
	int val = tin[gang[g][it - gang[g].begin()]];
	return (val >= tin[u] && val <= tout[u]);
}

// Is there some gang-node in the subtree rooted at (u)
inline bool check(int u, bool print){
	vector < int > cur, below;
	bool found_below = false;
	for(int g : gangs){
		if(is_leaf_in_subtree(u, g)){
			if(!print) return true;
			found_below = true;
			int gang_root = gang[g][0];
			if((tin[gang_root] <= tin[u]) || (tin[gang_root] > tout[u])){
				printf("%d\n", depth[capital] - depth[u]);
				return true;
			}
			int dist = depth[gang_root] - depth[u];
			cur.push_back(get_kth(gang_root, dist - 1));
			below.push_back(g);
		}
		else cur.push_back(dp[0][u]);
	}
	if(!print) return false;
	sort(cur.begin(), cur.end());
	cur.resize(unique(cur.begin(), cur.end()) - cur.begin());
	if((int) cur.size() > 1){
		printf("%d\n", depth[capital] - depth[u]);
		return true;	
	}	
	if(found_below){
		int minimum_time = INF, maximum_time = 0;
		int mn = 0, mx = 0;
		for(int g : below){
			if(tin[gang[g][0]] < minimum_time){
				mn = gang[g][0];
				minimum_time = tin[gang[g][0]];
			}
			if(tin[gang[g][sz[g] - 1]] > maximum_time){
				mx = gang[g][sz[g] - 1];
				maximum_time = tin[gang[g][sz[g] - 1]];
			}
		}
		int closest = lca(mn, mx);
		printf("%d\n", (depth[closest] - depth[u]) + (depth[capital] - depth[u]));
		return true;
	}
}

/*
	Finds the first node X in the path from capital --> root which has a gang node
	present in it's subtree : The optimal node is present in subtree(X).
*/

inline void solve(){
	if(check(capital, 0)){
		check(capital, 1);
		return;
	}
	int node = capital;
	for(int i = LN - 1; i >= 0; i--)
		if(!check(dp[i][node], 0))
			node = dp[i][node];
	node = dp[0][node];
	check(node, 1);
}

// Because Codechef...
inline void increase_stack_size(){
	const rlim_t kStackSize = 32 * 1024 * 1024; 
	struct rlimit rl;
	int result;
	getrlimit(RLIMIT_STACK, &rl);
	rl.rlim_cur = kStackSize;
	result = setrlimit(RLIMIT_STACK, &rl);
}

int main(){
	increase_stack_size();
	scanf("%d", &n);
	for(int i = 1; i < n; i++){
		int u, v;
		scanf("%d %d", &u, &v);
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	dfs(1, 1);
	scanf("%d", &k);
	for(int i = 1; i <= k; i++){
		scanf("%d", sz + i);
		int mn = -1, mx = -1;
		for(int j = 1; j <= sz[i]; j++){
			int x;
			scanf("%d", &x);
			gang[i].push_back(x);
		}
		sort(gang[i].begin(), gang[i].end(), compare);
		// Root of the gang is lca of the two extreme nodes.
		int gang_root = lca(gang[i][0], gang[i][sz[i] - 1]);
		gang[i].push_back(gang_root);
		// Sort by visit time and remove duplicates from the vector
		sort(gang[i].begin(), gang[i].end(), compare);
		gang[i].resize(unique(gang[i].begin(), gang[i].end()) - gang[i].begin());
		// Size of gang : Root + leaves
		sz[i] = (int)(gang[i].size());
	}
	scanf("%d", &q);
	while(q--){
		scanf("%d %d", &capital, &t);
		gangs.clear();
		for(int i = 1; i <= t; i++){
			int g;
			scanf("%d", &g);
			gangs.push_back(g);
		}
		solve();
	}
}
