#include "bits/stdc++.h"
using namespace std;

const int N = 100005;

int n, c, d;
int bit[2][N];

inline void update(int id, int x, int val) {
    for (int i = x; i < N; i += i & -i) {
        bit[id][i] = max(bit[id][i], val);
    }
}

inline int query(int id, int x) {
    int res = 0;
    for (int i = x; i > 0; i -= i & -i) {
        res = max(res, bit[id][i]);
    }
    return res;
}

int main() {
    cin >> n >> c >> d;
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        int b, p;
        char t;
        cin >> b >> p >> t;
        if (t == 'C') {
            int other_coin = query(0, c - p);
            int other_diam = query(1, d);
            int cur_best = max(other_diam, other_coin);
            if (cur_best && p <= c) {
                ans = max(ans, cur_best + b);
            }
            if (p <= c) update(0, p, b);
        } else {
            int other_coin = query(0, c);
            int other_diam = query(1, d - p);
            int cur_best = max(other_diam, other_coin);
            if (cur_best && p <= d) {
                ans = max(ans, cur_best + b);
            }
            if (p <= d) update(1, p, b);
        }
    }
    cout << ans << "\n";
}