// Snackdown 2015 onsite finals
#include "bits/stdc++.h"
using namespace std;

const int N  = 75;
const int LN = 10;
const double EPS = 1e-7;

int t, n, m;
double p;
long long choose[N][N];
double dp[LN][N];

inline void preprocess(){
	choose[0][0] = 1;
	for(int i = 1; i < N; i++){
		for(int j = 0; j <= i; j++){
			if(j == 0 || j == i) choose[i][j] = 1;
			else choose[i][j] = (choose[i - 1][j] + choose[i - 1][j - 1]);
		}
	}
}

inline double solve(int level, int lefty){
	if(lefty == 0) return 0.0;
	if(lefty == (1 << level)) return 1.0;
	if(level == 1 and lefty == 1) return p;
	if(dp[level][lefty] != -1.0) return dp[level][lefty];
	int sub = 1 << (level - 1);
	double res = 0.0, all = 0.0;
	for(int i = 0; i <= min(sub, lefty); i++){
		all += (choose[lefty][i] * 1.0 * choose[(1 << level) - lefty][sub - i]);
	}
	for(int i = 0; i <= min(sub, lefty); i++){
		if(lefty - i > sub) continue;
		double pl = solve(level - 1, i);
		double pr = solve(level - 1, lefty - i);
		double win = (pl * pr) + ((1 - pl) * pr * p) + ((1 - pr) * pl * p);
		win *= (choose[lefty][i] * 1.0 * choose[(1 << level) - lefty][sub - i]);
		if(all != 0.0) win /= all;
		else win = 0.0;
		res += win;
	}
	return dp[level][lefty] = res;
}

int main(){
	freopen("ioi.in", "r", stdin);
	preprocess();
	scanf("%d", &t);
	while(t--){
		scanf("%d %d %lf", &n, &m, &p);
		int total = n + m;
		int rounds = (int) (log2(n + m) + EPS);
		for(int i = 0; i <= rounds; i++)
			for(int j = 0; j <= m; j++)
				dp[i][j] = -1.0;
		printf("%.15f\n", solve(rounds, m));
	}
}
