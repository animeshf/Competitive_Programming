/*
	Compatible Numbers - Codeforces
	For each index i, find index j such that arr[i] & arr[j] == 0.
	Print -1 if such a j does not exist.

	N <= 1e6, 1 <= arr[i] <= 2**22

	Let f[i] be inverse of arr[i]. (Take only 22 bits!)
	Let ans[i] be the answer for number i.

	Notice that ans[f[i]] = arr[i].
	Moreover, you can switch off any subset of the on bits in f[i] and the answer
	would still be arr[i] for that resulting number. Naive method would take O(3 ^ N)
	but you can do it in O(2 ^ N * N) by iterating from MAX_VAL to 0, and switching off
	only one on bit at a time instead of all subsets.
*/

#include "bits/stdc++.h"
using namespace std;

const int LN = 22;
const int  N = 1 << LN;

int n;
int arr[N], dp[N];

inline int inv(int x){
	int res = x;
	for(int i = 0; i < LN; i++){
		res ^= (1 << i);
	}
	return res;
}

int main(){
	freopen("ioi.in", "r", stdin);
	fill(dp, dp + N, -1);
	scanf("%d", &n);	
	for(int i = 1; i <= n; i++){
		scanf("%d", arr + i);	
		dp[inv(arr[i])] = arr[i]; 
	}
	for(int i = N - 1; i >= 0; i--){
		if(dp[i] == -1) continue;
		for(int j = 0; j < LN; j++){
			if(i & (1 << j)){
				dp[i ^ (1 << j)] = dp[i];
			}
		}
	}
	for(int i = 1; i <= n; i++){
		printf("%d", dp[arr[i]]);
		if(i < n) printf(" ");
	}
}