#include "bits/stdc++.h"
using namespace std;

int n, m;
vector < vector < int > > arr;
vector < vector < int > > f, g;

inline void init() {
    scanf("%d %d", &n, &m);
    arr.resize(n, vector < int >(m, 0));
    f.resize(n, vector < int >(m, 0));
    g.resize(n, vector < int >(m, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &arr[i][j]);
        }
    }
}

inline void build() {
    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            f[i][j] = j;
            if (j + 1 < m) {
                if (arr[i][j] <= arr[i][j + 1]) {
                    f[i][j] = f[i][j + 1];
                }
            }
            int l = j, r = f[i][j];
            int res = j - 1;
            if (i + 1 < n) {
                while (l <= r) {
                    int mid = (l + r) / 2;
                    if (arr[i][mid] <= arr[i + 1][j]) {
                        res = mid;
                        l = mid + 1;
                    } else {
                        r = mid - 1;
                    }
                }
            }
            g[i][j] = res;
        }
    }
}

// g[i][j] <= f[i][j]

// <=
vector < int > above(int col, bool include) {
    vector < int > res(n, 0);
    for (int i = 0; i < n; i++) {
        res[i] = i;
    }
    // res[i] = how high up can we go such that f[i]/g[i] is the minimum (not strictly)
    stack < pair < int, int > > values;
    // Stack should be in increasing order
    values.push(make_pair(g[0][col], 0));
    for (int i = 1; i < n; i++) {
        int target = f[i][col];
        if (include) {
            target = g[i][col];
        }

        // target should be minimum (not strictly)
        while (!values.empty() && values.top().first >= target) {
            values.pop();
        }
        if (values.empty()) {
            res[i] = 0;
        } else {
            res[i] = values.top().second + 1;
        }

        // to maintain invariant for future
        target = g[i][col];
        while (!values.empty() && values.top().first >= target) {
            values.pop();
        }
        values.push(make_pair(g[i][col], i));
    }
    return res;
}

// <
vector < int > below(int col) {
    vector < int > res(n, 0);
    for (int i = 0; i < n; i++) {
        res[i] = i;
    }
    // res[i] = how low can we go such that g[i] is strictly minimum
    stack < pair < int, int > > values;
    // Stack should be in increasing order
    values.push(make_pair(g[n - 1][col], n - 1));
    for (int i = n - 2; i >= 0; i--) {
        int target = g[i][col];
        // target should be strictly minimum
        while (!values.empty()) {
            int value = values.top().first;
            int index = values.top().second;
            if (value > target) {
                values.pop();
            } else {
                if (f[index][col] > target) {
                    res[i] = index;
                } else {
                    res[i] = index - 1;
                }
                break;
            }
        }
        if (values.empty()) {
            res[i] = n - 1;
        }
        values.push(make_pair(g[i][col], i));
    }
    return res;
}

inline void solve() {
    long long res = 0;
    for (int col = 0; col < m; col++) {
        vector < int > above_inc = above(col, 1);
        vector < int > above_exc = above(col, 0);
        vector < int > below_inc = below(col);
        for (int row = 0; row < n; row++) {
            // stuff that ends here
            int up_dist_end_here = row - above_exc[row] + 1;
            res += (f[row][col] - col + 1) * 1LL * up_dist_end_here;

            // stuff that passes through this one
            int up_dist_end_below = row - above_inc[row] + 1;
            int down_dist = below_inc[row] - row;
            res += (g[row][col] - col + 1) * 1LL * up_dist_end_below * 1LL * down_dist;
        }
    }
    cout << res << endl;
}

int main() {
    init();
    build();
    solve();
}