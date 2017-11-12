// Codeforces - Tourists (Some Div 1E)
#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9 + 333;
const int MAX = (1 << 18);
const int LN  = 18;

int n, m, q, timekeeper, cnt_bcc, sz, w[MAX];
int disc[MAX], low[MAX], cut[MAX], bcc[MAX], baap[MAX], is_cut[MAX];
int tree[MAX << 1], timer, chainPos[MAX], head[MAX], root;
int sub[MAX], depth[MAX], dp[LN][MAX], vis[MAX];
vector < pair < int, int > > adj[MAX];
vector < int > bc_tree[MAX], in_bcc[MAX];
pair < int, int > temp[MAX];
multiset < int > costs[MAX];

inline void make_tree(int p_id = -1, int p = 0, int x = 1) {
    vis[x] = 1;
    temp[sz++] = make_pair(x, p);
    low[x] = disc[x] = ++timekeeper;
    for(auto it : adj[x]) {
        int u = it.first;
        int e = it.second;
        if(e != p_id) {
            if(!vis[u]) {
                make_tree(e, x, u);
                low[x] = min(low[x], low[u]);
                if(low[u] >= disc[x]) {
                    cut[x] = 1;
                    cnt_bcc++;
                    while(temp[sz] != make_pair(u, x)) {
                        bcc[temp[sz - 1].first] = cnt_bcc;
                        bcc[temp[sz - 1].second] = cnt_bcc;
                        sz--;
                    }
                    in_bcc[x].push_back(cnt_bcc);
                    baap[cnt_bcc] = x;
                }
            }
            else
                low[x] = min(low[x], disc[u]);
        }
    }
}

inline void pre(int p = 0, int x = root) {
    sub[x] = 1;
    dp[0][x] = p;
    depth[x] = depth[p] + 1;
    for(int i = 1; i < LN; i++) dp[i][x] = dp[i - 1][dp[i - 1][x]];
    for(auto it : bc_tree[x]) {
        int u = it;
        if(u != p) {
            pre(x, u);
            sub[x] += sub[u];
        }
    }
}

inline void hld(int p = -1, int x = root, int h = root) {
    head[x] = h;
    chainPos[x] = ++timer;
    int rdcount = -1, rajat = -1; // pro-child
    for(auto it : bc_tree[x]) {
        int u = it;
        if(u != p and sub[u] > rdcount) {
            rdcount = u;
            rajat = u;
        }
    }
    if(rajat != -1) hld(x, rajat, h);
    for(auto it : bc_tree[x]) {
        int u = it;
        if(u != p and u != rajat)
            hld(x, u, u);
    }
}

inline void update(int x, int k) {
    tree[x += MAX] = k;
    while(x > 1) {
        x >>= 1;
        tree[x] = min(tree[x + x], tree[x + x + 1]);
    }
}

inline int query(int l, int r) {
    int res = INF;
    for(l += MAX, r += MAX; l <= r; l >>= 1, r >>= 1) {
        if(l & 1) res = min(res, tree[l++]);
        if(~r & 1) res = min(res, tree[r--]);
    }
    return res;
}

inline int get_lca(int x, int y) {
    if(depth[x] < depth[y]) swap(x, y);
    for(int i = LN - 1; i >= 0; i--)
        if(depth[x] - (1 << i) >= depth[y])
            x = dp[i][x];
    if(x == y) return x;
    for(int i = LN - 1; i >= 0; i--) {
        if(dp[i][x] != dp[i][y]) {
            x = dp[i][x];
            y = dp[i][y];
        }
    }
    return dp[0][x];
}

inline int qmin(int x, int up) {
    int res = INF;
    while(depth[x] >= depth[up]) {
        res = min(res, query(max(chainPos[up], chainPos[head[x]]), chainPos[x]));
        x = dp[0][head[x]];
    }
    return res;
}

int main () {
    
    scanf("%d %d %d", &n, &m, &q);
    
    for(int i = 1; i <= n; i++) {
        scanf("%d", w + i);
    }
    
    for(int i = 1; i <= m; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        adj[x].push_back(make_pair(y, i));
        adj[y].push_back(make_pair(x, i));
    }
    
    make_tree();
    
    for(int i = n; i >= 1; i--) {
        if(cut[i]) {
            cut[i] = ++cnt_bcc;
            is_cut[cnt_bcc] = i;
        }
    }
    
    root = cnt_bcc;
    
    for(int i = 1; i <= n; i++) {
        if(cut[i]) {
            bc_tree[bcc[i]].push_back(cut[i]);
            bc_tree[cut[i]].push_back(bcc[i]);
            for(auto it : in_bcc[i]) {
                int x = it;
                bc_tree[cut[i]].push_back(x);
                bc_tree[x].push_back(cut[i]);
            }
        }
    }
    
    for(int i = 1; i <= n; i++) {
        sort(bc_tree[i].begin(), bc_tree[i].end());
        bc_tree[i].resize(unique(bc_tree[i].begin(), bc_tree[i].end()) - bc_tree[i].begin());
    }
    
    pre();
    hld();
    
    for(int i = 1; i <= n; i++) {
        costs[bcc[i]].insert(w[i]);
    }
    
    for(int i = 1; i <= cnt_bcc; i++) {
        update(chainPos[i], costs[i].size() ? *costs[i].begin() : INF);
    }
    
    while(q--){
        char c;
        int x, y;
        scanf(" %c %d %d", &c, &x, &y);
        if(c == 'A') {
            if(x == y) {
                printf("%d\n", w[x]);
                continue;
            }
            x = cut[x] ? cut[x] : bcc[x];
            y = cut[y] ? cut[y] : bcc[y];
            int lca = get_lca(x, y);
            int res = min(qmin(x, lca), qmin(y, lca));
            if(is_cut[lca]) res = min(res, w[is_cut[lca]]);
            else if(baap[lca]) res = min(res, w[baap[lca]]);
            printf("%d\n", res);
        }
        else {
            costs[bcc[x]].erase(costs[bcc[x]].find(w[x]));
            costs[bcc[x]].insert(w[x] = y);
            update(chainPos[bcc[x]], *costs[bcc[x]].begin());
        }
    }    
}