// LightOJ - 1137

#include "bits/stdc++.h"
using namespace std;

const int N = 50005;

int t;
double l, n, c;

inline double solve(double h) {
    double diameter = h + (l * l) / (4.0 * h);
    double radius = diameter / 2.0;
    double theta = asin(l / (2.0 * radius));
    // Arc length
    double result = (2 * theta * radius);
    return result;
}

int main() {
    cin >> t;
    cout << fixed << setprecision(10);
    for (int qq = 1; qq <= t; qq++) {
        cout << "Case " << qq << ": ";
        cin >> l >> n >> c;
        double arc_length = (1.0 + n * c) * l;
        double qs = 0.0, qe = l / 2.0;
        for (int i = 1; i <= 200; i++) {
            double height = (qs + qe) / 2.0;
            if (solve(height) >= arc_length) {
                qe = height;
            } else {
                qs = height;
            }
        }
        cout << qe << endl;
    }
    return 0;
}