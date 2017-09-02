// NENA 2016 E
// There is no way to test this code (without making your own data), so don't count on it.

#include "bits/stdc++.h"
using namespace std;

const int N = 1000005;
const int MAGIC = 200;

int n;
int x[N], y[N];

inline double compute_intersection(int x1, int y1, int x2, int y2, double h) {
    if (x2 == x1) {
        return x1 * 1.0;
    }
    double slope = ((y2 - y1) * 1.0) / ((x2 - x1) * 1.0);
    return (h - (y1 * 1.0)) / slope + (x1 * 1.0);
}

inline double solve(double height) {
    double res = 0.0;
    double st_x = -1.0, en_x = -1.0;
    for (int i = 1; i < n; i++) {
        if (st_x == -1.0) {
            if (y[i] <= height and y[i + 1] >= height) {
                st_x = compute_intersection(x[i], y[i], x[i + 1], y[i + 1], height);
            }
        } else {
            if (y[i] >= height and y[i + 1] < height) {
                en_x = compute_intersection(x[i], y[i], x[i + 1], y[i + 1], height);
                res = max(res, (en_x - st_x) * height);
                st_x = en_x = -1.0;
            }
        }
    }
    return res;
}

int main() {
    freopen ("inp.in", "r", stdin);
    ios :: sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        cin >> n;
        for (int i = 1; i <= n; i++) {
            cin >> x[i] >> y[i];
        }
        double l = 1e-9, r = 1e9;
        double ans = 0.0;
        for (int i = 1; i <= MAGIC; i++) {
            double p1 = l + (r - l) / 3.0;
            double p2 = r - (r - l) / 3.0;
            double x = solve(p1);
            double y = solve(p2);
            ans = max(ans, max(x, y));
            if (x < y) {
                l = p1;
            } else {
                r = p2;
            }
        }
        cout << fixed << setprecision(2) << ans << endl;
    }
}
