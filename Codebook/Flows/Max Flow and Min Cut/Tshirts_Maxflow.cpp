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

const int N = 105;

int test, n, m;
map < string, int > tshirt;

int main(){
	freopen("ioi.in", "r", stdin);
	ios :: sync_with_stdio(false);
	cin >> test;

	tshirt["XXL"] = 1, tshirt["XL"] = 2, tshirt["L"]  = 3;
	tshirt["M"]   = 4, tshirt["S"]  = 5, tshirt["XS"] = 6;
	
	for(int qq = 1; qq <= test; qq++){
		cin >> n >> m;
		Dinic mf(m + 8);
		int source = 0, sink = m + 7;
		for(int i = 1; i <= 6; i++){
			mf.AddEdge(source, i, n);
		}
		for(int i = 7; i <= m + 6; i++){
			string str; 
			cin >> str, mf.AddEdge(tshirt[str], i, 1);
			cin >> str, mf.AddEdge(tshirt[str], i, 1);
			mf.AddEdge(i, sink, 1);
		}
		cout << "Case " << qq << ": ";
		if(mf.GetMaxFlow(source, sink) == m) cout << "YES\n";
		else cout << "NO\n";
	}
}
