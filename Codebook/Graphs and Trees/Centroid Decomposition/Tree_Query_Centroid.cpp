/*
	Given a Tree T and Q queries, each of the form (v, l) :
	Each query returns number of vertices u such that distance(v, u) <= l

	Centroid Decomposition!
*/


#include <bits/stdc++.h>
using namespace std;

const int MAX = 100005;
const int  LN = 20;

int n, q;
vector < pair < int, long long > > adj[MAX];
int done[MAX], parent[MAX], depth[MAX], sub[MAX];
long long dist[LN][MAX];
vector < long long > val[MAX], valp[MAX];

void dfs(int u, int p){
	sub[u] = 1;
	for(auto v : adj[u]){
		if(v.first == p || done[v.first]) continue;
		dfs(v.first, u);
		sub[u] += sub[v.first];
	}
}

int find(int u, int p, int tar){
	for(auto v : adj[u]){
		if(v.first == p || done[v.first]) continue;
		if(sub[v.first] > tar) return find(v.first, u, tar);
	}
	return u;
}

void explore(int u, int p, long long d, int cur){
	val[cur].push_back(d);
	dist[depth[cur]][u] = d;
	for(auto v : adj[u]){
		if(done[v.first] || v.first == p) continue;
		explore(v.first, u, d + v.second, cur);
	}
}

void decompose(int u, int p){
	dfs(u, p);
	int centroid = find(u, p, sub[u] / 2);
	parent[centroid] = p;
	done[centroid]   = true;
	depth[centroid]  = (p == 0) ? (0) : (depth[p] + 1);
	explore(centroid, p, 0, centroid);
	sort(val[centroid].begin(), val[centroid].end());
	for(auto v : adj[centroid]){
		if(done[v.first]) continue;
		decompose(v.first, centroid);
	}
}

void preprocess(){
	for(int i = 1; i <= n; i++){
		int cur = i;
		while(parent[cur] != 0){
			valp[cur].push_back(dist[depth[parent[cur]]][i]);
			cur = parent[cur];
		}
	}
	for(int i = 1; i <= n; i++) sort(valp[i].begin(), valp[i].end());
}

int query(int v, long long l){
	int ans = upper_bound(val[v].begin(), val[v].end(), l) - val[v].begin();
	int cur = v;
	while(parent[cur] != 0){
		long long d = dist[depth[parent[cur]]][v];
		int tot = upper_bound(val[parent[cur]].begin(), val[parent[cur]].end(), l - d) - val[parent[cur]].begin();
		int ext = upper_bound(valp[cur].begin(), valp[cur].end(), l - d) - valp[cur].begin();
		ans += tot - ext;
		cur = parent[cur];
	}
	return ans;
}

int main(){

	scanf("%d %d\n", &n, &q);
	for(int i = 1; i < n; i++){
		int u, v;
		long long l;
		scanf("%d %d %lld\n", &u, &v, &l);
		adj[u].push_back(make_pair(v, l));
		adj[v].push_back(make_pair(u, l));
	}

	decompose(1, 0);
	preprocess();
	while(q--){
		int v;
		long long l;
		scanf("%d %lld\n", &v, &l);
		printf("%d\n", query(v, l));
	}
}
