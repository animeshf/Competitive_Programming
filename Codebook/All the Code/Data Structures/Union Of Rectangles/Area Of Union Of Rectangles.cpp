// Problem: Strange Tree (OpenBracket 2017).

#include "bits/stdc++.h"
using namespace std;

// Area of Union of Rectangles Template

// Given a list of rectangles on the 2D Plane, returns the union of their areas.
// A rectangle is defined as a pair of pairs ((x1, y1), (x2, y2)) where (x1, y1)
// is the bottom left corner of the rectangle and (x2, y2) is the top right corner.
// O(n log n)

const int MAXQ = 400005; // 2 * number of rectangles.
const int MINY = 0;
const int MAXY = 400005; // The max y coordinate.
#define ii pair < int, int >

struct event{
    int x, l, r, t;
    event(int _x = 0, int _l = 0, int _r = 0, int  _t = 0){
        x = _x, l = _l, r = _r, t = _t;
    }
    friend bool operator < (event a, event b){
        if(a.x == b.x){
            int y1 = (a.t == 1) ? (a.l) : (a.r);
            int y2 = (b.t == 1) ? (b.l) : (b.r);
            return (y1 < y2);
        }
        return (a.x < b.x);
    }
}query[MAXQ];

int lazy[MAXY * 4];
ii tree[MAXY * 4];

inline ii combine(ii a, ii b){
    if(a.first == b.first) return ii(a.first, a.second + b.second);
    return min(a, b);
}

inline void push(int l, int r, int node){
    tree[node].first += lazy[node];
    if(l != r){
        lazy[node * 2] += lazy[node];
        lazy[node * 2 + 1] += lazy[node];
    }
    lazy[node] = 0;
}

inline void update(int node, int l, int r, int qs, int qe, int val){
    push(l, r, node);
    if(l > qe || r < qs) return;
    if(l >= qs && r <= qe){
        lazy[node] = val;
        push(l, r, node);
        return;
    }
    int mid = (l + r) >> 1;
    update(node * 2, l, mid, qs, qe, val);
    update(node * 2 + 1, mid + 1, r, qs, qe, val);
    tree[node] = combine(tree[node * 2], tree[node * 2 + 1]);
}

inline void build(int node, int l, int r){
    tree[node] = ii(0, r - l + 1);
    if(l != r){
        int mid = (l + r) >> 1;
        build(node * 2, l, mid);
        build(node * 2 + 1, mid + 1, r);
    }
}

inline long long solve(vector < pair < ii, ii > > rectangles) {
    int counter = 0;
    for(pair < ii, ii > r: rectangles){
        ++counter;
        int x1 = r.first.first;
        int y1 = r.first.second;
        int x2 = r.second.first;
        int y2 = r.second.second;
        query[2 * counter - 1] = event(x1, y1, y2, 1);
        query[2 * counter] = event(x2, y1, y2, -1);
    }
    sort(query + 1, query + 2 * counter + 1);
    build(1, MINY, MAXY);
    long long area = 0;
    for(int i = 1; i <= 2 * counter; i++){
        update(1, MINY, MAXY, query[i].l, query[i].r - 1, query[i].t);
        long long min_val = tree[1].first;
        long long min_cnt = (!min_val) ? (tree[1].second) : (0);
        area += (query[i + 1].x - query[i].x) * 1LL * (MAXY - MINY + 1 - min_cnt);
    }
    return area;
}

// End of template

const int N = 200005;
const int LN = 20;

int n, timer;
int arr[N], tin[N], tout[N], depth[N];
int anc[LN][N];
vector < int > adj[N];
vector < int > values[N];

inline void dfs(int u, int p) {
    values[arr[u]].push_back(u);
    anc[0][u] = p;
    for (int i = 1; i < LN; i++)
        anc[i][u] = anc[i - 1][anc[i - 1][u]];
    tin[u] = ++timer;
    for (int v: adj[u]) {
        if (v != p) {
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }
    tout[u] = timer;
}

inline bool is_ancestor(int x, int y) {
    return tin[x] <= tin[y] and tout[x] >= tout[y];
}

inline int get_ancestor(int x, int k) {
    for (int i = LN - 1; i >= 0; i--) {
        if (k >= (1 << i)) {
            x = anc[i][x];
            k -= (1 << i);
        }
    }
    return x;
}

int main() {
    ios :: sync_with_stdio(false);
    cin >> n;
    for (int i = 2; i <= n; i++) {
        int p; cin >> p;
        adj[p].push_back(i);
        adj[i].push_back(p);
    }
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    dfs(1, 1);
    vector < pair < ii, ii > > rectangles;
    int x1, y1, x2, y2;
    for (int i = 1; i <= n / 2; i++) {
        int x = values[i][0];
        int y = values[i][1];
        if (tin[x] > tin[y]) swap(x, y);
        if (is_ancestor(x, y)) {
            int p = get_ancestor(y, depth[y] - depth[x] - 1);
            x1 = 1, x2 = tin[p];
            y1 = tin[y], y2 = tout[y] + 1;
            rectangles.push_back(make_pair(ii(x1, y1), ii(x2, y2)));
            x1 = tin[y], x2 = tout[y] + 1;
            y1 = tout[p] + 1, y2 = n + 1;
            rectangles.push_back(make_pair(ii(x1, y1), ii(x2, y2)));
        } else {
            x1 = tin[x], x2 = tout[x] + 1;
            y1 = tin[y], y2 = tout[y] + 1;
            rectangles.push_back(make_pair(ii(x1, y1), ii(x2, y2)));
        }
    }
    long long bad_pairs = solve(rectangles);
    cout << (n * 1LL * n - bad_pairs * 2LL) << endl;
}