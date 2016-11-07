// Some codeforces round
// eq[i] = number of equivalence classes with (i) elements.

#include <bits/stdc++.h>
using namespace std;

const int N   = 4005;
const int MOD = 1000000007;

inline int prod(int x, int y){
	long long res = x * 1LL * y;
	if(res >= MOD) res %= MOD;
	return res;
}

inline int add(int x, int y){
	int res = x + y;
	if(res >= MOD) res -= MOD;
	return res;
}

int dp[N][N], eq[N];
int fact[N], inv_fact[N];

inline int power(int x, int y){
	if(y == 0) return 1;
	int res = power(x, y >> 1);
	res = prod(res, res);
	if(y & 1) res = prod(res, x);
	return res;
}

inline void preprocess(){
	fact[0] = inv_fact[0] = 1;
	for(int i = 1; i < N; i++) fact[i] = prod(fact[i - 1], i);
	for(int i = N - 1; i >= 1; i--) inv_fact[i] = power(fact[i], MOD - 2);
}

inline int choose(int n, int r){
	if(r < 0 || r > n) return 0;
	return prod(fact[n], prod(inv_fact[r], inv_fact[n - r]));
}

int n;
int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d", &n);
	preprocess();
	dp[0][0] = eq[0] = 1;
	for(int i = 1; i < n; i++){
		for(int j = 1; j <= i; j++){
			dp[i][j] = prod(dp[i - 1][j], j);
			dp[i][j] = add(dp[i][j], dp[i - 1][j - 1]);
			eq[i] = add(eq[i], dp[i][j]);
		}
	}
	int ans = 0;
	for(int i = 0; i < n; i++){
		ans = add(ans, prod(choose(n, i), eq[i]));
	}
	printf("%d\n", ans);
}
