// Given a HxW grid with N black cells, compute number of paths from (1, 1) to (H, W)
// that does not go through any black cell. Report the answer modulo 1000000007.
// H, W <= 100000 and N <= 5000.
// This solution is O(N^2).

#include "bits/stdc++.h"
using namespace std;

const int H = 200005;
const int N = 2005;
const int MOD = 1000000007;

int h, w, n;
int fact[H], inv_fact[H];
int dp[N];

inline int prod(int x, int y){
    long long res = x * 1LL * y;
    if(res >= MOD) res %= MOD;
    return res;
}

inline int add(int x, int y){
    int res = x + y;
    if (res < 0) res += MOD;
    if(res >= MOD) res -= MOD;
    return res;
}

inline int power(int x, int y){
    if(y == 0) return 1;
    int res = power(x, y >> 1);
    res = prod(res, res);
    if(y & 1) res = prod(res, x);
    return res;
}

inline void preprocess(){
    fact[0] = inv_fact[0] = 1;
    for(int i = 1; i < H; i++) fact[i] = prod(fact[i - 1], i);
    for(int i = H - 1; i >= 1; i--) inv_fact[i] = power(fact[i], MOD - 2);
}

inline int choose(int n, int r){
    if(r < 0 || r > n) return 0;
    return prod(fact[n], prod(inv_fact[r], inv_fact[n - r]));
}

inline int solve(int a1, int b1, int a2, int b2) {
    int length_of_path = (a2 - a1) + (b2 - b1);
    int number_of_turns = (b2 - b1);
    return choose(length_of_path, number_of_turns);
}

int main() {
    ios :: sync_with_stdio(false);
    preprocess();
    cin >> h >> w >> n;
    vector < pair < int, int > > black_cells;
    for (int i = 1; i <= n; i++) {
        int foo, bar; cin >> foo >> bar;
        black_cells.push_back({foo, bar});
    }
    black_cells.push_back({h, w}); // For convenience.
    sort(black_cells.begin(), black_cells.end());
    // dp[i] = number of paths from (1, 1) to (x_i, y_i)
    // which doesn't pass through any (x_j, y_j) for all j < i.
    // Standard counting trick: Fix the first time we break the rule.
    for (int i = 0; i <= n; i++) {
        int x = black_cells[i].first;
        int y = black_cells[i].second;
        dp[i] = solve(1, 1, x, y);
        for (int j = 0; j < i; j++) {
            int a = black_cells[j].first;
            int b = black_cells[j].second;
            if (a <= x and b <= y) {
                dp[i] = add(dp[i], -prod(dp[j], solve(a, b, x, y)));
            }
        }
    }
    cout << dp[n] << endl;
}