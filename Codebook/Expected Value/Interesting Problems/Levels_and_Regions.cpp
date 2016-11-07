// Codeforces VK Cup 2016 - Divide and Conquer DP Optimization
#include <bits/stdc++.h>
using namespace std;

const int N = 200005;
const int K = 55;

int n, k, t[N];
double t_sum[N], t_inv[N], pre[N], dp[K][N];

/*
dp[j][i]   = min(dp[j - 1][k] + cost[k + 1][i]), where (1 <= k < i).
cost[i][j] = (t[i]) / (t[i]) + 
             (t[i] + t[i + 1]) / (t[i + 1]) + 
             (t[i] + t[i + 1] + t[i + 2]) / (t[i + 2]) + ....
             (t[i] + t[i + 1].... + t[j]) / (t[j])
           = (pre[j] - pre[i - 1]) - t_sum[i - 1] * (t_inv[j] - t_inv[i - 1])
*/

inline double cost(int i, int j){
	return (pre[j] - pre[i - 1]) - t_sum[i - 1] * (t_inv[j] - t_inv[i - 1]);
}

inline void compute(int j, int l, int r, int qs, int qe){
	int i = (l + r) >> 1, best_idx = -1;
	dp[j][i] = 1e18;
	for(int k = qs; k <= min(qe, i - 1); k++){
		if(dp[j - 1][k] + cost(k + 1, i) < dp[j][i]){
			dp[j][i] = dp[j - 1][k] + cost(k + 1, i);
			best_idx = k;
		}
	}
	if(i > l) compute(j, l, i - 1, qs, best_idx);
	if(i < r) compute(j, i + 1, r, best_idx, qe);
}

int main(){
	scanf("%d %d", &n, &k);
	for(int i = 1; i <= n; i++){
		scanf("%d", t + i);
		t_sum[i] = t_sum[i - 1] + t[i];
		t_inv[i] = t_inv[i - 1] + (1.0 / t[i]);
		pre[i]   = pre[i - 1] + (t_sum[i] / t[i]);
	}
	for(int i = 1; i <= n; i++) dp[1][i] = dp[1][i - 1] + (pre[i] - pre[i - 1]);
	for(int i = 2; i <= k; i++) compute(i, 1, n, 1, n);
	printf("%.10f\n", dp[k][n]);
}
