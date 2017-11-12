#include "bits/stdc++.h"
using namespace std;

// extended_euclid(a, b) returns a pair (x, y) of integers such that gcd(a, b) = a * x + b * y.
inline pair < long long, long long > extended_euclid(long long a, long long b) {
    long long x = 1, y = 0, x1 = 0, y1 = 1;
    while (b != 0) {
        long long q = a / b, prev_x1 = x1, prev_y1 = y1, prev_b = b;
        x1 = x - q * x1;
        y1 = y - q * y1;
        b = a - q * b;
        x = prev_x1;
        y = prev_y1;
        a = prev_b;
    }
    return (a > 0) ? (make_pair(x, y)) : (make_pair(-x, -y));
}

int main() {
    pair < long long, long long > a = extended_euclid(30, 7);
    printf("%lld %lld\n", a.first, a.second);
}