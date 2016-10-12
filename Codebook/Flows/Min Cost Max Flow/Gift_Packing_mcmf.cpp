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
        Flow f, c;
        Cost w;
        Edge(int u, int v, Flow f, Flow c, Cost w) : u(u), v(v), f(f), c(c), w(w) {}
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
        g[u].push_back(e.size());
        e.push_back(Edge(u, v, 0, c, w));
        // For residual graph
        g[v].push_back(e.size());
        e.push_back(Edge(v, u, 0, 0, -w));
    }   

    inline pair < Cost, Flow > getMaxFlow(int source, int sink){
        this -> source = source;
        this -> sink = sink;
        for(int i = 0; i < (int) e.size(); i++) e[i].f = 0;
        Flow flow = 0;
        Cost cost = 0;
        while(bellmanFord()){
            int u = sink;
            Flow pushed = infiniteFlow;
            Cost pushCost = 0;
            while(u != source){   
                int id = prev[u];
                pushed = min(pushed, e[id].c - e[id].f);
                pushCost += e[id].w;
                u = e[id].u;
            }
            u = sink;
            while(u != source){
                int id = prev[u];
                e[id].f += pushed;
                e[id ^ 1].f -= pushed;   
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
                if(e[id].f < e[id].c && dist[v] > dist[u] + w + EPS){
                    dist[v] = dist[u] + w;
                    prev[v] = id;
                    update = true;
                }   
            }
            if(!update) break;
        }
        return (dist[sink] + EPS) < (infiniteDistance);
    }

    // After running mcmf, e[id].f has the flow which has passed through that edge in the optimal soln
    inline void displayEdges(){
        cout << "******" << '\n';
        for(int i = 0; i < (int) e.size(); ++i)
            cout << e[i].u << " " << e[i].v << " " << e[i].f << " " << e[i].c << " " << e[i].w <<"\n";
        cout << "******" << '\n';
    }
};
 



const int N = 55;

int t, n, m;
int a[N][N];

int main(){
	freopen("ioi.in", "r", stdin);
	cin >> t;
	for(int qq = 1; qq <= t; qq++){
		cin >> n;
		for(int i = 1; i <= n; i++)
			for(int j = 1; j <= n; j++)
				cin >> a[i][j];
		MinimumCostMaximumFlow mcmf(n + n + 2);
		int source = 0, sink = n + n + 1;
		for(int i = 1; i <= n; i++) mcmf.add(source, i, 1, 0);
		for(int i = 1; i <= n; i++)
			for(int j = 1; j <= n; j++)
				mcmf.add(i, j + n, 1, -a[i][j]);
		for(int i = n + 1; i <= n + n; i++) mcmf.add(i, sink, 1, 0);	
		cout << "Case " << qq  << ": " << -mcmf.getMaxFlow(source, sink).first;
		cout << "\n";
	}
}
