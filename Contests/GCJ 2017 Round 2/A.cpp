#include "bits/stdc++.h"
using namespace std;

const int N = 110;

int n, p;
int g[N], remainders[N];
int dp[N][N][N][4];

inline int brute(int a, int b, int c, int cur_p) {
    if (a + b + c == 0) {
        return 0;
    }
    if (dp[a][b][c][cur_p] != -1) {
        return dp[a][b][c][cur_p];
    }
    dp[a][b][c][cur_p] = 0;
    if (a > 0) {
        dp[a][b][c][cur_p] = max(dp[a][b][c][cur_p], brute(a - 1, b, c, (cur_p - 1 + p) % p));
    }
    if (b > 0) {
        dp[a][b][c][cur_p] = max(dp[a][b][c][cur_p], brute(a, b - 1, c, (cur_p - 2 + p) % p));
    }
    if (c > 0) {
        dp[a][b][c][cur_p] = max(dp[a][b][c][cur_p], brute(a, b, c - 1, (cur_p - 3 + p) % p));
    }
    if (cur_p == 0) {
        dp[a][b][c][cur_p] += 1;
    }
    return dp[a][b][c][cur_p];
}

int main() {
    freopen ("inp.in", "r", stdin);
    freopen ("B.out", "w", stdout);
    int tc; cin >> tc;
    for (int qq = 1; qq <= tc; qq++) {
        cout << "Case #" << qq << ": ";
        cin >> n >> p;
        memset(remainders, 0, sizeof remainders);
        memset(dp, -1, sizeof dp);
        for (int i = 1; i <= n; i++) {
            cin >> g[i];
            remainders[g[i] % p]++;
        }
        int res = brute(remainders[1], remainders[2], remainders[3], 0);
        res += remainders[0];
        cout << res << "\n";
    }
}