#include "bits/stdc++.h"
using namespace std;

const int N = 1005;
const int INF = 1000000000;
const int MAGIC = 100;
const double EPS = 1e-8;

int n, t;
int d[N], s[N];

inline double solve(double c) {
    double cur_time = 0.0;
    for (int i = 1; i <= n; i++) {
        cur_time += (d[i] * 1.0) / (s[i] + c);
    }
    return cur_time;
}

int main() {
    ios :: sync_with_stdio(false);
    cin >> n >> t;
    double lowest_c = -INF;
    for (int i = 1; i <= n; i++) {
        cin >> d[i] >> s[i];
        lowest_c = max(lowest_c, -1.0 * s[i]);
    }
    double l = lowest_c, r = INF;
    double ans = 0.0;
    for (int i = 1; i <= MAGIC; i++) {
        double mid = (l + r) / 2.0;
        double time_taken = solve(mid);
        if (time_taken > t * 1.0) {
            ans = mid;
            l = mid + EPS;
        } else {
            r = mid - EPS;
        }
    }
    cout << fixed << setprecision(10) << ans << "\n";
}