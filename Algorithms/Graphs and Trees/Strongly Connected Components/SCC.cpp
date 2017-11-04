
/*
    ------------ Strongly Connected Components ----------

    This implementation computes the SCCs of a directed graph in linear time.
    Things to set up before using this code:

    - adj[] should be an adjacency list corresponding to the directed graph
      we want to operate on.
    - MAXN should be the maximum number of nodes in the graph. These are 0 indexed.

    kosaraju() computes a vector of vectors, denoting the partition of the graph
    into SCCs. The number of SCCs can be found by looking at scc.size(), and each
    scc is stored in the same vector, indexed from [0, scc.size())

    ------------------------------------------------------
*/

const int MAXN = 100000;

vector < int > adj[MAXN], rev[MAXN];
vector < int > g[MAXN];
vector < bool > visit(MAXN);
vector < vector < int > > scc;

inline void dfs(vector<int> &res, int u) {
    visit[u] = true;
    for (int j = 0; j < (int) g[u].size(); j++) {
        if (!visit[g[u][j]]) {
            dfs(res, g[u][j]);
        }
    }
    res.push_back(u);
}

inline void kosaraju(int nodes) {
    fill(visit.begin(), visit.end(), false);
    vector < int > order;
    for (int i = 0; i < MAXN; i++) {
        g[i] = adj[i];
    }
    for (int i = 0; i < nodes; i++) {
        rev[i].clear();
        if (!visit[i]) {
            dfs(order, i);
        }
    }
    reverse(order.begin(), order.end());
    fill(visit.begin(), visit.end(), false);
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < (int) adj[i].size(); j++) {
            rev[adj[i][j]].push_back(i);
        }
    }
    scc.clear();
    for (int i = 0; i < MAXN; i++) {
        g[i] = rev[i];
    }
    for (int i = 0; i < (int) order.size(); i++) {
        if (visit[order[i]]) {
            continue;
        }
        vector < int > component;
        dfs(component, order[i]);
        scc.push_back(component);
    }
}
// End of Strongly Connected Components