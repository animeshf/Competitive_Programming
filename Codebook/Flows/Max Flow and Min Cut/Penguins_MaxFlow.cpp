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
	From ice berg (i), you can jump atmost m[i] times.
	Hence, make two copies of each berg, and add an edge from in_node to out_node.
	This edge will have capacity m[i] to limit the flow out of the i'th berg.

	Create a super-source and add an edge from it to all in_nodes with capacity
	p[i] i.e number of penguins at the i'th berg.
	
	Add edges between out_node of berg (x) and in_node of berg (y) if you can jump
	from berg (x) to berg (y).

	Now to check whether (x) is a valid meeting point, create a sink and add an edge
	from in_node(x) to sink. Now if the max-flow in this graph is equal to the total
	number of penguins, (x) is valid.

*/


const int N = 105;
const double EPS = 1e-7;

int test, n;
double d;
int x[N], y[N], p[N], m[N];

int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d", &test);
	for(int qq = 1; qq <= test; qq++){
		scanf("%d %lf", &n, &d);
		Dinic mf(n + n + 2);
		int source = 0, sink = n + n + 1;
		int penguins = 0;
		for(int i = 1; i <= n; i++){
			scanf("%d %d %d %d", x + i, y + i, p + i, m + i);
			mf.AddEdge(source, i, p[i]);
			mf.AddEdge(i, i + n, m[i]);
			penguins += p[i];
		}
		for(int i = 1; i <= n; i++){
			for(int j = 1; j <= n; j++){
				if(j == i) continue;
				long long dx = (x[j] - x[i]);
				long long dy = (y[j] - y[i]);
				long long dist = dx * dx + dy * dy;
				if(sqrt(dist) < d || (sqrt(dist) - d < EPS))
					mf.AddEdge(i + n, j, INF);
			}
		}
		vector < int > sol;
		for(int cur_sink = 1; cur_sink <= n; cur_sink++){
			Dinic temp = mf;
			temp.AddEdge(cur_sink, sink, INF);
			if(temp.GetMaxFlow(source, sink) == penguins)
				sol.push_back(cur_sink - 1);
		}
		printf("Case %d:", qq);
		if(sol.empty()) printf(" -1\n");
		else{
			for(int i = 0; i < (int) sol.size(); i++){
				printf(" %d", sol[i]);
			}
			printf("\n");
		}
	}
}
