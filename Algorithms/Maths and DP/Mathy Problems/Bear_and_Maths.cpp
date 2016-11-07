/*
	Round 356 Div 1B
*/

#include "bits/stdc++.h"
using namespace std;

pair < int, long long > res;
long long m;

inline long long f(long long x){
	return (x * x * x);
}

inline void solve(long long cur, int blocks, long long x){
	if(cur == 0){
		res = max(res, make_pair(blocks, x));
		return;
	}
	long long a = 1;
	while(f(a + 1) <= cur) a = a + 1;
	solve(cur - f(a), blocks + 1, x + f(a));
	if(a > 1) solve(f(a) - 1 - f(a - 1), blocks + 1, x + f(a - 1));
}

int main(){
	ios :: sync_with_stdio(false);
	cin >> m;
	solve(m, 0, 0);
	cout << res.first << ' ' << res.second << '\n';
}