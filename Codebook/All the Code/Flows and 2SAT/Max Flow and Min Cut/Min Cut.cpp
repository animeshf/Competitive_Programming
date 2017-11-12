// SWERC 2015 - Landscaping

/**
    General strategies to solve min-cut problems:

    1) Convert everything to "penalties". Most often the problem
       would reduce to a bipartite graph with a source connecting to
       nodes on the left partite and a sink connected to nodes on the
       right partite.

    2) Don't think in terms of flow, as most often it won't make sense
       straightaway. Think in terms of "cuts" where removing each edge
       can be thought of as a penalty. We want to minimize our penalty
       i.e. find a minimum cut.

    3) If certain constraint must be satisfied, say node (i) on the left
       and node (j) on the right must always be in the "same set", then we
       can model this by adding an infinite edge between these two nodes,
       since infinite edges will never be part of a min-cut. Other constraints
       can be handled using similar tricks.

    Retrieving min-cut from flow network:

    Do a BFS in the final residual graph from the source vertex to compute
    S = set of vertices reachable from source. The edges in the min cut are
    precisely those with one end point in S and the other in V \ S.
**/

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

const int N = 100;

int n, m, a, b;
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};
int timer;
int id[N][N];
bool high[N][N];

int main() {
    scanf("%d %d %d %d", &n, &m, &a, &b);
    Dinic mf(n * m + 2);
    int source = 0;
    int sink = n * m + 1;
    for (int i = 1; i <= n; i++) {
        char inp[m + 1];
        scanf("%s", inp + 1);
        for (int j = 1; j <= m; j++) {
            id[i][j] = ++timer;
            high[i][j] = (inp[j] == '#');
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (high[i][j]) {
                mf.AddEdge(source, id[i][j], b);
            } else {
                mf.AddEdge(id[i][j], sink, b);
            }
            for (int k = 0; k < 4; k++) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    if (ni >= 1 && ni <= n && nj >= 1 && nj <= m) {
                            mf.AddEdge(id[i][j], id[ni][nj], a);
                    }
                }
        }
    }
    cout << mf.GetMaxFlow(source, sink) << endl;
}