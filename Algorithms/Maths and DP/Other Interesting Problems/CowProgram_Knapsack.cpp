/*
	Knapsack with restrictions on coins
	Given a set of inequalitites of the form :
	Coin (x) should be used strictly more number of times than Coin (y)

	Find number of ways to sum up to (t) 
	Note : The DAG formed is a chain 

	The idea is to create a new set of denominations by fixing certain coins,
	to ensure that we always maintain the inequalities no matter what we choose.
*/

#include "bits/stdc++.h"
using namespace std;

const int N = 305;
const int T = 100005;
const int MOD = 1000000007;

int n, q, t;
int arr[N], dp[N][T], nxt[N];
bool is_root[N], vis[N];
vector < int > val;

inline int add(int x, int y){
	int res = x + y;
	if(res >= MOD) res -= MOD;
	return res;
}

int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d %d %d", &n, &q, &t);
	for(int i = 1; i <= n; i++){
		scanf("%d", arr + i);
		nxt[i] = -1;
		is_root[i] = true;
		vis[i] = false;
	}
	for(int i = 1; i <= q; i++){
		int b, c;
		scanf("%d %d", &b, &c);
		is_root[c] = false;
		nxt[b] = c;
	}
	long long total = 0;
	for(int i = 1; i <= n; i++){
		if(is_root[i]){
			int node = i, sum = 0;
			do{
				vis[node] = true;
				sum += arr[node];
				val.push_back(sum);
				if(nxt[node] != -1) total += sum;
				node = nxt[node];
			}while(node != -1);
		}
	}
	bool ok = (total <= t);
	for(int i = 1; i <= n; i++) ok &= vis[i];
	if(!ok){
		printf("0\n");
		return 0;
	}
	t -= total;
	int len = (int) val.size();
	dp[0][0] = 1;
	for(int i = 1; i <= len; i++){
		for(int j = 0; j <= t; j++){
			dp[i][j] = dp[i - 1][j];
			if(j >= val[i - 1]){
				dp[i][j] = add(dp[i][j], dp[i][j - val[i - 1]]);
			}
		}
	}
	printf("%d\n", dp[len][t]); 
}