// Golf Bot - Kattis
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

int main() {
    freopen ("inp.in", "r", stdin);
    ios :: sync_with_stdio(false);
    int n; cin >> n;
    int size = (1 << 19);
    vector < long long > a(size, 0), b(size, 0);
    a[0] = b[0] = 1;
    for (int i = 1; i <= n; i++) {
        int x; cin >> x;
        a[x] = 1; b[x] = 1;
    }
    fft(a, false);
    fft(b, false);
    vector < long long > c(size, 0);
    for (int i = 0; i < size; i++) {
        c[i] = a[i] * b[i] % mod;
    }
    fft(c, true);
    int m; cin >> m;
    int ans = 0;
    for (int i = 1; i <= m; i++) {
        int x; cin >> x;
        if (c[x] > 0) {
            ans++;
        }
    }
    cout << ans << "\n";
}