/*
	Special Pairs - Hackerearth
	Find Number of unordered-pairs (i, j) such that A[i] & A[j] = 0
	Let a number be X. We know that for inverse of X, X will be a valid
	choice. Let X* be inverse of X, considering only the first LN bits.

	For all subsets of X*, X is a valid choice. We can consider all X**
	such that X** <= X*, and X will be a valid choice for each such X**.
	Hence, the problem reduces to finding # of X** in arr[]. This can be
	done with SOS-Subset DP in 2^(LN) * LN = N log N
*/

#include "bits/stdc++.h"	
using namespace std;

const int LN = 20;
const int N  = (1 << LN);

int t, n;
int arr[N], freq[N], ans[N];
int dp[N][LN + 1];

int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d", &t);
	while(t--){
		memset(dp, 0, sizeof dp);
		memset(freq, 0, sizeof freq);
		memset(ans, 0, sizeof ans);
		scanf("%d", &n);
		for(int i = 1; i <= n; i++){
			scanf("%d", arr + i);
			freq[arr[i]]++;
		}
		for(int mask = 0; mask < N; mask++){
			dp[mask][0] = freq[mask];
			for(int i = 0; i < LN; i++){
				if(mask & (1 << i))
					dp[mask][i + 1] = dp[mask][i] + dp[mask ^ (1 << i)][i];
				else
					dp[mask][i + 1] = dp[mask][i];
			}
			ans[mask] = dp[mask][LN];
		}
		long long res = 0;
		for(int i = 1; i <= n; i++){
			res += ans[(1 << LN) - 1 - arr[i]]; // (2^(LN) - 1) ^ arr[i]
		}
		printf("%lld\n", res);
	}
}