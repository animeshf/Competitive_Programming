#include "bits/stdc++.h"
using namespace std;

// Start of Integer FFT Template.
// Values are computed modulo 1.6e8
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
const long long root = 243;
const long long root_1 = 114609789;
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

// End to Integer FFT Template.

const int N = 1000000;
char str[N];

int main() {
    scanf("%s", str);
    int length = strlen(str);
    int size = 1;
    while (size < 2 * length) {
        size *= 2;
    }
    vector < long long > A(size, 0);
    vector < long long > B(size, 0);
    for (int i = 0; i < length; i++) {
        if (str[i] == 'A') {
            A[i] = 1;
        } else {
            B[length - i] = 1;
        }
    }
    fft(A, false);
    fft(B, false);
    vector < long long > C(size, 0);
    for (int i = 0; i < size; i++) {
        C[i] = A[i] * B[i] % mod;
    }
    fft(C, true);
    for (int i = length + 1; i < 2 * length; i++) {
        printf("%lld\n", C[i]);
    }
}
