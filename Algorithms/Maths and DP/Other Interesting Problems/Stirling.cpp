// ICM Technex 2018 E

#include "bits/stdc++.h"
using namespace std;

const int K = 5050;
const int MOD = 1000000007;

inline int add(int x, int y) {
    int res = x + y;
    if (res >= MOD) {
        res %= MOD;
    }
    return res;
}

inline int prod(int x, int y) {
    long long res = x * 1LL * y;
    if (res >= MOD) {
        res %= MOD;
    }
    return res;
}

int n, k;
int factorials[K];
int stirling[K][K];

inline int power(int a, int b) {
    if (b < 0) {
        return 0;
    }
    if (b == 0) {
        return 1;
    }
    if (b == 1) {
        return a;
    }
    int res = power(a, b >> 1);
    res = prod(res, res);
    if (b & 1) {
        res = prod(res, a);
    }
    return res;
}

inline int choose(int n, int x) {
    if (x > n) {
        return 0;
    }
    int res = n;
    for (int i = 1; i <= x - 1; i++) {
        res = prod(res, n - i);
    }
    return prod(res, power(factorials[x], MOD - 2));
}

int main() {
    freopen ("inp.in", "r", stdin);
    cin >> n >> k;
    factorials[0] = 1;
    for (int i = 1; i < K; i++) {
        factorials[i] = prod(factorials[i - 1], i);
    }
    stirling[0][0] = 1;
    for (int i = 1; i < K; i++) {
        for (int j = 1; j < K; j++) {
            stirling[i][j] = add(stirling[i - 1][j - 1], prod(j, stirling[i - 1][j]));
        }
    }
    int res = 0;
    for (int x = 1; x <= k; x++) {
        res = add(res, prod(prod(factorials[x], stirling[k][x]), prod(choose(n, x), power(2, n - x))));
    }
    cout << res << endl;
}