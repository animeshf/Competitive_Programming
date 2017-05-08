#include "bits/stdc++.h"
using namespace std;

const int N = 10000 + 50;
const int W = 100 + 50;
const int MOD = 1000000007;

inline int add(int x, int y) {
    int res = x + y;
    if (res >= MOD) {
        res -= MOD;
    }
    if (res < 0) {
        res += MOD;
    }
    return res;
}

int n, w, h;
int dp[N][W];

int main(){
    cin >> n >> w >> h;
    /*
        We need number of solutions to x_1 + x_2 + ... x_w <= N, 0 <= x_i <= H.
        This is equivalent to the following problem :-
        Count number of ways to add dividers between indices in an array of N elements,
        such that no two adjacent dividers are separated by more than H.
        Let's do the following dp :
        dp[i][j] = Add j dividers in 1...i such that the j'th divider is between i and i + 1.
    */
    for (int i = 0; i <= n; i++) {
        for (int j = 1; j <= w; j++) {
            if (j == 1) {
                if (i <= h) { // The first partition can be anywhere in [0...h] but not after it
                    dp[i][j] = 1;
                } else {
                    dp[i][j] = 0;
                }
            } else {
                for (int k = max(0, i - h); k <= i; k++) {
                    dp[i][j] = add(dp[i][j], dp[k][j - 1]);
                }
            }
        }
    }
    int ans = 0;
    for (int i = 0; i <= n; i++) {
        ans = add(ans, dp[i][w]);
    }
    for (int i = 0; i <= h; i++) {
        if (n >= i * w) ans = add(ans, -1);
    }
    cout << ans << '\n';
}