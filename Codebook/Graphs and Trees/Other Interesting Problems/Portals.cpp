/*
	Codeforces Portals
*/

#include <bits/stdc++.h>
using namespace std;
const int MAX = 1e5 + 5;
const long long INF = (1LL << 60LL);
typedef pair < int, int > pii;
int n, m, k, in_queue[MAX], portal[MAX], parent[MAX];
long long dist[MAX];
vector < pii > adj[MAX];
vector < pair < long long, pii > > edges;
int find(int x){
	if(parent[x] == x) return x;
	return parent[x] = find(parent[x]);
}
int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d %d", &n, &m);
	for(int i = 1; i <= m; i++){
		int u, v, w;
		scanf("%d %d %d", &u, &v, &w);
		adj[u].push_back({v, w});
		adj[v].push_back({u, w});
	}
	scanf("%d", &k);
	queue < int > spfa;
	for(int i = 1; i <= n; i++) dist[i] = INF;
	for(int i = 1; i <= k; i++){
		int x;
		scanf("%d", &x);
		portal[x] = parent[x] = x;
		dist[x] = 0;
		in_queue[x] = true, spfa.push(x);
	}
	while(!spfa.empty()){
		int node = spfa.front();
		spfa.pop();
		in_queue[node] = false;
		for(auto next : adj[node]){
			if(dist[node] + next.second < dist[next.first]){
				dist[next.first] = dist[node] + next.second;
				portal[next.first] = portal[node];
				if(!in_queue[next.first]){
					spfa.push(next.first);
					in_queue[next.first] = true;
				}
			}
		}
	}
	for(int node = 1; node <= n; node++){
		for(auto next : adj[node]){
			if(portal[node] != portal[next.first])
				edges.push_back({dist[node] + next.second + dist[next.first], {portal[node], portal[next.first]}});
		}
	}
	long long res = dist[1];
	sort(edges.begin(), edges.end());
	for(auto e : edges){
		long long weight = e.first;
		int u = find(e.second.first), v = find(e.second.second);
		if(u != v){
			res += weight;
			parent[v] = u;
		}
	}
	printf("%lld\n", res);
}
