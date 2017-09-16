// 2014 ICPC World Finals: Problem I

#include "bits/stdc++.h"
using namespace std;

/*
    Hopcroft Karp Max Matching in O(E * sqrt(V))
    MAXN = Number of Nodes, MAXM = Number of Edges
    n1 = Size of left partite, n2 = Size of right partite
    Nodes are numbered from [0, n1 - 1] and [0, n2 - 1]

    init(n1, n2) declares the two partite sizes and resets arrays
    addEdge(x, y) adds an edge between x in left partite and y in right partite
    maxMatching() returns the maximum matching

    For the two functions below: L denotes the list of nodes in the left partite,
    R denotes the list of nodes in the right partite, and edges denotes the list
    of edges between a node in L and a node in R. Make sure that the graph is bipartite.

    constructMVC(L, R, edges) returns the set of nodes in the minimum vertex cover
    constructMIS(L, R, edges) returns the set of nodes in the maximal independent set

    Important Theorems:

    |Maximum Matching| = |Minimum Vertex Cover| (Konig's Theorem)
    |L| + |R| - Maximum Matching = Maximal Independent Set
*/

const int MAXN = 105;
const int MAXM = 10005;

set < int > adjList[MAXN];
int seen[MAXN];
int matched[MAXN];
int n1, n2, edges, last[MAXN], previous[MAXM], head[MAXM];
int matching[MAXN], dist[MAXN], Q[MAXN];
bool used[MAXN], vis[MAXN];

inline void init(int _n1, int _n2) {
    n1 = _n1;
    n2 = _n2;
    edges = 0;
    fill(last, last + n1, -1);
    fill(matching, matching + n2, -1);
}

inline void addEdge(int u, int v) {
    head[edges] = v;
    previous[edges] = last[u];
    last[u] = edges++;
}

inline void bfs() {
    fill(dist, dist + n1, -1);
    int sizeQ = 0;
    for(int u = 0; u < n1; ++u){
        if(!used[u]){
            Q[sizeQ++] = u;
            dist[u] = 0;
        }
    }
    for(int i = 0; i < sizeQ; i++){
        int u1 = Q[i];
        for(int e = last[u1]; e >= 0; e = previous[e]){
            int u2 = matching[head[e]];
            if(u2 >= 0 && dist[u2] < 0){
                dist[u2] = dist[u1] + 1;
                Q[sizeQ++] = u2;
            }
        }
    }
}

inline bool dfs(int u1) {
    vis[u1] = true;
    for(int e = last[u1]; e >= 0; e = previous[e]){
        int v = head[e];
        int u2 = matching[v];
        if((u2 < 0) || (!vis[u2] && dist[u2] == dist[u1] + 1 && dfs(u2))){
            matching[v] = u1;
            used[u1] = true;
            return true;
        }
    }
    return false;
}

inline int maxMatching() {
    fill(used, used + n1, false);
    fill(matching, matching + n2, -1);
    for(int res = 0; ;){
        bfs();
        fill(vis, vis + n1, false);
        int f = 0;
        for(int u = 0; u < n1; ++u)
            if(!used[u] && dfs(u))
                ++f;
        if(!f) return res;
        res += f;
    }
}

inline void alternating_paths(int u, int c = 0) {
    seen[u] = true;
    for (int v: adjList[u]) {
        if (seen[v]) continue;
        if (c == 0) {
            if (matching[v] == -1) continue;
            alternating_paths(v, c ^ 1);
        } else if (v == matching[u]) {
            alternating_paths(v, c ^ 1);
        }
    }
}

inline void initGraph(vector < pair < int, int > > edges) {
    for (int i = 0; i < MAXN; i++) {
        adjList[i].clear();
    }
    for (int i = 0; i < (int) edges.size(); i++) {
        adjList[edges[i].first].insert(edges[i].second);
        adjList[edges[i].second].insert(edges[i].first);
    }
}

