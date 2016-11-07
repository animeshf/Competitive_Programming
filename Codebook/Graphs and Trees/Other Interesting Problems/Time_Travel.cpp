/*
	Snackdown Finals 2015
*/

#include "bits/stdc++.h"
using namespace std;

const int N   = 1e5 + 5;
const int INF = 1e8 + 8;

int t, n, k;
int arr[N], mark[N], sub[N], depth[N], mxdep[N], dp[N];
vector < int > adj[N], lev[N];

inline void dfs(int u, int p){
	sub[u] = 1;
	mxdep[u] = depth[u];
	for(int v : adj[u]){
		if(v != p){
			depth[v] = depth[u] + 1;
			dfs(v, u);
			sub[u] += sub[v];
			mxdep[u] = max(mxdep[u], mxdep[v]);
		}
	}
	if(mark[depth[u]]) lev[depth[u]].push_back(u);
}


int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d", &t);
	while(t--){
		scanf("%d", &n);
		for(int i = 1; i <= n; i++){
			adj[i].clear();
			lev[i].clear();
			dp[i] = INF;
		}
		dp[n + 1] =  0;
		for(int i = 1; i < n; i++){
			int u, v;
			scanf("%d %d", &u, &v);
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		scanf("%d", &k);
		for(int i = 1; i <= k; i++){
			scanf("%d", arr + i);
			mark[arr[i]] = 1;
		}
		depth[1] = 1, dfs(1, -1);
		for(int i = n; i >= 1; i--){
			if(!lev[i].size()){
				dp[i] = dp[i + 1];
				continue;
			}
			for(int x : lev[i]){
				dp[i] = min(dp[i], sub[x] + dp[mxdep[x] + 1]);
			}
		}
		printf("%d\n", dp[1]);
		for(int i = 1; i <= k; i++){
			mark[arr[i]] = 0;
		}
	}
}
