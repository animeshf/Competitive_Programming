#include "bits/stdc++.h"
using namespace std;

const int N = 1234567;

#define ll long long
struct Line {
	mutable ll k, m, p;
	bool operator<(const Line& o) const { return k < o.k; }
	bool operator<(ll x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
	// (for doubles, use inf = 1/.0, div(a,b) = a/b)
	const ll inf = LLONG_MAX;
	ll div(ll a, ll b) { // floored division
		return a / b - ((a ^ b) < 0 && a % b); }
	bool isect(iterator x, iterator y) {
		if (y == end()) { x->p = inf; return false; }
		if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
		else x->p = div(y->m - x->m, x->k - y->k);
		return x->p >= y->p;
	}
	void add(ll k, ll m) {
		auto z = insert({k, m, 0}), y = z++, x = y;
		while (isect(y, z)) z = erase(z);
		if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
		while ((y = x) != begin() && (--x)->p >= y->p)
			isect(x, erase(y));
	}
	ll query(ll x) {
		assert(!empty());
		auto l = *lower_bound(x);
		return l.k * x + l.m;
	}
};
#undef ll

struct rectangle {
    long long x, y, a;
    inline friend bool operator < (rectangle p, rectangle q) {
        if (p.x == q.x) return p.y < q.y;
        return p.x < q.x;
    }
}arr[N];

int n;
long long dp[N];
LineContainer lines;

int main() {
    ios :: sync_with_stdio(false);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i].x >> arr[i].y >> arr[i].a;
    }
    sort(arr + 1, arr + n + 1);
    long long res = 0;
    lines.add(0, 0);
    for (int i = 1; i <= n; i++) {
        long long best = lines.query(arr[i].y);
        dp[i] = best + (arr[i].x * arr[i].y - arr[i].a);
        lines.add(-arr[i].x, dp[i]);
        res = max(res, dp[i]);
    }
    cout << res << endl;
}
