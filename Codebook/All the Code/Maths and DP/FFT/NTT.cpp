// SPOJ MAXMATCH
#include "bits/stdc++.h"
using namespace std;

/*
    ========== Number Theoretic Transform ===========

    The code below can be used to multiply two polynomials in O(n log n).
    The multiplication happens modulo 5 * (2 ^ 25) + 1 which is around 1.6e8.
    This implementation has been thoroughly tested and can be used when
    dealing with integers. Some implementation details:

    - n should be a power of 2. Generally, if we are multiplying two degree 'd'
      polynomials, n should be the smallest power of two greater than 2 * d.
    - Changing the modulo requires some number theoretic results. Most modulos
      do not work. Here is what we do to deal with modulos:
    - Suppose the modulo is M. M must be prime and of the form 2^k * x + 1
      where k >= ceil(log n) and x >= 1. To make the code below work for a particular
      M, we need to change the 4 constants below. Here is how we find the
      constants:
       - mod: M
       - root_pw: 2^k
       - root: Let p = find_primitive_root(M). Then root = (p ^ x) % M
       - root_1: inverse(root, M)

       We can work out these values offline by using the find_primitive_root()
       and inverse() functions below, and then hardcode them into the program.

    - Suppose the modulo is not of the form 2^k * x + 1, and we know that in
      the product polynomial, the coefficients will be less than ~1e15. Then
      we can compute NTT using large primes p1, p2 (around 1e7) and then
      compute the value modulo (p1 * p2) using CRT. This will give us a
      very high precision result :)

    - P1 =  5 * (2 ^ 25) + 1
      P2 =  7 * (2 ^ 20) + 1
      P1 * P2 >= 1e15.
    ==================================================
*/

inline long long gcd(long long a, long long b, long long &s, long long &t) {
    if (b == 0) {
        t = 0;
        s = (a < 0) ? -1 : 1;
        return (a < 0) ? -a : a;
    } else {
        long long g = gcd(b, a % b, t, s);
        t -= a / b * s;
        return g;
    }
}

inline long long inverse(long long n, long long mod) {
    long long s, t;
    gcd(n, mod, s, t);
    return (s > 0) ? s : s + mod;
}

const long long mod = 5 * (1 << 25) + 1;
long long root = 243;
long long root_1 = 114609789;
const long long root_pw = 1 << 25;

inline void fft (vector < long long > & a, bool invert) {
    int n = (int) a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j >= bit; bit >>= 1) {
            j -= bit;
        }
        j += bit;
        if (i < j) {
          swap(a[i], a[j]);
        }
    }
    for (int len = 2; len <= n; len <<= 1) {
        long long wlen = invert ? root_1 : root;
        for (long long i = len; i < root_pw; i <<= 1)
            wlen = (long long) (wlen * 1ll * wlen % mod);
        for (int i = 0; i < n; i += len) {
            long long w = 1;
            for (int j = 0; j < len / 2; j++) {
                long long u = a[i + j];
                long long v = (long long) (a[i + j + len / 2] * 1ll * w % mod);
                a[i + j] = u + v < mod ? u + v : u + v - mod;
                a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + mod;
                w = (long long) (w * 1ll * wlen % mod);
            }
        }
    }
    if (invert) {
        long long nrev = inverse(n, mod);
        for (int i = 0; i < n; i++)
          a[i] = (long long) (a[i] * 1ll * nrev % mod);
    }
}

// ============ Stuff to figure out the 4 constants =============
inline int power(int a, int b, int modulo) {
    if (b == 0) {
        return 1 % modulo;
    }
    if (b == 1) {
        return a % modulo;
    }
    int res = power(a, b / 2, modulo);
    res = (res * 1LL * res) % modulo;
    if (b & 1) {
        res = (res * 1LL * a) % modulo;
    }
    return res;
}

vector < int > factorize(int x) {
    // Returns prime factors of x
    vector < int > primes;
    for (int i = 2; i * i <= x; i++) {
        if (x % i == 0) {
            primes.push_back(i);
            while (x % i == 0) {
                x /= i;
            }
        }
    }
    if (x != 1) {
        primes.push_back(x);
    }
    return primes;
}

inline bool test_primitive_root(int a, int m) {
    // Is 'a' a primitive root of modulus 'm'?
    // m must be of the form 2^k * x + 1
    int exp = m - 1;
    int val = power(a, exp, m);
    if (val != 1) {
        return false;
    }
    vector < int > factors = factorize(exp);
    for (int f: factors) {
        int cur = exp / f;
        val = power(a, cur, m);
        if (val == 1) {
            return false;
        }
    }
    return true;
}

inline int find_primitive_root(int m) {
    // Find primitive root of the modulus 'm'.
    // m must be of the form 2^k * x + 1
    for (int i = 2; ; i++) {
        if (test_primitive_root(i, m)) {
            return i;
        }
    }
}

// End of NTT Template.

const int MAXN = 100000;
const int SIZE = 1 << 19;

int n;
char s[MAXN];

int main() {
    freopen ("inp.in", "r", stdin);
    // Adjust constants
    int primitive_root = find_primitive_root(mod);
    root = power(primitive_root, 5, mod); // 5 = x from the template defn.
    root_1 = inverse(root, mod);
    // Solve problem
    scanf("%s", s);
    n = strlen(s);
    int off = n + 1;
    vector < long long > result(SIZE, 0);
    for (char x = 'a'; x <= 'c'; x++) {
        vector < long long > a(SIZE, 0);
        vector < long long > b(SIZE, 0);
        vector < long long > c(SIZE, 0);
        for (int i = 0; i < n; i++) {
            if (s[i] == x) {
                a[(i + 1) + off] = 1;
                b[-(i + 1) + off] = 1;
            }
        }
        fft(a, false);
        fft(b, false);
        for (int i = 0; i < SIZE; i++) {
            c[i] = a[i] * b[i] % mod;
        }
        fft(c, true);
        for (int i = 0; i < SIZE; i++) {
            result[i] += c[i];
        }
    }
    int mx_val = 0;
    set < int > indices;
    for (int i = 1; i <= n; i++) {
        int j = i + off + off;
        if (result[j] > mx_val) {
            mx_val = result[j];
            indices.clear();
            indices.insert(i);
        } else if (result[j] == mx_val) {
            indices.insert(i);
        }
    }
    printf("%d\n", mx_val);
    for (int i: indices) {
        printf("%d ", i);
    }
    printf("\n");
}