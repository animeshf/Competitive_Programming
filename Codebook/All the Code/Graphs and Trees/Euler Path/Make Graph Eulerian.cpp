// CF - Story of Princess (ICPC 15/16 Samara)

#include "bits/stdc++.h"
using namespace std;

struct Edge;
typedef list<Edge>::iterator iter;

struct Edge
{
    int next_vertex;
    iter reverse_edge;

    Edge(int next_vertex)
        :next_vertex(next_vertex)
        { }
};

const int max_vertices = 200000;
int num_vertices;
list<Edge> adj[max_vertices];       // adjacency list

vector<int> path;

void find_path(int v)
{
    while(adj[v].size() > 0)
    {
        int vn = adj[v].front().next_vertex;
        adj[vn].erase(adj[v].front().reverse_edge);
        adj[v].pop_front();
        find_path(vn);
    }
    path.push_back(v);
}

void add_edge(int a, int b)
{
    adj[a].push_front(Edge(b));
    iter ita = adj[a].begin();
    adj[b].push_front(Edge(a));
    iter itb = adj[b].begin();
    ita->reverse_edge = itb;
    itb->reverse_edge = ita;
}

// ************ End of Undirected Euler Path *************

/**
    Essentially, we want an Eulerian Graph since those are the nicest for this problem.
    However, the graph we get might not be Eulerian. It can be shown that we can't get
    an optimal solution if the graph is not Eulerian.

    We can reduce the problem to : "Add fewest edges to make the graph Eulerian". This
    can be solved greedily. For each component, compute # of odd degree vertices, this
    number has to be even. Match them up until you have 2 vertices of odd degree. Now
    your graph is Eulerian, use Heirholzer to compute an Euler Path.
**/

int n, m;
vector < int > odd;
int vis[max_vertices];

inline void dfs(int node) {
    vis[node] = true;
    int degree = 0;
    for (Edge e: adj[node]) {
        int next = e.next_vertex;
        degree += 1;
        if (!vis[next]) {
            dfs(next);
        }
    }
    if (degree & 1) {
        odd.push_back(node);
    }
}

inline void solve(int root) {
    odd.clear();
    dfs(root);
    int path_start = root;
    while ((int) odd.size() > 2) {
        int u = odd.back(); odd.pop_back();
        int v = odd.back(); odd.pop_back();
        add_edge(u, v);
    }
    if ((int) odd.size() == 2) {
        root = odd[0];
    }
    if ((int) adj[root].size()) {
        find_path(root);
    }
}

int main() {
    ios :: sync_with_stdio(false);
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        int u, v; cin >> u >> v;
        add_edge(u, v);
    }
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) {
            solve(i);
        }
    }
    cout << (int) path.size() << endl;
    for (int v: path) {
        cout << v << ' ';
    }
    cout << endl;
}