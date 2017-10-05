// CodeForces: Library Management

// Given a list A with N elements where 1 <= A_i <= D, compute minimum number of swaps you must make
// to make each A_i appear as a contiguous block in the list. You can only swap adjacent elements.
// Example: [1 2 1 2] -> Answer = 1 (swap index 3 and index 2).
// The solution is O(N * D + 2^D * D^2) using bitmask dynamic programming.

#include "bits/stdc++.h"
using namespace std;

const int N = 100005;
const int D = 10;
const long long INF = 100000000000000LL;

// inv[i][j] = Number of (i, j) pairs such that pos_i < pos_j
// So if we place group (j) before group (i) in our permutation we would have to do atleast inv[i][j] swaps

int n;
int c[N], f[D];
long long inv[D][D];
long long dp[1 << D];

inline long long solve(int mask) {
    if (mask == (1 << D) - 1) {
        return 0;
    }
    if (dp[mask] != -1) {
        return dp[mask];
    }
    dp[mask] = INF;
    for (int i = 0; i < D; i++) {
        if (!((mask >> i) & 1)) {
            long long cost = 0;
            for (int j = 0; j < D; j++) {
                if ((mask >> j) & 1) {
                    // We placed j before we placed i.
                    cost += inv[i][j];
                }
            }
            dp[mask] = min(dp[mask], cost + solve(mask | (1 << i)));
        }
    }
    return dp[mask];
}

int main() {
    freopen ("inp.in", "r", stdin);
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &c[i]);
    }
    for (int i = n; i >= 1; i--) {
        int val = c[i];
        for (int j = 0; j < D; j++) {
            inv[val][j] += f[j];
        }
        f[val]++;
    }
    memset(dp, -1, sizeof dp);
    printf("%lld\n", solve(0));
}