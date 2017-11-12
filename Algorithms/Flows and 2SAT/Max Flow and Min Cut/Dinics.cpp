#include "bits/stdc++.h"
using namespace std;


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

const int N = 105;
const int M = N * N;

int n, m;
int w[N], l[N], t[N];
int in_crane[N];
int out_crane[N];
int in_building[N];
int out_building[N];
int source, sink, timer;
int par[M];
int rev[M]; // id of crane or building that we're dealing with

inline void solve(int id) {
    int st = out_building[id];
    vector < int > indices;
    while (st != source) {
        indices.push_back(rev[st]);
        st = par[st];
    }
    reverse(indices.begin(), indices.end());
    set < int > printed;
    for (int x: indices) {
        if ((x >= 1 && x <= n) && !printed.count(x)) {
            cout << x << ' ';
            printed.insert(x);
        }
    }
    cout << endl;
}

void bfs(Dinic flow) {
    queue < int > q;
    fill(par, par + M, -1);
    par[source] = source;
    q.push(source);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (Edge e: flow.G[u]) {
            int v = e.to;
            if (e.flow == 1) {
                if (par[v] == -1) {
                    par[v] = u;
                    q.push(v);
                }
            }
        }
    }
    for (int i = 1; i <= m; i++) {
        solve(i);
    }
}

int main() {
    source = 0;
    cin >> n;
    rev[source] = 0;
    for (int i = 1; i <= n; i++) {
        cin >> w[i] >> l[i];
        in_crane[i] = ++timer;
        rev[timer] = i;
        out_crane[i] = ++timer;
        rev[timer] = i;
    }
    cin >> m;
    for (int i = 1; i <= m; i++) {
        cin >> t[i];
        in_building[i] = ++timer;
        rev[timer] = -1;
        out_building[i] = ++timer;
        rev[timer] = -1;
    }
    rev[sink] = 0;
    sink = ++timer;
    Dinic flow(timer + 10);
    for (int i = 1; i <= n; i++) {
        if (w[i] == 0) {
            // source to 0 weight cranes
            flow.AddEdge(source, in_crane[i], 1);
        }
        // each crane has a capacity of 1 -> ensuring vertex disjoint paths
        flow.AddEdge(in_crane[i], out_crane[i], 1);
        for (int j = 1; j <= n; j++) {
            if (j == i) {
                continue;
            }
            if (l[i] >= w[j]) {
                // edge from one crane to another crane.
                flow.AddEdge(out_crane[i], in_crane[j], 1);
            }
        }
        for (int j = 1; j <= m; j++) {
            if (l[i] >= t[j]) {
                // ith crane satisfied jth building
                flow.AddEdge(out_crane[i], in_building[j], 1);
            }
        }
    }
    for (int i = 1; i <= m; i++) {
        // limit outflow from each building to be 1
        flow.AddEdge(in_building[i], out_building[i], 1);
        // add edge to sink
        flow.AddEdge(out_building[i], sink, 1);
    }
    int total = flow.GetMaxFlow(source, sink);
    if (total < m) {
        cout << "impossible" << endl;
    } else {
        assert (total == m);
        bfs(flow);
    }
}