// LIGHTOJ
#include "bits/stdc++.h"
using namespace std;

// Dinic's Maxflow Template
const int INF = 1000000000;

struct Edge {
	int from, to, cap, flow, index;
	Edge(int from, int to, int cap, int flow, int index) :
	from(from), to(to), cap(cap), flow(flow), index(index) {}
};

struct Dinic{
	int N;
	vector < vector < Edge > > G;
	vector < Edge * > dad;
	vector < int > Q;

	Dinic(int N) : N(N), G(N), dad(N), Q(N) {}

	void AddEdge(int from, int to, int cap){
		G[from].push_back(Edge(from, to, cap, 0, G[to].size()));
		if (from == to) G[from].back().index++;
		G[to].push_back(Edge(to, from, 0, 0, G[from].size() - 1));
	}

	long long BlockingFlow(int s, int t){
		fill(dad.begin(), dad.end(), (Edge *) NULL);
		dad[s] = &G[0][0] - 1;

		int head = 0, tail = 0;
		Q[tail++] = s;
	
		while(head < tail){
			int x = Q[head++];
			for (int i = 0; i < G[x].size(); i++){
				Edge &e = G[x][i];
				if(!dad[e.to] && e.cap - e.flow > 0){
					dad[e.to] = &G[x][i];
					Q[tail++] = e.to;
				}
			}
		}
		if (!dad[t]) return 0;

		long long totflow = 0;
		for (int i = 0; i < G[t].size(); i++){
			Edge *start = &G[G[t][i].to][G[t][i].index];
			int amt = INF;
			for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]){
				if (!e) { amt = 0; break; }
				amt = min(amt, e->cap - e->flow);
			}
			if (amt == 0) continue;
			for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]) {
				e->flow += amt;
				G[e->to][e->index].flow -= amt;
			}
			totflow += amt;
		}
		return totflow;
	}

	long long GetMaxFlow(int s, int t){
		long long totflow = 0;
		while (long long flow = BlockingFlow(s, t)) totflow += flow;
		return totflow;
	}
};

// End of Dinic's Maxflow

/*
	The min-cut of G(V, E) finds the minimum cost subset E' of E such that in G(V, E \ E'),
	Source S and Sink T are not connected! This is exactly what we want in this problem.
	However, we can also remove a vertex instead of an edge. 
	Hence, we will split each vertex into 2 nodes and add an edge with weight equal to 
	the cost of removing that vertex.

	In this graph, min cut will give us the answer. And since we know that min cut = max flow,
	the problem becomes easy to solve!
*/


int test, m, w;

int main(){
	freopen("ioi.in", "r", stdin);
	ios :: sync_with_stdio(false);
	cin >> test;
	for(int qq = 1; qq <= test; qq++){
		cin >> m >> w;
		Dinic mf(m + m + 2);
		int source = 1, sink = m;
		for(int i = 2; i < m; i++){
			int cost; cin >> cost;
			mf.AddEdge(i, i + m, cost);
		}
		for(int i = 1; i <= w; i++){
			int u, v, c;
			cin >> u >> v >> c;
			int nu = u, nv = v;
			if(u >= 2 and u <= m - 1) nu = u + m;
			mf.AddEdge(nu, v, c);
			if(v >= 2 and v <= m - 1) nv = v + m;
			mf.AddEdge(nv, u, c);
		}
		cout << "Case " << qq << ": " << mf.GetMaxFlow(source, sink) << '\n';
	}
}
