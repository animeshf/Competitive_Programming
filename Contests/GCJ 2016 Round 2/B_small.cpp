#include "bits/stdc++.h"
using namespace std;

const int N = 16;

int n, k;
long double p[N];

inline long double solve(vector < long double > &arr) {
    long double res = 0.0;
    for (int mask = 0; mask < (1 << k); mask++) {
        if (__builtin_popcount(mask) != k / 2) {
            continue;
        }
        long double cur = 1.0;
        for (int i = 0; i < k; i++) {
            if (mask & (1 << i)) {
                cur *= arr[i];
            } else {
                cur *= (1.0 - arr[i]);
            }
        }
        res += cur;
    }
    return res;
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
        vector < long double > probs(k);
        long double ans = 0.0;
        for (int mask = 0; mask < (1 << n); mask++) {
            int id = 0;
            if (__builtin_popcount(mask) != k) {
                continue;
            }
            for (int i = 0; i < n; i++){
                if (mask & (1 << i)) {
                    probs[id++] = p[i + 1];
                }
            }
            assert(id == k);
            ans = max(ans, solve(probs));
        }
        cout << "Case #" << qq << ": ";
        cout << fixed << setprecision(8) << ans << "\n";
    }
}