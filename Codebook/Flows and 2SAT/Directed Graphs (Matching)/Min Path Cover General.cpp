// LIGHTOJ
#include "bits/stdc++.h"
using namespace std;

const int N = 1005;
const int M = N * N;

/*
	In this problem we need to find a min. path cover in a general dir.graph, with no
	requirement of vertex-disjointedness. Hence, just compress the directed graph
	into a dag by condensing scc's to a single node. After this, perform transitive closure
	on the dag.

	Finally : min path cover = count_scc - max_matching()

	Note that if vertex disjointedness was required, we would NOT do a transitive closure.
*/

/*
	Hopcroft Karp Max Matching in O(E * sqrt(V))
	N = Number of Nodes, M = Number of Edges
	n1 = Size of left partite, n2 = Size of right partite
	Nodes are numbered from [0, n1 - 1] and [0, n2 - 1]

	init(n1, n2) declares the two partite sizes and resets arrays
	addEdge(x, y) adds an edge between x in left partite and y in right partite
	maxMatching() returns the maximum matching

	Maximum Matching = Minimum Vertex Cover (Konig's Theorem)
	N - Maximum Matching = Maximal Independent Set
*/

int n1, n2, edges, last[N], previous[M], head[M];
int matching[N], dist[N], Q[N];
bool used[N], vis[N];

inline void init(int _n1, int _n2) {
	n1 = _n1;
	n2 = _n2;
	edges = 0;
	fill(last, last + n1, -1);
}

inline void addEdge(int u, int v) {
	head[edges] = v;
	previous[edges] = last[u];
	last[u] = edges++;
}

inline void bfs() {
	fill(dist, dist + n1, -1);
	int sizeQ = 0;
	for(int u = 0; u < n1; ++u){
		if(!used[u]){
			Q[sizeQ++] = u;
			dist[u] = 0;
		}
	}
	for(int i = 0; i < sizeQ; i++){
		int u1 = Q[i];
		for(int e = last[u1]; e >= 0; e = previous[e]){
			int u2 = matching[head[e]];
			if(u2 >= 0 && dist[u2] < 0){
				dist[u2] = dist[u1] + 1;
				Q[sizeQ++] = u2;
			}
		}
	}
}

inline bool dfs(int u1) {
	vis[u1] = true;
	for(int e = last[u1]; e >= 0; e = previous[e]){
		int v = head[e];
		int u2 = matching[v];
		if(u2 < 0 || !vis[u2] && dist[u2] == dist[u1] + 1 && dfs(u2)){
			matching[v] = u1;
			used[u1] = true;
			return true;
		}
	}
	return false;
}

inline int maxMatching() {
	fill(used, used + n1, false);
	fill(matching, matching + n2, -1);
	for(int res = 0; ;){
		bfs();
		fill(vis, vis + n1, false);
		int f = 0;
		for(int u = 0; u < n1; ++u)
			if(!used[u] && dfs(u))
				++f;
		if(!f) return res;
		res += f;
	}
}

// End of Hopcroft Karp Template

int t, n, m, scc;
int visit[N], component[N];
vector < int > adj[N], tra[N], dag[N];
stack < int > order;

inline void dfs1(int u){
	for(int i = 0; i < (int) adj[u].size(); i++){
		int v = adj[u][i];
		if(!visit[v]){
			visit[v] = true;
			dfs1(v);
		}
	}
	order.push(u);
}

inline void dfs2(int u){
	component[u] = scc;
	for(int i = 0; i < (int) tra[u].size(); i++){
		int v = tra[u][i];
		if(!component[v]) dfs2(v);
	}
}

inline void explore(int root){
	queue < int > bfs;
	bfs.push(root);
	while(!bfs.empty()){
		int node = bfs.front();
		bfs.pop();
		if(node != root) addEdge(root - 1, node - 1);
		for(int i = 0; i < (int) dag[node].size(); i++){
			int next = dag[node][i];
			bfs.push(next);
		}
	}
}

int main(){
	freopen("ioi.in", "r", stdin);
	cin >> t;
	for(int qq = 1; qq <= t; qq++){
		cin >> n >> m;

		// Cleanup the arrays
		for(int i = 1; i <= n; i++){
			adj[i].clear();
			tra[i].clear();
			dag[i].clear();
			visit[i] = false;
			component[i] = 0;
		}
		while(!order.empty()) order.pop();
		scc = 0;

		// Make graph and its transpose
		for(int i = 1; i <= m; i++){
			int u, v;
			cin >> u >> v;
			adj[u].push_back(v);
			tra[v].push_back(u);
		}

		// Kosaraju Phase 1
		for(int i = 1; i <= n; i++){
			if(!visit[i]){
				visit[i] = true;
				dfs1(i);
			}
		}

		// Kosaraju Phase 2
		while(!order.empty()){
			int node = order.top();
			order.pop();
			if(!component[node]){
				++scc;
				dfs2(node);
			}
		}

		// Add edges between scc'sO
		for(int u = 1; u <= n; u++){
			for(int i = 0; i < (int) adj[u].size(); i++){
				int v = adj[u][i];
				if(component[u] != component[v])
					dag[component[u]].push_back(component[v]);
			}
		}

		// Remove Multiple Edges : Important step in SCC Condensation!
		for(int i = 1; i <= scc; i++){
			sort(dag[i].begin(), dag[i].end());
			dag[i].resize(unique(dag[i].begin(), dag[i].end()) - dag[i].begin());
		}

		init(scc, scc); // Initialise Bipartite graph with 'count_scc' nodes
		for(int i = 1; i <= scc; i++) explore(i); // Transitive Closure

		int ans = scc - maxMatching(); // Minimum Path Cover
		cout << "Case " << qq << ": " << ans << "\n";
	}
}
