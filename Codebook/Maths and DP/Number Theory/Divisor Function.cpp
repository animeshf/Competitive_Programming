// Implementation of the sigma function for computing the number of divisors
// and the sum of the divisors of a number, given its prime factorization.

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll NumberOfDivisors(const map<ll, ll>& factors) {
  ll ans = 1;
  for (auto pe : factors) ans *= pe.second + 1;
  return ans;
}

ll SumOfDivisors(const map<ll, ll>& factors) {
  ll ans = 1;
  for (auto pe : factors) {
    ll power_sum = 0;
    ll term = 1;
    for (ll i = 0; i <= pe.second; i++) {
      power_sum += term;
      term *= pe.first;
    }
    ans *= power_sum;
  }
  return ans;
}

int main() {
  map<ll, ll> factors = {{2, 10}, {3, 6}, {1009, 1}};
  assert(NumberOfDivisors(factors) == 154);
  assert(SumOfDivisors(factors) == 2259744710);
  return 0;
}
