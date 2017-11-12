// Find T Paths from (1, 1) to (n, m) such that the sum of the SET of values in the union of the T paths is maximized.
// We can also handle Disjointedness of Paths easily.
// This assumes A_{ij} is positive.

#include "bits/stdc++.h"
using namespace std;

// Min cost Max flow Template using SPFA

struct Edge {
    int u, v;
    long long cap, cost;

    Edge(int _u, int _v, long long _cap, long long _cost) {
        u = _u; v = _v; cap = _cap; cost = _cost;
    }
};

struct MinimumCostMaximumFlow{
    int n, s, t;
    long long flow, cost;
    vector<vector<int> > graph;
    vector<Edge> e;
    vector<long long> dist;
    vector<int> parent;

    MinimumCostMaximumFlow(int _n){
        // 0-based indexing
        n = _n;
        graph.assign(n, vector<int> ());
    }

    void add(int u, int v, long long cap, long long cost, bool directed = true){
        graph[u].push_back(e.size());
        e.push_back(Edge(u, v, cap, cost));

        graph[v].push_back(e.size());
        e.push_back(Edge(v, u, 0, -cost));

        if(!directed)
            add(v, u, cap, cost, true);
    }

    pair<long long, long long> getMinCostFlow(int _s, int _t){
        s = _s; t = _t;
        flow = 0, cost = 0;

        while(SPFA()){
            flow += sendFlow(t, 1LL<<62);
        }

        return make_pair(flow, cost);
    }

    // Not sure about negative cycle
    bool SPFA(){
        parent.assign(n, -1);
        dist.assign(n, 1LL<<62);        dist[s] = 0;
        vector<int> queuetime(n, 0);    queuetime[s] = 1;
        vector<bool> inqueue(n, 0);     inqueue[s] = true;
        queue<int> q;                   q.push(s);
        bool negativecycle = false;


        while(!q.empty() && !negativecycle){
            int u = q.front(); q.pop(); inqueue[u] = false;

            for(int i = 0; i < graph[u].size(); i++){
                int eIdx = graph[u][i];
                int v = e[eIdx].v, w = e[eIdx].cost, cap = e[eIdx].cap;

                if(dist[u] + w < dist[v] && cap > 0){
                    dist[v] = dist[u] + w;
                    parent[v] = eIdx;

                    if(!inqueue[v]){
                        q.push(v);
                        queuetime[v]++;
                        inqueue[v] = true;

                        if(queuetime[v] == n+2){
                            negativecycle = true;
                            break;
                        }
                    }
                }
            }
        }

        return dist[t] != (1LL<<62);
    }

    long long sendFlow(int v, long long curFlow){
        if(parent[v] == -1)
            return curFlow;
        int eIdx = parent[v];
        int u = e[eIdx].u, w = e[eIdx].cost;

        long long f = sendFlow(u, min(curFlow, e[eIdx].cap));

        cost += f*w;
        e[eIdx].cap -= f;
        e[eIdx^1].cap += f;

        return f;
    }
};

/***** End of Minimum Cost Maximum Flow Template *****/

const int N = 105;

int n, m, t, id;
int arr[N][N];
int in[N][N], out[N][N];

int main() {
    cin >> n >> m >> t;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cin >> arr[i][j];
        }
    }
    // Enumerate the nodes.
    int source = ++id;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            in[i][j] = ++id;
            out[i][j] = ++id;
        }
    }
    int sink = ++id;
    // Add appropriate edges
    MinimumCostMaximumFlow mcmf(id + 10);
    mcmf.add(source, in[1][1], t, 0);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            mcmf.add(in[i][j], out[i][j], 1, -arr[i][j]); // The first time we take this edge we add its value
            mcmf.add(in[i][j], out[i][j], t - 1, 0); // Subsequent times we don't add its value
            // If we want to make paths disjoint we can just not have the above edge at all.
            if (i + 1 <= n) {
                mcmf.add(out[i][j], in[i + 1][j], t, 0);
            }
            if (j + 1 <= m) {
                mcmf.add(out[i][j], in[i][j + 1], t, 0);
            }
        }
    }
    mcmf.add(out[n][m], sink, t, 0);
    cout << -mcmf.getMinCostFlow(source, sink).second << endl;
}