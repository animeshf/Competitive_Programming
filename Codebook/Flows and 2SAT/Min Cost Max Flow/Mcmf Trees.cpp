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
	In this problem, we are given 2 rooted trees and asked : What is the minimum number of leaves
	you can add to either of these trees such that they are isomorphic i.e. structurally similar?

	We will do the following dp :
	dp[u][v] = Minimum number of nodes to add such that subtree rooted at (u) in T1 and subtree
	rooted at (v) in T2 are isomorphic.

	Answer : dp[0][0]

	Base Cases : If (u) and (v) are both leaves, dp[u][v] = 0

	Recurrence :-
	Let u have n children c1, c2...cn, and v have m children d1, d2, d3.... dm.

	You can match any c_i with any d_j with cost dp[c_i][d_j].
	You need to match all c_i with some d_j.

    Notice that if m > n, some c_is won't be matched, so you must add that entire subtree
	as it is. Similarly, if n > m, some d_j's won't be matched hence you must add the entire
	subtree as it is to make it isomorphic. This can be handled by taking max(n, m) nodes in
	each partite (thus one of the partites will have some null nodes). The cost of edges with
	one end point as a null node will be the subtree size of the node at to the other endpoint.

	After making this bipartite graph, you can run min-cost-max-flow to find the minimum
	cost matching and that would be the desired dp[u][v] value.

	Complexity : O(N ** 3 log N). Summation of edges added = O(N ** 2), and since mcmf works in
	O(V * E * log E), complexity equals O(N ** 3 log N)
*/

const int N = 200;
int t, n1, n2;
int sub1[N], sub2[N], dp[N][N];
vector < int > adj1[N], adj2[N];

inline void dfs1(int u){
	sub1[u] = 1;
	for(int i = 0; i < (int) adj1[u].size(); i++){
		int v = adj1[u][i];
		dfs1(v);
		sub1[u] += sub1[v];
	}
}

inline void dfs2(int u){
	sub2[u] = 1;
	for(int i = 0; i < (int) adj2[u].size(); i++){
		int v = adj2[u][i];
		dfs2(v);
		sub2[u] += sub2[v];
	}
}

inline int solve(int u, int v){
	if(dp[u][v] != -1) return dp[u][v];
	int ucnt = adj1[u].size(), vcnt = adj2[v].size();
	int match = max(ucnt, vcnt); // Number of nodes in each partite
	if(match == 0) return dp[u][v] = 0; // Base Case
	for(int i = 1; i <= match; i++)
		for(int j = 1; j <= match; j++)
			if(i <= ucnt and j <= vcnt) solve(adj1[u][i - 1], adj2[v][j - 1]);
	MinimumCostMaximumFlow mcmf(match * 2 + 2);
	int source = 0, sink = match * 2 + 1;
	for(int i = 1; i <= match; i++) mcmf.add(source, i, 1, 0);
	for(int i = 1; i <= match; i++){
		for(int j = 1; j <= match; j++){
			if(i <= ucnt and j <= vcnt) // Edge between two children
				mcmf.add(i, match + j, 1, dp[adj1[u][i - 1]][adj2[v][j - 1]]);
			else if(i > ucnt) // Edge between null node and proper node
				mcmf.add(i, match + j, 1, sub2[adj2[v][j - 1]]);
			else // Edge between null node and proper node
				mcmf.add(i, match + j, 1, sub1[adj1[u][i - 1]]);
		}
	}
	for(int i = match + 1; i <= match * 2; i++) mcmf.add(i, sink, 1, 0);
	pair < long long, long long > res = mcmf.getMaxFlow(source, sink);
	assert(res.second == match); // All nodes must be paired
	return dp[u][v] = res.first;
}

int main(){
	cin >> t;
	for(int qq = 1; qq <= t; qq++){
		cin >> n1;
		for(int i = 0; i < N; i++){
			adj1[i].clear();
			adj2[i].clear();
			sub1[i] = 0;
			sub2[i] = 0;
		}
		for(int i = 1; i <= n1; i++){
			int p; cin >> p;
			adj1[p].push_back(i);
		}
		cin >> n2;
		for(int i = 1; i <= n2; i++){
			int p; cin >> p;
			adj2[p].push_back(i);
		}
		dfs1(0); dfs2(0);
		memset(dp, -1, sizeof dp);
		int ans = solve(0, 0);
		cout << "Case " << qq << ": " << ans << '\n';
	}
}
