#include "bits/stdc++.h"
using namespace std;

const int N = 257;
const long long INF = 1LL << 50LL;

long long pixels[N];
long long dp[N][N];

inline long long solve(int index, int rem) {
    if (dp[index][rem] != -1) {
        return dp[index][rem];
    }
    if (index == N - 1) {
        if (rem == 0) {
            return dp[index][rem] = 0;
        } else {
            return dp[index][rem] = INF;
        }
    }
    if (rem == 0) {
        long long cost = 0;
        for (int i = index + 1; i < N; i++) {
            long long dist = i - index;
            cost += dist * dist * pixels[i];
        }
        return dp[index][rem] = cost;
    }
    dp[index][rem] = INF;
    for (int place = index + 1; place < N; place++) {
        long long cost = 0;
        for (int i = index + 1; i < place; i++) {
            long long x = (index == 0) ? (INF) : (i - index);
            long long y = place - i;
            long long z = min(x, y);
            cost += z * z * pixels[i];
        }
        dp[index][rem] = min(dp[index][rem], cost + solve(place, rem - 1));
    }
    return dp[index][rem];
}

int main() {
    ios :: sync_with_stdio(false);
    int d, k; cin >> d >> k;
    for (int i = 1; i <= d; i++) {
        int location; cin >> location;
        cin >> pixels[location + 1];
    }
    memset(dp, -1, sizeof dp);
    cout << solve(0, k) << "\n";
}