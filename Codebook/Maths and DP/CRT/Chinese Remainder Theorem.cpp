// SPOJ FACTMUL
#include "bits/stdc++.h"
using namespace std;

const long long MOD = 109546051211;
const int P = 186583;
const int Q = 587117;

// Chinese Remainder Theorem: O(n^2 log n)
// All credit for this code goes to Alex Li.
inline long long perform_mod(long long a, long long m) {
    long long r = a % m;
    return (r >= 0) ? r : (r + m);
}

inline long long mod_inverse(long long a, long long m) {
    a = perform_mod(a, m);
    if (a == 0) {
        return 0;
    } else {
        return perform_mod((1 - m * mod_inverse(m % a, a)) / a, m);
    }
}

// n is the size of a and p
// (a[i], p[i]) denotes the remainder of the value we're trying to compute modulo p[i].
// all elements in p[] should be pairwise coprime.
inline long long garner_restore(int n, int a[], int p[]) {
    vector < long long > x(a, a + n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            x[i] = mod_inverse((long long) p[j], (long long) p[i]) * (x[i] - x[j]);
        }
        x[i] = (x[i] % p[i] + p[i]) % p[i];
    }
    long long res = x[0], m = 1;
    for (int i = 1; i < n; i++) {
        m *= p[i - 1];
        res += x[i] * m;
    }
    return res;
}
// End of Template

inline int get(int n, int modulo) {
    long long result = 1;
    long long factorial = 1;
    for (int i = 1; i <= n; i++) {
        factorial = (factorial * i) % modulo;
        result = (result * factorial) % modulo;
    }
    return result;
}

int main() {
    int n; cin >> n;
    int a[2], p[2];
    a[0] = get(n, P); p[0] = P;
    a[1] = get(n, Q); p[1] = Q;
    cout << garner_restore(2, a, p) << endl;
}