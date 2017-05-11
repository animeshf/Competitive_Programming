#include "bits/stdc++.h"
using namespace std;

const int N = 200005;
const int INF = 1000000000;

int n, m, k;
int c[N];
int f[N], fi[N], se[N];

struct node {
    int count;
    int val;
    long long sum;
    node (int cx = 0, long long ss = 0, int v = 0) {
        count = cx;
        sum = ss;
        val = v;
    }
}tree[N * 4];


inline node merge(node a, node b) {
    return node(a.count + b.count, a.sum + b.sum, 69);
}

inline void update(int id, int l, int r, int pos, int val) {
    if (l == r) {
        tree[id].sum += val;
        tree[id].val = val;
        tree[id].count++;
        return;
    }
    int mid = (l + r) >> 1;
    if (mid >= pos) {
        update(id * 2, l, mid, pos, val);
    } else {
        update(id * 2 + 1, mid + 1, r, pos, val);
    }
    tree[id] = merge(tree[id * 2], tree[id * 2 + 1]);
}

inline long long query(int id, int l, int r, int z) {
    if (tree[1].count < z) {
        return 1LL << 60LL;
    }
    if (l == r) {
        assert (tree[id].count >= z);
        return z * tree[id].val;
    }
    int mid = (l + r) >> 1;
    int lc = id * 2; int rc = lc + 1;
    if (tree[lc].count >= z) {
        return query(lc, l, mid, z);
    } else {
        return tree[lc].sum + query(rc, mid + 1, r, z - tree[lc].count);
    }
}

vector < long long > common;
vector < long long > a, b;
vector < long long > sa, sb;
map < long long, int > cc;

int main() {
    ios :: sync_with_stdio(false);
    cin >> n >> m >> k;
    for (int i = 1; i <= n; i++) {
        cin >> c[i];
    }
    int z; cin >> z;
    for (int i = 1; i <= z; i++) {
        int x; cin >> x;
        f[x]++;
        fi[x] = 1;
    }
    cin >> z;
    for (int i = 1; i <= z; i++) {
        int x; cin >> x;
        f[x]++;
        se[x] = 1;
    }
    vector < int > compress;
    for (int i = 1; i <= n; i++) {
        compress.push_back(c[i]);
    }
    sort (compress.begin(), compress.end());
    compress.resize(unique(compress.begin(), compress.end()) - compress.begin());
    for (int i = 1; i <= n; i++) {
        cc[c[i]] = lower_bound(compress.begin(), compress.end(), c[i]) - compress.begin();
        cc[c[i]] += 1;
    }
    for (int i = 1; i <= n; i++) {
        if (f[i] == 0) {
            update(1, 1, n, cc[c[i]], c[i]);
        } else if (f[i] == 2) {
            common.push_back(c[i]);
        } else if (fi[i]) {
            a.push_back(c[i]);
        } else {
            b.push_back(c[i]);
        }
    }
    sort(common.begin(), common.end());
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    sa.resize(a.size());
    sb.resize(b.size());
    for (int i = 0; i < (int) a.size(); i++) {
        if (i == 0) {
            sa[i] = a[i];
        } else {
            sa[i] = sa[i - 1] + a[i];
        }
    }
    for (int i = 0; i < (int) b.size(); i++) {
        if (i == 0) {
            sb[i] = b[i];
        } else {
            sb[i] = sb[i - 1] + b[i];
        }
    }
    z = 0;
    long long run_sum = 0;
    long long ans = 1LL << 60LL;
    for (int i = k; i < a.size(); i++) {
        update(1, 1, n, cc[a[i]], a[i]);
    }
    for (int i = k; i < b.size(); i++) {
        update(1, 1, n, cc[b[i]], b[i]);
    }
    int l = min(k, (int) a.size()) - 1;
    int r = min(k, (int) b.size()) - 1;
    for (int z = 0; z <= (int) common.size(); z++) {
        int need = max(0, k - z);
        if (need > (int) a.size()) {
            if (z < common.size()) {
                run_sum += common[z];
            }
            if ((need <= b.size()) && (r >= 0)) {
                update(1, 1, n, cc[b[r]], b[r]);
                r--;
            }
            continue;
        }
        if (need > (int) b.size()) {
            if (z < common.size()) {
                run_sum += common[z];
            }
            if ((need <= a.size()) && (l >= 0)) {
                update(1, 1, n, cc[a[l]], a[l]);
                l--;
            }
            continue;
        }
        if (need * 2 + z > m) {
            if (z < common.size()) {
                run_sum += common[z];
            }
            if ((need <= a.size()) and (l >= 0)) {
                update(1, 1, n, cc[a[l]], a[l]);
                l--;
            }
            if ((need <= b.size()) and (r >= 0)) {
                update(1, 1, n, cc[b[r]], b[r]);
                r--;
            }
            continue;
        }
        long long cand = run_sum;
        if (need > 0) {
            cand += sa[need - 1] + sb[need - 1];
        }
        int quantity = m - need - need - z;
        if (quantity != 0) {
            cand += query(1, 1, n, quantity);
        }
        ans = min(ans, cand);
        if (z < common.size()) {
            run_sum += common[z];
        }
        if ((need <= a.size()) && (l >= 0)) {
            update(1, 1, n, cc[a[l]], a[l]);
            l--;
        }
        if ((need <= b.size()) && (r >= 0)) {
            update(1, 1, n, cc[b[r]], b[r]);
            r--;
        }
    }
    if (ans >= 1LL << 60LL) {
        cout << "-1\n";
        return 0;
    }
    cout << ans << "\n";
}
