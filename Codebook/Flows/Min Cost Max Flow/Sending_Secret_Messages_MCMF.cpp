// LightOJ
#include "bits/stdc++.h"
using namespace std;


// Min cost Max flow template

struct MinimumCostMaximumFlow {

    typedef long long Flow;
    typedef long long Cost;
    static const Cost infiniteDistance = 1e18;
    static const Cost EPS = 1e-7;
    static const Flow infiniteFlow = 1e18;
    
    struct Edge{
        int u, v;
        Flow mx, c;
        Cost w;
        Edge(int u, int v, Flow mx, Flow c, Cost w) : u(u), v(v), mx(mx), c(c), w(w) {}
    };  
    
    vector < Edge > e;
    vector < vector < int > > g;  
    int n, source, sink, *prev;
    Cost *dist;

    MinimumCostMaximumFlow(int n) : n(n){
        dist = (Cost*)malloc(sizeof(Cost)*n);
        prev = (int*) malloc(sizeof(int)*n);
        g.resize(n);
    }

    ~MinimumCostMaximumFlow(){
        free(dist);
        free(prev);
        g.clear();
    }

    inline void add(int u, int v, Flow c, Cost w){
        g[u].push_back(e.size())	;
        e.push_back(Edge(u, v, 0, c, w));
        // For residual graph
        g[v].push_back(e.size());
        e.push_back(Edge(v, u, 0, 0, -w));
    }   

    inline pair < Cost, Flow > getMaxFlow(int source, int sink){
        this -> source = source;
        this -> sink = sink;
        for(int i = 0; i < (int) e.size(); i++) e[i].mx = 0;
        Flow flow = 0;
        Cost cost = 0;
        while(bellmanFord()){
            int u = sink;
            Flow pushed = infiniteFlow;
            Cost pushCost = 0;
            while(u != source){   
                int id = prev[u];
                pushed = min(pushed, e[id].c - e[id].mx);
                pushCost += e[id].w;
                u = e[id].u;
            }
            u = sink;
            while(u != source){
                int id = prev[u];
                e[id].mx += pushed;
                e[id ^ 1].mx -= pushed;   
                u = e[id].u;
            }
            flow += pushed;
            cost += pushCost * pushed;
        }
        return make_pair(cost, flow);
    }

    inline bool bellmanFord(){
        for(int i = 0; i < n; ++i) dist[i] = infiniteDistance;
        dist[source] = 0;
        for(int k = 0; k < n; ++k){
            bool update = false;
            for(int id = 0; id < (int) e.size(); ++id){
                int u = e[id].u;
                int v = e[id].v;
                if(dist[u] + EPS >= infiniteDistance) continue;
                Cost w = e[id].w;
                if(e[id].mx < e[id].c && dist[v] > dist[u] + w + EPS){
                    dist[v] = dist[u] + w;
                    prev[v] = id;
                    update = true;
                }   
            }
            if(!update) break;
        }
        return (dist[sink] + EPS) < (infiniteDistance);
    }

    // After running mcmf_max, e[id].mx has the flow which has passed through that edge in the optimal soln
    inline void displayEdges(){
        cout << "******" << '\n';
        for(int i = 0; i < (int) e.size(); ++i)
            cout << e[i].u << " " << e[i].v << " " << e[i].mx << " " << e[i].c << " " << e[i].w <<"\n";
        cout << "******" << '\n';
    }
};
 

/*
	In this question, we are given an undirected graph with each edge having a capacity and a
	cost per unit flow associated with it. We want to send 'P' units of flow from source to sink
	and minimise the cost. There are two things to take care of :-
	
	1) Cost of an edge depends on the flow going through it. - This is fine because in mcmf,
	   cost is always per unit flow.
	2) We don't want to MAXIMISE flow : We just want it to be >= P - To handle this, add a super
	   sink and connect it to the original sink with weight = P. Now the max flow in this graph
	   would be P, and the cost to achieve this would be the minimum cost!
*/


const int N = 200;
int t, n, m, p;

int main(){
	freopen("ioi.in", "r", stdin);
	cin >> t;
	for(int qq = 1; qq <= t; qq++){
		cin >> n >> m >> p;
		MinimumCostMaximumFlow mcmf(n + 2);
		int source = 1, sink = n + 1;
		for(int i = 1; i <= m; i++){
			int u, v, w, c;
			cin >> u >> v >> w >> c;
			mcmf.add(u, v, w, c);
			mcmf.add(v, u, w, c);
		}
		mcmf.add(n, sink, p, 0);
		mcmf.add(sink, n, p, 0);
		pair < long long, long long > res = mcmf.getMaxFlow(source, sink);
		cout << "Case " << qq  << ": ";
		if(res.second != p) cout << "impossible";
		else cout << res.first;
		cout << "\n";
	}
}
