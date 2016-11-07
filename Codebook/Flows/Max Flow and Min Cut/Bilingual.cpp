// GCJ 2015 Elimination Round : Problem C - Bilingual
#include <bits/stdc++.h>
#define rf freopen("gcj.in", "r", stdin)
#define wf freopen("gcj.out", "w", stdout)
using namespace std;

const int N   = 205;

vector < string > sentence[N];
map < string, int > words;
int t, n;

// Dinic's Algorithm to compute Maximum Flow

const int INF = 1000000000;

struct Edge {
	int from, to, cap, flow, index;
	Edge(int from, int to, int cap, int flow, int index) :
	from(from), to(to), cap(cap), flow(flow), index(index) {}
};

struct Dinic {
	int N;
	vector < vector < Edge > > G;
	vector < Edge * > dad;
	vector < int > Q;

	Dinic(int N) : N(N), G(N), dad(N), Q(N) {}

	void AddEdge(int from, int to, int cap) {
		G[from].push_back(Edge(from, to, cap, 0, G[to].size()));
		if (from == to) G[from].back().index++;
		G[to].push_back(Edge(to, from, 0, 0, G[from].size() - 1));
	}

	long long BlockingFlow(int s, int t) {
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

	long long GetMaxFlow(int s, int t) {
		long long totflow = 0;
		while (long long flow = BlockingFlow(s, t)) totflow += flow;
		return totflow;
	}
};

// End of Maximum Flow Template


// Input a sentence
void input(int idx){
	string str, temp;
	getline(cin, str);
	str += " ";
	for(int i = 0; i < (int) str.size(); i++){
		if(str[i] == ' '){
			sentence[idx].push_back(temp);
			temp = "";
		}
		else temp += str[i];
	}
}

int main(){
	rf, wf;
	cin >> t;
	for(int qq = 1; qq <= t; qq++){
		words.clear();
		cin >> n;
		cin.ignore();
		for(int i = 1; i <= n; i++){
			sentence[i].clear();
			input(i);
		}
		int dis = 1;
		for(int i = 1; i <= n; i++){
			for(string w : sentence[i]){
				if(words.find(w) == words.end()){
					words[w] = dis;
					dis += 2;
				}
			}
		}
		Dinic mf(n + dis + 5);
		for(int i = 1; i <= n; i++){
			for(string w : sentence[i])
				mf.AddEdge(i, n + words[w], INF), mf.AddEdge(n + words[w] + 1, i, INF);
		}
		for(auto entry : words){
			mf.AddEdge(n + entry.second, n + entry.second + 1, 1);
		}
		cout << "Case #" << qq << ": " << mf.GetMaxFlow(1, 2) << '\n';
	}
}
