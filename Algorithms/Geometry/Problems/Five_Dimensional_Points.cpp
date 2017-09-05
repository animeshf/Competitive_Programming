// CF 432 - A

#include "bits/stdc++.h"
using namespace std;

const int N = 1005;

int n;
int a[N][N];

inline bool is_acute(int i, int j, int k) {
    int res = 0;
    for (int d = 1; d <= 5; d++) {
        res += (a[j][d] - a[i][d]) * (a[k][d] - a[i][d]);
    }
    return (res > 0);
}

int main() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= 5; j++) {
            cin >> a[i][j];
        }
    }
    vector < int > sol;
    for (int i = 1; i <= n; i++) {
        bool ok = true;
        for (int j = 1; j <= n; j++) {
            for (int k = 1; k <= n; k++) {
                if (i == j || i == k || j == k) {
                    continue;
                }
                if (is_acute(i, j, k)) {
                    ok = false;
                    break;
                }
            }
            if (not ok) {
                break;
            }
        }
        if (ok) {
            sol.push_back(i);
        }
    }
    cout << (int) sol.size() << endl;
    for (int p: sol) {
        cout << p << endl;
    }
}