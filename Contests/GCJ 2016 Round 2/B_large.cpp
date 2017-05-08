#include "bits/stdc++.h"
using namespace std;

const int N = 205;

int n, k;
long double p[N];
long double dp[N][N];

inline long double solve(vector < long double > &arr) {
    for (int i = 0; i <= k / 2; i++) {
        for (int j = 0; j <= k; j++) {
            dp[i][j] = 0.0;
        }
    }
    // dp[i][j] = exactly (i) yes votes, using people 1..j
    dp[0][0] = 1.0;
    for (int i = 1; i <= k; i++) {
        dp[0][i] = dp[0][i - 1] * (1.0 - arr[i]);
    }
    for (int i = 1; i <= k / 2; i++) {
        for (int j = 1; j <= k; j++) {
            dp[i][j] = dp[i][j - 1] * (1.0 - arr[j]);
            dp[i][j] += (dp[i - 1][j - 1] * arr[j]);
        }
    }
    return dp[k / 2][k];
}

int main() {
    freopen ("B.in", "r", stdin);
    freopen ("B.out", "w", stdout);
    int t; cin >> t;
    for (int qq = 1; qq <= t; qq++) {
        cin >> n >> k;
        for (int i = 1; i <= n; i++) {
            cin >> p[i];
        }
        // We'll pick some prefix and some suffix always.
        sort (p + 1, p + n + 1);
        vector < long double > probs(k + 1);
        long double ans = 0.0;
        for (int i = 1; i <= n; i++) {
            int cur = i;
            int id = 0;
            for (int len = 1; len <= k; len++) {
                probs[++id] = p[cur];
                cur = cur + 1;
                if (cur > n) {
                    cur = 1;
                }
            }
            ans = max(ans, solve(probs));
        }
        cout << "Case #" << qq << ": ";
        cout << fixed << setprecision(8) << ans << "\n";
    }
}