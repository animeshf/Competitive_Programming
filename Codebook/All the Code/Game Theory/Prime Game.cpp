// Map DP to compute Grundy Numbers

#include "bits/stdc++.h"
using namespace std;

map < int, int > prime_mask; // Exponents present for each prime
map < int, int > memo;
set < int > primes;

inline void factorize(int x) {
    for (int p = 2; p * p <= x; p++) {
        int cnt = 0;
        while ((x % p) == 0) {
            x /= p;
            cnt += 1;
        }
        if (cnt > 0) {
            prime_mask[p] |= (1 << cnt);
            primes.insert(p);
        }
    }
    if (x != 1) {
        prime_mask[x] |= (1 << 1);
        primes.insert(x);
    }
}

inline int grundy(int mask) {
    if (mask == 0) {
        return memo[mask] = 0;
    }
    if (memo.count(mask)) {
        return memo[mask];
    }
    set < int > reachable_states;
    for (int k = 1; k <= 30; k++) {
        int new_mask = 0;
        for (int i = 1; i <= 30; i++) {
            if ((mask >> i) & 1) {
                if (i < k) {
                    new_mask |= (1 << i);
                } else if (i > k) {
                    new_mask |= (1 << (i - k));
                }
            }
        }
        if (new_mask != mask) {
            reachable_states.insert(grundy(new_mask));
        }
    }
    int mex = 0;
    while (reachable_states.count(mex)) {
        mex += 1;
    }
    return memo[mask] = mex;
}

int main() {
    ios :: sync_with_stdio(false);
    int n; cin >> n;
    for (int i = 1; i <= n; i++) {
        int x; cin >> x;
        factorize(x);
    }
    int res = 0;
    for (int prime_factor: primes) {
        memo.clear();
        res ^= grundy(prime_mask[prime_factor]);
    }
    if (not res) {
        cout << "Arpa" << endl;
    } else {
        cout << "Mojtaba" << endl;
    }
}