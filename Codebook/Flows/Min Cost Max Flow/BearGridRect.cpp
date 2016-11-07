// Topcoder SRM by Errichto
#include "bits/stdc++.h"
using namespace std;

const int INF = 1e5 + 5;
int M, N;

struct BearGridRect{

 	int row(int i){
 		return i + 1;
 	}

 	int col(int i){
 		return N + i + 1;
 	}

    int rect1(int i){
 		return N + N + i + 1;
 	}

    int rect2(int i){
 		return N + N + M + i + 1;
 	}

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
	};
 	
	vector < int > findPermutation(int n, vector < int > r1, vector < int > r2, vector < int > c1, vector < int > c2, vector < int > cnt){
		N = n, M = (int) r1.size();
		int m = M;
		int source = 0, sink = N + N + M + M + 1;
		MinimumCostMaximumFlow mf(N + N + M + M + 2);
		set < pair < int , int > > edges;
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				int inside_rectangle = -1;
				for(int k = 0; k < m; k++){
					if(r1[k] <= i and r2[k] >= i and c1[k] <= j and c2[k] >= j){
						inside_rectangle = k;
						break;	
					}
				}
				if(inside_rectangle == -1){
					if(edges.count({row(i), col(j)}) == 0){
						mf.add(row(i), col(j), 1, 0);
						edges.insert({row(i), col(j)});
					}
				}
				else{
					if(edges.count({row(i), rect1(inside_rectangle)}) == 0){
						mf.add(row(i), rect1(inside_rectangle), 1, -INF);
						edges.insert({row(i), rect1(inside_rectangle)});
					}
					if(edges.count({rect2(inside_rectangle), col(j)}) == 0){
						mf.add(rect2(inside_rectangle), col(j), 1, 0);
						edges.insert({rect2(inside_rectangle), col(j)});
					}
				}
			}
		}
		for(int i = 0; i < m; i++){
			mf.add(rect1(i), rect2(i), cnt[i], 0);
		}
		for(int i = 0; i < n; i++){
			mf.add(source, row(i), 1, 0);
			mf.add(col(i), sink, 1, 0);
		}
		int flow = mf.getMaxFlow(source, sink).second;
		vector < int > x(n);
		vector < int > bad(1); bad[0] = -1;
		int mat[300][300];
		memset(mat, 0, sizeof mat);
		for(int i = 0; i < (int) mf.e.size(); i++){
			mat[mf.e[i].u][mf.e[i].v] = mf.e[i].f;
		}
		if(flow != n) return bad;
		for(int i = 0; i < m; i++){
			if(mat[rect1(i)][rect2(i)] != cnt[i]) return bad;
		}
		for(int i = 0; i < n; i++){
			bool ok = false;
			for(int j = 0; j < n; j++)
				if(mat[row(i)][col(j)] == 1) x[i] = j, ok = true;
			if(ok) continue;
			int idx = -1;
			for(int j = 0; j < m; j++)
				if(mat[row(i)][rect1(j)] == 1) idx = j;
			for(int j = 0; j < n; j++)
				if(mat[rect2(idx)][col(j)] == 1){
					x[i] = j;
					mat[rect2(idx)][col(j)] = 0;
					break;
				}
		}
		return x;	
	}
};
