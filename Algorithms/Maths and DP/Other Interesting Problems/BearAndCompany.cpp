// Bear and Company - Codeforces
#include "bits/stdc++.h"
using namespace std;

const int N = 80;
const int INF = 100000000;

int n, nv, nk, nx;
int pos_v[N], pos_k[N], pos_x[N];
int vsum[N], ksum[N], xsum[N];
int dp[N][N][N][2]; // < cnt_v, cnt_k, cnt_x, last_v >
string s;

inline int cost(int v, int k, int x, int id) {
    return max(vsum[id - 1] - v, 0) + max(ksum[id - 1] - k, 0) + max(xsum[id - 1] - x, 0);
}

int main() {
    freopen ("inp.in", "r", stdin);
    cin >> n >> s;
    s = " " + s;
    for (int i = 1; i <= n; i++) {
        vsum[i] = vsum[i - 1];
        ksum[i] = ksum[i - 1];
        xsum[i] = xsum[i - 1];
        if (s[i] == 'V') {
            pos_v[++nv] = i;
            vsum[i]++;
        } else if (s[i] == 'K') {
            pos_k[++nk] = i;
            ksum[i]++;
        } else {
            pos_x[++nx] = i;
            xsum[i]++;
        }
    }
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            for (int k = 0; k <= n; k++) {
                dp[i][j][k][0] = INF;
                dp[i][j][k][1] = INF;
            }
        }
    }
    dp[0][0][0][0] = dp[0][0][0][1] = 0;
    for (int v = 0; v <= nv; v++) {
        for (int k = 0; k <= nk; k++) {
            for (int x = 0; x <= nx; x++) {
                for (int last_v = 0; last_v < 2; last_v++) {
                    if (v < nv) {
                        dp[v + 1][k][x][1] = min(dp[v + 1][k][x][1], dp[v][k][x][last_v] + cost(v, k, x, pos_v[v + 1]));
                    }
                    if ((k < nk) && (!last_v)) {
                        dp[v][k + 1][x][0] = min(dp[v][k + 1][x][0], dp[v][k][x][last_v] + cost(v, k, x, pos_k[k + 1]));
                    }
                    if (x < nx) {
                        dp[v][k][x + 1][0] = min(dp[v][k][x + 1][0], dp[v][k][x][last_v] + cost(v, k, x, pos_x[x + 1]));
                    }
                }
            }
        }
    }
    cout << min(dp[nv][nk][nx][0], dp[nv][nk][nx][1]) << "\n";
    return 0;
}