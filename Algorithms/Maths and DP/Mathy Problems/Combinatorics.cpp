#include "bits/stdc++.h"
using namespace std;

// Returns N choose R. Takes O(N log MOD) for precomputation, and then O(1) per query.
// Adjust N and MOD as required -> MOD must be prime.
// Call preprocess() from your main() once.
// After that, choose(n, k) returns (n choose k) modulo (MOD).

const int N = 200005;
const int MOD = 1000000007;

int fact[N], inv_fact[N];

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
    for(int i = 1; i < N; i++) fact[i] = prod(fact[i - 1], i);
    for(int i = N - 1; i >= 1; i--) inv_fact[i] = power(fact[i], MOD - 2);
}

inline int choose(int n, int r){
    if(r < 0 || r > n) return 0;
    return prod(fact[n], prod(inv_fact[r], inv_fact[n - r]));
}

// End of Counting Template

int main(){
    preprocess();
    cout << choose(10, 2) << endl;
}
