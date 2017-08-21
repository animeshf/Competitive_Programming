#include "bits/stdc++.h"
using namespace std;

const int N = 12345678;

// 0 = null

int n, q, ticks;
int root[N], lc[N], rc[N], sum[N];

inline int insert(int old_root, int l, int r, int pos, int val) {
    int cur_root = ++ticks;
    lc[cur_root] = lc[old_root];
    rc[cur_root] = rc[old_root];
    sum[cur_root] = sum[old_root] + val;
    if (l == r) {
        return cur_root;
    }
    int mid = (l + r) >> 1;
    if (mid >= pos) {
        lc[cur_root] = insert(lc[old_root], l, mid, pos, val);
    } else {
        rc[cur_root] = insert(rc[old_root], mid + 1, r, pos, val);
    }
    return cur_root;
}

inline int query(int lid, int rid, int l, int r, int min_freq) {
    int num_values_in_this_range = sum[rid] - sum[lid];
    if (num_values_in_this_range < min_freq) {
        return -1;
    }
    if (l == r) {
        return l;
    }
    int mid = (l + r) >> 1;
    int lol = query(lc[lid], lc[rid], l, mid, min_freq);
    if (lol != -1) {
        return lol;
    }
    return query(rc[lid], rc[rid], mid + 1, r, min_freq);
}

int main() {
    freopen ("inp.in", "r", stdin);
    ios :: sync_with_stdio(false);
    cin >> n >> q;
    for (int i = 1; i <= n; i++) {
        int x; cin >> x;
        root[i] = insert(root[i - 1], 1, n, x, 1);
    }
    while (q--) {
        int l, r, k; cin >> l >> r >> k;
        int freq = (r - l + 1) / k + 1;
        cout << query(root[l - 1], root[r], 1, n, freq) << endl;
    }
}
