#include "bits/stdc++.h"
using namespace std;

const int N = 100005;
const int INF = 1000000000;

int a, b, h, w, n;
int arr[N];
map < pair < pair < int, int >, int >, int > dp;

inline int solve(int idx, int hh, int ww) {
    if (hh < 0) hh = INF;
    if (ww < 0) ww = INF;
    if (hh > ww) {
        swap(hh, ww);
    }
    pair < pair < int, int >, int > item = make_pair(make_pair(idx, hh), ww);
    if (dp.count(item)) {
        return dp[item];
    }
    if (hh >= a && ww >= b) {
        return dp[item] = 0;
    }
    if (idx >= 35) {
        return INF;
    }
    dp[item] = INF;
    if (hh < N) {
        dp[item] = 1 + solve(idx + 1, hh * arr[idx], ww);
    }
    if (ww < N) {
        dp[item] = min(dp[item], 1 + solve(idx + 1, hh, ww * arr[idx]));
    }
    return dp[item];
}


int main() {
    freopen ("inp.in", "r", stdin);
    ios :: sync_with_stdio(false);
    cin >> a >> b >> h >> w >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    if (a > b) {
        swap(a, b);
    }
    sort(arr + 1, arr + 1 + n);
    reverse(arr + 1, arr + 1 + n);
    int res = solve(1, h, w);
    if (res >= INF) {
        cout << "-1\n";
    } else {
        cout << res << "\n";
    }
}
