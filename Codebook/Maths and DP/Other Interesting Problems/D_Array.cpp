#include "bits/stdc++.h"
using namespace std;

const int N = 1005;
const int MOD = 1000000007;

int n, q;
int arr[N], d[N];
int dp[N][N], dp_sum[N][N];

int main(){

	cin >> n;
	arr[0] = 0;
	for(int i = 1; i <= n; i++){
		cin >> arr[i];
	}
	cin >> q;
	
	/*
		Subtask 2 :-
		dp[i][j] : Number of ways to fill [i + 1...n] such that after the (i)'th index is 
		processed, we have (j) elements remaining in the stack. (including arr[i])
	*/

	for(int i = 2; i <= n + 1; i++){
		dp[n][i]  = 1;
		dp_sum[n][i] = dp_sum[n][i - 1] + dp[n][i];
	}

	for(int i = n - 1; i >= 1; i--){
		for(int j = 2; j <= i + 1; j++){
			dp[i][j] = dp_sum[i + 1][j + 1];
			dp_sum[i][j] = dp_sum[i][j - 1] + dp[i][j];
			if(dp_sum[i][j] >= MOD) dp_sum[i][j] -= MOD;
		}
	}

	int rank = 0, total = dp_sum[1][2];
	
	stack < int > st;
	for(int i = 1; i <= n; i++){
		// (i) is the first index of difference
		st.push(arr[i - 1]);
		int stack_size = st.size();
		while(st.top() > arr[i]){
			st.pop();
			++d[i];
		}
		int cur = dp_sum[i][max(0, stack_size - d[i])];
		rank = (rank + cur) % MOD;
	}

	for(int i = 1; i <= n; i++){
		cout << d[i];
		if(i < n) cout << " ";
		else cout << "\n";
	}
	
	if(q >= 1){
		cout << ((total - rank + MOD) % MOD) << "\n";
	}

	/*
		Subtask 3 :-
		dp[i][j] = number of ways to have arrays of length (i) such that after processing
		(i), the stack has j elements (inc i), and that the stack has >= 3 elements at every 
		index from (2...i)
	*/

	memset(dp, 0, sizeof dp);
	memset(dp_sum, 0, sizeof dp_sum);

	dp[1][2] = dp_sum[1][2] = 1;

	for(int i = 2; i <= n; i++){
		for(int j = 3; j <= i + 1; j++){
			dp[i][j] = (dp_sum[i - 1][i] - dp_sum[i - 1][j - 2] + MOD) % MOD;
			dp_sum[i][j] = dp_sum[i][j - 1] + dp[i][j];
			if(dp_sum[i][j] >= MOD) dp_sum[i][j] -= MOD;
		}
	}

	total = 0;
	for(int i = 2; i <= n; i++){
		int x = (i - 1);
		int y = (n - i + 1);
		int cur = (dp_sum[x][x + 1] * 1LL * dp_sum[y][y + 1]) % MOD;
		total = (total + cur) % MOD;
	}

	if(q >= 2){
		cout << total << "\n";
	}
}