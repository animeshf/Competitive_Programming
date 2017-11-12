#include "bits/stdc++.h"
using namespace std;

/*
	In this problem, a subgame can be considered as a substring of the original string.
	We start with str[1...N], and we can remove any important substring from this and give 
	two disjoint substrings to the opponent. The opponent then has the choice to choose 
	any one of these two disjoint subgames, hence the value of such a state is simply 
	the xor of these two subgames. 

	So, the problem reduces to computing the reachable grundy values from the initial
	state and then finding the mex. The mex will give us the grundy value of the original
	string, which would help us determine the answer. 
	If the mex is nonzero, we can win. Else we will surely lose.
	
	Suppose that this game was being played on many strings simultaneously. In that case,
	we would just take the xor of each of the mex values we found and check if it's nonzero.

	It is important to identify the sub-games to solve problems on Grundy Numbers!
*/


const int N = 55;
int t, n, len;
int dp[N][N], exist[N][N];
char str[N], arr[N][N];

inline int solve(int l, int r){
	if(l > r) return 0;
	if(dp[l][r] != -1) return dp[l][r];
	set < int > values;
	for(int i = l; i <= r; i++)
		for(int j = i; j <= r; j++)
			if(exist[i][j])
				values.insert(solve(l, i - 1) ^ solve(j + 1, r));
	int mex = 0;
	while(values.count(mex)) mex++;
	return dp[l][r] = mex;
} 

int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d", &t);
	while(t--){
		scanf("%s", str + 1);
		len = strlen(str + 1);
		scanf("%d", &n);
		for(int i = 1; i <= n; i++){
			scanf("%s", arr[i] + 1);
		}
		memset(exist, 0, sizeof exist);
		memset(dp, -1, sizeof dp);
		for(int i = 1; i <= len; i++){
			for(int j = i; j <= len; j++){
				for(int k = 1; k <= n; k++){
					if(strlen(arr[k] + 1) != j - i + 1) continue;
					int ptr = 1;
					while(ptr <= (j - i + 1) && arr[k][ptr] == str[i + ptr - 1])
						ptr++;
					if(ptr > (j - i + 1)) exist[i][j] = 1;
				}
			}
		}
		if(solve(1, len)) puts("Teddy");
		else puts("Tracy");	
	}
}