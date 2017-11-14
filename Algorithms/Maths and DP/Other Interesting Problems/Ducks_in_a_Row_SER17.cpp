#include "bits/stdc++.h"
#define ii pair < int, int >
using namespace std;

const int N = 2005;
const int INF = 100000000;

/**
    Observation: We will never require to make two overlapping flips.
    The solution is thus a disjoint subset of intervals. Suppose we have
    'j' intervals, we want to pick them such that we get at least k segments
    of length at least n.

    Idea: dp[i][j][1] = Optimal way to make (j) flips among the first (i)
                        values if we flip the (i)th one. Optimal, in this
                        context means we first maximize the number of
                        segments of length n, and then we maximize the
                        length of the current segment.
          dp[i][j][0] = Same, but we don't flip (i)
**/

int n, k;
ii dp[N][N][2]; // (max_segments, current_length)

inline bool better(ii a, ii b) {
    if (a.first != b.first) {
        return a.first > b.first;
    } else {
        int a_length = (a.second >= n) ? (-1) : (a.second);
        int b_length = (b.second >= n) ? (-1) : (b.second);
        return a_length > b_length;
    }
}

inline ii max(ii a, ii b) {
    if (better(a, b)) {
        return a;
    }
    return b;
}

inline ii update(ii a, char x) {
    ii res;
    res.first = a.first;
    res.second = a.second;
    if (x == 'D') {
        res.second += 1;
        if (res.second == n) {
            res.first += 1;
        }
    } else {
        res.second = 0;
    }
    return res;
}

int main() {
    freopen ("inp.in", "r", stdin);
    cin >> n >> k;
    string s; cin >> s;
    int len = (int)(s.size());
    if (n * k > len) {
        cout << -1 << endl;
        return 0;
    }
    ii dummy = {-INF, -INF};
    for (int i = 0; i < len; i++) {
        for (int j = 0; j <= len; j++) {
            dp[i][j][0] = dp[i][j][1] = dummy;
        }
    }
    int res = INF;
    if (n == 1 && k == 1 && len == 1) {
        res = 1 - (s[0] == 'D');
    }
    if (s[0] == 'D') {
        dp[0][0][0] = {(n == 1) ? (1): (0), 1};
        dp[0][1][1] = {0, 0};
    } else {
        dp[0][0][0] = {0, 0};
        dp[0][1][1] = {(n == 1) ? (1): (0), 1};
    }
    for (int i = 1; i < len; i++) {
        for (int j = 0; j <= len; j++) {
            char other = 'D' ^ 'G' ^ s[i];
            {
                // Case 1: We won't flip (i)
                ii one = update(dp[i - 1][j][0], s[i]);
                ii two = update(dp[i - 1][j][1], s[i]);
                dp[i][j][0] = max(one, two);
            }
            if (j > 0) {
                // Case 2: We flip (i)
                ii one = update(dp[i - 1][j][1], other);
                ii two = update(dp[i - 1][j - 1][0], other);
                dp[i][j][1] = max(one, two);
            }
            if (max(dp[i][j][0], dp[i][j][1]).first >= k) {
                res = min(res, j);
            }
        }
    }
    if (res == INF) {
        cout << -1 << endl;
    } else {
        cout << res << endl;
    }
}