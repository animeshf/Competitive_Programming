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
	Lets binary search on the maximum jump distance.
	
	For a fixed (x), we need to check if we can find two paths from source to sink 
	which have no small-rock vertex in common. This is a standard application of 
	maximum flow : find number of vertex disjoint paths between two nodes.
	This can be done easily by splitting each vertex into two nodes : in and out.

	In this problem, we can use the big-rocks multiple times, hence edge between
	in_node and out_node for big-rocks should have infinite capacity. 
	However, we can use the small-rocks only once, so the edge between in_node and 
	out_node for small-rocks should have unit capacity.
	An edge will be added from out_node(a) to in_node(b) if we can jump from a to b.

	If the maximum flow from source to sink in this graph is greater than 1, we have
	found two valid paths, hence (x) is valid!
*/

const int N = 105;

int test, n, d;
int type[N], dist[N];
string str[N];

inline bool solve(int jump_dist){
	Dinic mf(n + n + 2);
	int source = 0, sink = n + n + 1;
	for(int i = 1; i <= n; i++){
		if(dist[i] <= jump_dist) mf.AddEdge(source, i, 2);
		if(type[i] == 1) mf.AddEdge(i, i + n, 1);
		else mf.AddEdge(i, i + n, 2);
		if(dist[i] + jump_dist >= d) mf.AddEdge(i + n, sink, 2);
	}	
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= n; j++){
			if(i == j) continue;
			if(abs(dist[j] - dist[i]) <= jump_dist)
				mf.AddEdge(i + n, j, 2);
		}
	}
	return (mf.GetMaxFlow(source, sink) >= 2);
}

int main(){
	freopen("ioi.in", "r", stdin);
	ios :: sync_with_stdio(false);
	cin >> test;
	for(int qq = 1; qq <= test; qq++){
		cin >> n >> d;
		for(int i = 1; i <= n; i++){
			cin >> str[i];
			type[i] = (str[i][0] == 'S');
			int val = str[i][2] - '0';
			for(int j = 3; j < (int) str[i].size(); j++)
				val = (val * 10) + (str[i][j] - '0');
			dist[i] = val;
		}
		int l = 1, r = d, ans = d;
		while(l <= r){
			int mid = (l + r) >> 1;
			if(solve(mid)){
				ans = mid;
				r = mid - 1;
			}
			else l = mid + 1;
		}
		cout << "Case " << qq << ": " << ans << '\n';
	}
}