// Constructs the Minimum Vertex Cover using entries in matching[]
// Assumes maxMatching() has been called just before this function is called.
// Considers ONLY nodes in L, R (left partite and right partite)
// edges: List of edges between nodes in L and nodes in R in this Bipartite Graph.
set < int > constructMVC(vector < int > L, vector < int > R, vector < pair < int, int > > edges) {
    initGraph(edges);
    fill(matched, matched + MAXN, false);
    fill(seen, seen + MAXN, false);
    for (int r: R) {
        if (matching[r] != -1) {
            matched[matching[r]] = true;
        }
    }
    for (int l: L) {
        if (matched[l] == true) continue;
        if (!seen[l]) {
            alternating_paths(l);
        }
    }
    set < int > mvc;
    for (int r: R) {
        if (seen[r]) {
            mvc.insert(r);
        }
    }
    for (int l: L) {
        if (!seen[l]) {
            mvc.insert(l);
        }
    }
    return mvc;
}

// Constructs the Maximal Independent Set using entries in matching[]
// Assumes maxMatching() has been called just before this function is called!
// Considers ONLY nodes in L, R (left partite and right partite)
// edges: List of edges between nodes in L and nodes in R in this Bipartite Graph.
set < int > constructMIS(vector < int > L, vector < int > R, vector < pair < int, int > > edges) {
    set < int > U;
    for (int vertex: L) U.insert(vertex);
    for (int vertex: R) U.insert(vertex);
    set < int > MVC = constructMVC(L, R, edges);
    for (int vertex: MVC) U.erase(vertex);
    return U;
}

/******** End of Bipartite Matching Template ********/

const int N = 105;
const int M = N * N;

int n, d;
set < int > adj[N];
int x[N], y[N], visited[N];
vector < int > black, white;

inline int euclidean_distance(int i, int j) {
    return (x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]);
}

inline void coloring(int u, int color) {
    visited[u] = true;
    if (!color) {
        black.push_back(u);
    } else {
        white.push_back(u);
    }
    for (int v: adj[u]) {
        if (!visited[v]) {
            coloring(v, color ^ 1);
        }
    }
}

int solve(int s, int t, int threshold) {
    set < int > candidates;
    for (int i = 0; i < n; i++) {
        adj[i].clear();
        visited[i] = false;
        if (s == i || t == i) {
            continue;
        }
        if (euclidean_distance(s, i) <= threshold && euclidean_distance(t, i) <= threshold) {
            candidates.insert(i);
        }
    }
    for (int i: candidates) {
        for (int j: candidates) {
            if (i != j) {
                if (euclidean_distance(i, j) > threshold) {
                    adj[i].insert(j);
                    adj[j].insert(i);
                }
            }
        }
    }
    black.clear();
    white.clear();
    for (int i = 0; i < n; i++) {
        if (!candidates.count(i)) {
            continue;
        }
        if (!visited[i]) {
            coloring(i, 0);
        }
    }
    init(n, n);
    for (int w: white) {
        for (int b: black) {
            if (adj[w].count(b))
                addEdge(w, b);
        }
    }
    int clique_size = 2 + (int) white.size() + (int) black.size() - maxMatching();
    return clique_size;
}

int main() {
    ios :: sync_with_stdio(false);
    cin >> n >> d;
    for (int i = 0; i < n; i++) {
        cin >> x[i] >> y[i];
    }
    int result = 1;
    set < int > sol;
    sol.insert(0);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int edge_dist = euclidean_distance(i, j);
            if (edge_dist <= d * d) {
                int best = solve(i, j, euclidean_distance(i, j));
                if (best > result) {
                    result = best;
                    sol.clear(); sol.insert(i); sol.insert(j);
                    vector < pair < int, int > > edges;
                    for (int u = 0; u < n; u++) {
                        for (int v: adj[u]) {
                            edges.push_back(make_pair(min(u, v), max(u, v)));
                        }
                    }
                    set < int > mis = constructMIS(white, black, edges);
                    for (int vertex: mis) {
                        sol.insert(vertex);
                    }
                }
            }
        }
    }
    cout << (int) sol.size() << endl;
    for (int v: sol) {
        cout << (v + 1) << ' ';
    }
    cout << endl;
}