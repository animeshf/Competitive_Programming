/*
	Jzzhu and Numbers - Codeforces
	Number of subsets of an array of N elements whose bitwise-and is zero.
	N <= 1e6, arr[i] <= 1e6

	dp[mask][i] = Number of (x) in arr[] such that :-
	
	1) (arr[x] & mask) == (mask) 
	2) arr[x] and mask might differ only the first i bits. 

	The bits are numbered from right to left with the rightmost bit numbered (1).
	
	f[mask] = Number of (x) in arr[] such that (mask & arr[x] == mask)
	f[mask] = dp[mask][LN]

	f[0] = number of (x) in arr[] such that (arr[x] & 0 == 0) == n
	Answer = 2^n - 1
	but some of these subsets might have bitwise and with 1 bit set
	Answer -= 2^(f(x)) - 1 for all x such that popcount(x) == 1
	but some of these might have 2 bits set and we might have subtracted them twice...

	Thus, inclusion exclusion based on parity of popcount(x)!
*/

#include "bits/stdc++.h"	
using namespace std;

const int LN = 20;
const int MOD = 1000000007;

int n;
int arr[1 << LN];
int dp[1 << LN][LN + 1];
int f[1 << LN], p[1 << LN];

int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d", &n);
	for(int i = 1; i <= n; i++){
		scanf("%d", arr + i);
		++dp[arr[i]][0];
	}
	for(int mask = (1 << LN) - 1; mask >= 0; mask--){
		for(int i = 0; i < LN; i++){
			if(mask & (1 << i))
				dp[mask][i + 1] = dp[mask][i];
			else
				dp[mask][i + 1] = dp[mask][i] + dp[mask | (1 << i)][i];
		}
		f[mask] = dp[mask][LN];
	}
	p[0] = 1;
	for(int i = 1; i < (1 << LN); i++){
		p[i] = (2LL * p[i - 1]) % MOD;
	}
	for(int i = 0; i < (1 << LN); i++){
		p[i] = (p[i] - 1 + MOD) % MOD;
	}
	int ans = 0;
	for(int i = 0; i < (1 << LN); i++){
		if(__builtin_popcount(i) & 1)
			ans = (ans - p[f[i]] + MOD) % MOD;
		else
			ans = (ans + p[f[i]]) % MOD;
	}
	printf("%d\n", ans);
}