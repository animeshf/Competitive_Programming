// Implementation of Lucas's theorem that computes Binomial(n, k) mod p, for
// a prime p, in terms of the base p expansions of n and k. This can be used
// with the Chinese remainder theorem to compute Binomial(n, k) mod m, for some
// square-free integer m.

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll p;
vector<ll> factorials, inverse_factorials;

ll powmod(ll b, ll e) {
  ll ans = 1;
  while (e) {
    if (e % 2) ans = (ans * b) % p;
    b = (b * b) % p;
    e /= 2;
  }
  return ans;
}

void GetFactorials() {
  factorials.resize(p);
  inverse_factorials.resize(p);
  factorials[0] = 1;
  inverse_factorials[0] = 1;
  for (int i = 1; i < p; i++) {
    factorials[i] = (i * factorials[i - 1]) % p;
    inverse_factorials[i] = powmod(factorials[i], p - 2);
  }
}

ll Binomial(ll n, ll k) {
  if (n < 0 || k < 0 || k > n) return 0;
  ll tmp = (factorials[n] * inverse_factorials[k]) % p;
  return (tmp * inverse_factorials[n - k]) % p;
}

ll LucasBinomial(ll n, ll k) {
  ll ans = 1;
  while (n || k) {
    ans = (ans * Binomial(n % p, k % p)) % p;
    n /= p;
    k /= p;
  }
  return ans;
}

int main() {
  p = 10000019; // Set the prime as required
  GetFactorials(); // Precompute Factorials
  // Now LucasBinomial(n, k) returns (n choose k) modulo p.
  assert(LucasBinomial(1000000, 500000) == 2421826);
  return 0;
}
