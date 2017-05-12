#include "bits/stdc++.h"
using namespace std;

const int N = 500;

int w, h, b;
int in_id[N][N], out_id[N][N];
bool rekt[N][N];

int dx[] = {0, -1, 0, 1};
int dy[] = {1, 0, -1, 0};

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

int main() {
    freopen ("inp.in", "r", stdin);
    freopen ("C.out", "w", stdout);
    int tc; cin >> tc;
    for (int qq = 1; qq <= tc; qq++) {
        cout << "Case #" << qq << ": ";
        cin >> w >> h >> b;
        int ticks = 0;
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                in_id[i][j] = ++ticks;
                out_id[i][j] = ++ticks;
                rekt[i][j] = false;
            }
        }
        for (int z = 1; z <= b; z++) {
            int x, y, xx, yy;
            cin >> x >> y >> xx >> yy;
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {
                    if (i >= x && i <= xx && j >= y && j <= yy) {
                        rekt[i][j] = true;
                    }
                }
            }
        }
        Dinic graph(ticks + 10);
        int source = ++ticks;
        int sink = ++ticks;
        for (int i = 0; i < w; i++) {
            if (!rekt[i][0]) {
                graph.AddEdge(source, in_id[i][0], 1);
            }
            if (!rekt[i][h - 1]) {
                graph.AddEdge(out_id[i][h - 1], sink, 1);
            }
        }
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                if (rekt[i][j]) {
                    continue;
                }
                graph.AddEdge(in_id[i][j], out_id[i][j], 1);
                for (int k = 0; k < 4; k++) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    if (ni >= 0 && ni < w && nj >= 0 && nj < h) {
                        if (!rekt[ni][nj]) {
                            graph.AddEdge(out_id[i][j], in_id[ni][nj], 1);
                        }
                    }
                }
            }
        }
        cout << graph.GetMaxFlow(source, sink) << "\n";
    }
}