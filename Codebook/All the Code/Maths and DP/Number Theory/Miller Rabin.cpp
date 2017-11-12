// This is a deterministic variant of the Miller-Ragin primality test for
// signed 64-bit integers.

#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;

const vector<int> p = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

inline ull MultiplyMod(ull x, ull n, ull m) {
  ull a = 0, b = x % m;
  for (; n > 0; n >>= 1) {
    if (n & 1) a = (a + b) % m;
    b = (b << 1) % m;
  }
  return a % m;
}

inline ull PowerMod(ull x, ull n, ull m) {
  ull a = 1, b = x;
  for (; n > 0; n >>= 1) {
    if (n & 1) a = MultiplyMod(a, b, m);
    b = MultiplyMod(b, b, m);
  }
  return a % m;
}

inline bool IsPrime(long long n) {
  for (int i = 0; i < p.size(); i++) {
    if (n % p[i] == 0) return n == p[i];
  }
  if (n < p.back()) return false;
  ull t = n - 1;
  int s = 0;
  for (t = n - 1; !(t & 1); t >>= 1) s++;
  for (int i = 0; i < p.size(); i++) {
    ull r = PowerMod(p[i], t, n);
    if (r == 1) continue;
    bool ok = false;
    for (int j = 0; j < s && !ok; j++) {
      ok |= (r == (ull)n - 1);
      r = MultiplyMod(r, r, n);
    }
    if (!ok) return false;
  }
  return true;
}

int main() {
  assert(IsPrime(100000001000000029) == true);
  assert(IsPrime(500000006500000021) == false);
  return 0;
}
