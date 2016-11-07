// CF Venture Cup
#include <bits/stdc++.h>
#define rf freopen("inp.in", "r", stdin)
using namespace std;

const int mx = 200005;

vector < int > adjList[mx];
queue  < int > leaves;
int n, k, u, v, res;

bool bad[mx], vis[mx], rem[mx];
int deg[mx], a[mx], val[mx], dp[mx][2];

void find_leaves(int u, int p){
	vis[u] = 1;
	for(int v : adjList[u]){
		if(v == p || bad[v]) continue;
		find_leaves(v, u);
	}
	if(deg[u] == 1) leaves.push(u);
}

void dfs(int u, int p){
	dp[u][1] = dp[u][0] = 0;
	
	for(int v : adjList[u]){
		if(v == p || bad[v]) continue;
		dfs(v, u);
		if(dp[v][0] >= dp[u][0]) dp[u][1] = dp[u][0], dp[u][0] = dp[v][0];
		else if(dp[v][0] > dp[u][1]) dp[u][1] = dp[v][0]; 
	}

	res = max(res, dp[u][0] + dp[u][1] + val[u]);
	dp[u][0] += val[u];
}

void check(int u){
	find_leaves(u, -1);

	while(!leaves.empty()){
		
		int cur = leaves.front();
		leaves.pop();
		rem[cur] = true;
		int spl = -1;
		
		for(int up : adjList[cur]) 
			if(!rem[up] && !bad[up]) spl = up;

		if (spl != -1) {
			val[spl] += val[cur];
			val[cur] = 0;
			deg[spl]--;
			if (deg[spl] == 1) leaves.push(spl);
		}
	}

	dfs(u, -1);
}

bool solve(int cur){
	
	res = 0;
	for(int i = 1; i <= n; i++){
		bad[i] = (a[i] < cur);
		vis[i] = rem[i] = false;
		deg[i] = adjList[i].size();
		val[i] = 1;
	}

	for(int i = 1; i <= n; i++) if(!vis[i] && !bad[i]) check(i);
	return (res >= k);
}


int main(){
	
	rf;
	
	cin.tie(0), ios::sync_with_stdio(false);
	cin >> n >> k;
	
	for(int i = 1 ; i <= n ; i++) cin >> a[i];
	for(int i = 1 ; i < n ; i++){
		cin >> u >> v;
		adjList[u].push_back(v);
		adjList[v].push_back(u);
	}

	int ans = 0;
	for(int i = 20 ; i >= 0 ; i--) 
		if(solve(ans + (1 << i))) ans += (1 << i);
	
	cout << ans << '\n';
}	