// Primitive Queries - DISTNUM3
#include "bits/stdc++.h"
using namespace std;

const int N = 100000 + 50;
const int LN = 17;

int n, q, blk, ticks, qid, uid, res;
int tin[N], tout[N], depth[N], block_id[N * 2];
int color[N], original_color[N], order[2 * N];
int ans[N], vis[N], freq[2 * N], last[N];
int anc[LN][N];
vector < int > all_colors;
vector < int > adj[N];

struct query {
    int l, r, lc, t, id;
    query(int _l = 0, int _r = 0, int _lc = 0, int _t = 0, int _id = 0) {
        l = _l;
        r = _r;
        lc = _lc;
        t = _t;
        id = _id;
    }
    inline friend bool operator < (query a, query b) {
        if (block_id[a.l] != block_id[b.l]) {
            return (block_id[a.l] < block_id[b.l]);
        } else if (block_id[a.r] != block_id[b.r]) {
            return (block_id[a.r] < block_id[b.r]);
        } else if (a.t != b.t) {
            return (a.t < b.t);
        } else {
            return (a.id < b.id);
        }
    }
}queries[N];

struct update {
    int node, old_color, new_color;
    update(int _node = 0, int _old_color = 0, int _new_color = 0) {
        node = _node;
        old_color = _old_color;
        new_color = _new_color;
    }
}updates[N];

inline int compress(int x) {
    return lower_bound(all_colors.begin(), all_colors.end(), x) - all_colors.begin();
}

inline void dfs(int u, int p) {
    tin[u] = ++ticks;
    order[ticks] = u;
    anc[0][u] = p;
    for (int i = 1; i < LN; i++) {
        anc[i][u] = anc[i - 1][anc[i - 1][u]];
    }
    for (int v : adj[u]) {
        if (v == p) {
            continue;
        }
        depth[v] = depth[u] + 1;
        dfs(v, u);
    }
    tout[u] = ++ticks;
    order[ticks] = u;
}

inline int lca(int x, int y) {
    if (depth[x] < depth[y]) {
        swap(x, y);
    }
    for (int i = LN - 1; i >= 0; i--) {
        if (depth[x] - (1 << i) >= depth[y]) {
            x = anc[i][x];
        }
    }
    if (x == y) {
        return x;
    }
    for (int i = LN - 1; i >= 0; i--) {
        if (anc[i][x] != anc[i][y]) {
            x = anc[i][x];
            y = anc[i][y];
        }
    }
    return anc[0][x];
}

inline void visit(int timer) {
    int node = order[timer];
    if (vis[node]) {
        if (freq[color[node]] == 1) {
            res--;
        }
        freq[color[node]]--;
    } else {
        if (freq[color[node]] == 0) {
            res++;
        }
        freq[color[node]]++;
    }
    vis[node] ^= 1;
}

inline void change_color(int u, int x) {
    if (vis[u]) {
        visit(tin[u]);
        color[u] = x;
        visit(tin[u]);
    } else {
        color[u] = x;
    }
}

int main() {
    scanf("%d %d", &n, &q);
    blk = (cbrt(2 * n + 10)) * (cbrt(2 * n + 10));
    for (int i = 1; i <= n; i++) {
        scanf("%d", &original_color[i]);
        all_colors.push_back(original_color[i]);
        last[i] = original_color[i];
    }
    for (int i = 1; i <= 2 * n; i++) {
        block_id[i] = i / blk + 1;
    }
    for (int i = 1; i < n; i++) {
        int foo, bar; scanf("%d %d", &foo, &bar);
        adj[foo].push_back(bar);
        adj[bar].push_back(foo);
    }
    dfs(1, 1);
    for (int i = 1; i <= q; i++) {
        int type; scanf("%d", &type);
        if (type == 1) {
            int u, v; scanf("%d %d", &u, &v);
            if (tin[u] > tin[v]) {
                swap(u, v);
            }
            int lc = lca(u, v);
            if (lc == u) {
                queries[1 + qid] = query(tin[u], tin[v], 0, uid, 1 + qid);
            } else {
                queries[1 + qid] = query(tout[u], tin[v], lc, uid, 1 + qid);
            }
            ++qid;
        } else {
            int v, y; scanf("%d %d", &v, &y);
            updates[++uid] = update(v, last[v], y);
            all_colors.push_back(y);
            last[v] = y;
        }
    }
    // Coordinate Compression
    sort(all_colors.begin(), all_colors.end());
    all_colors.resize(unique(all_colors.begin(), all_colors.end()) - all_colors.begin());
    for (int i = 1; i <= n; i++) {
        color[i] = compress(original_color[i]);
    }
    for (int i = 1; i <= uid; i++) {
        updates[i].old_color = compress(updates[i].old_color);
        updates[i].new_color = compress(updates[i].new_color);
    }
    // End of Coordinate Compression
    sort(queries + 1, queries + 1 + qid);
    int ql = 1, qr = 0, upd = 0;
    for (int i = 1; i <= qid; i++) {
        while (upd < queries[i].t) {
            ++upd;
            change_color(updates[upd].node, updates[upd].new_color);
        }
        while (upd > queries[i].t) {
            change_color(updates[upd].node, updates[upd].old_color);
            --upd;
        }
        while (ql < queries[i].l) {
            visit(ql++);
        }
        while (ql > queries[i].l) {
            visit(--ql);
        }
        while (qr < queries[i].r) {
            visit(++qr);
        }
        while (qr > queries[i].r) {
            visit(qr--);
        }
        if (queries[i].lc) {
            visit(tin[queries[i].lc]);
        }
        ans[queries[i].id] = res;
        if (queries[i].lc) {
            visit(tin[queries[i].lc]);
        }
    }
    for (int i = 1; i <= qid; i++) {
        printf("%d\n", ans[i]);
    }
}