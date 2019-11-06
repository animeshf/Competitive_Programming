#include "bits/stdc++.h"
using namespace std;

const int N = 123456;
const int Q = N * 2;
const int MINY = 0;
const int MAXY = 2000000000;
const int NODES = 10000000;
const int OFF = 1000000000;

struct node {
    int min_val, min_cnt; // for area union.
    int ones; // for odd areas.
};

int n;
node tree[NODES];
int tick;
int lc[NODES], rc[NODES];
int lazy[2][NODES];
tuple<int, int, int, int> events[Q];

inline void create_if_null(int &v, int l, int r) {
    if (!v) {
        v = ++tick;
        tree[v].min_val = 0;
        tree[v].min_cnt = r - l + 1;
        tree[v].ones = 0;
        lazy[0][v] = lazy[1][v] = 0;
    }
}

inline void push(int& v, int l, int r) {
    if (lazy[0][v] != 0) {
        tree[v].min_val += lazy[0][v];
        if (l != r) {
            lazy[0][lc[v]] += lazy[0][v];
            lazy[0][rc[v]] += lazy[0][v];
        }
        lazy[0][v] = 0;
    }
    if (lazy[1][v] == 1) {
        tree[v].ones = (r - l + 1) - (tree[v].ones);
        if (l != r) {
            lazy[1][lc[v]] ^= 1;
            lazy[1][rc[v]] ^= 1;
        }
        lazy[1][v] = 0;
    }
}

inline void merge(int &v, int l, int r) {
    node result;
    if (tree[lc[v]].min_val < tree[rc[v]].min_val) {
        result.min_val = tree[lc[v]].min_val;
        result.min_cnt = tree[lc[v]].min_cnt;
    } else if (tree[rc[v]].min_val < tree[lc[v]].min_val) {
        result.min_val = tree[rc[v]].min_val;
        result.min_cnt = tree[rc[v]].min_cnt;
    } else {
        result.min_val = tree[lc[v]].min_val;
        result.min_cnt = tree[lc[v]].min_cnt + tree[rc[v]].min_cnt;
    }
    result.ones = tree[lc[v]].ones + tree[rc[v]].ones;
    tree[v] = result;
}

void update(int &root, long long l, long long r, long long qs, long long qe, int delta) {
    long long mid = (l + r) >> 1;
    create_if_null(root, l, r);
    create_if_null(lc[root], l, mid);
    create_if_null(rc[root], mid + 1, r);
    push(root, l, r);
    if (l > qe || r < qs) {
        return;
    }
    if (l >= qs && r <= qe) {
        lazy[0][root] = delta;
        lazy[1][root] ^= 1;
        push(root, l, r);
        return;
    }
    update(lc[root], l, mid, qs, qe, delta);
    update(rc[root], mid + 1, r, qs, qe, delta);
    merge(root, l, r);
}

int main() {
    ios :: sync_with_stdio(false);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int x, y, u, v; cin >> x >> y >> u >> v;
        if (y > v) swap(y, v);
        if (x > u) swap(x, u);
        y += OFF, v += OFF;
        events[2 * i - 1] = make_tuple(x, +1, y, v);
        events[2 * i] = make_tuple(u, -1, y, v);
    }
    sort(events + 1, events + 2 * n + 1);
    long long union_area = 0, odd_area = 0;
    int root = 0;
    for (int i = 1; i < 2 * n; i++) {
        auto [x, delta, y_low, y_high] = events[i];
        update(root, MINY, MAXY, y_low, y_high - 1, delta);
        long long num_untouched = (tree[root].min_val == 0) ? (tree[root].min_cnt): (0);
        long long dx = get<0>(events[i + 1]) - x;
        union_area += (dx) * (MAXY - MINY + 1 - num_untouched);
        odd_area += (dx) * (tree[root].ones);
    }
    cout << union_area - odd_area << endl;
}
