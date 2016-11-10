// LightOJ
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
	This is a standard max-flow multi-source multi-sink problem with vertex as well as
	edge capacities. The trick for this is to split each vertex into two and add an edge
	between in_node[i] and out_node[i] with capacity = vertex_capacity[i].
	The edges between different regulators are added from out_node[i] to in_node[j]
	
	Running dinic's on this graph suffices to solve this problem efficiently.
*/

const int N = 105;
const double EPS = 1e-7;

int test, n, m, b, d;
int capacity[N];

int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d", &test);
	for(int qq = 1; qq <= test; qq++){
		scanf("%d", &n);
		int source = 0, sink = n + n + 1;
		Dinic mf(n + n + 2);
		for(int i = 1; i <= n; i++){
			scanf("%d", capacity + i);
			mf.AddEdge(i, i + n, capacity[i]);
		}
		scanf("%d", &m);
		for(int i = 1; i <= m; i++){
			int u, v, c;
			scanf("%d %d %d", &u, &v, &c);
			mf.AddEdge(u + n, v, c);
		}
		scanf("%d %d", &b, &d);
		for(int i = 1; i <= b + d; i++){
			int node;
			scanf("%d", &node);
			if(i <= b) mf.AddEdge(source, node, INF);
			else mf.AddEdge(node + n, sink, INF);
		}
		printf("Case %d: %lld\n", qq, mf.GetMaxFlow(source, sink));
	}
}
