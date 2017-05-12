#include "bits/stdc++.h"
using namespace std;

const int N = 105;

int n, m;
int a[N][N];
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, -1, 0, 1};
string arrow = "^<v>";

inline int get(char x) {
    for (int i = 0; i < 4; i++) {
        if (x == arrow[i]) {
            return i;
        }
    }
    return -1;
}

inline bool goesOff(int x, int y, int d) {
    int cx = x, cy = y;
    do {
        if ((cx != x) || (cy != y)) {
            if (a[cx][cy] >= 0) // Arrow
                return false;
        }
        cx += dx[d];
        cy += dy[d];
    } while ((cx >= 1 && cx <= n) && (cy >= 1 && cy <= m));
    return true;
}

int main() {
    freopen ("inp.in", "r", stdin);
    freopen ("A.out", "w", stdout);
    int tc; cin >> tc;
    for (int qq = 1; qq <= tc; qq++) {
        cout << "Case #" << qq << ": ";
        cin >> n >> m;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                char c; cin >> c;
                a[i][j] = get(c);
            }
        }
        bool ok = true;
        int res = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (a[i][j] >= 0) {
                    if (goesOff(i, j, a[i][j])) {
                        bool found = false;
                        for (int k = 0; k < 4; k++) {
                            if (k == a[i][j]) continue;
                            if (!goesOff(i, j, k))
                                found = true;
                        }
                        if (found) res++;
                        else ok = false;
                    }
                }
            }
        }
        if (ok) {
            cout << res << "\n";
        } else {
            cout << "IMPOSSIBLE\n";
        }
    }
}