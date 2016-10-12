#include "bits/stdc++.h"
using namespace std;

const int N = 1000 + 50; 

/*
	Game of Hyper Knights :-
	This can be modelled as a game theory problem. 
	The game is impartial because both players can work on any knight
	and the game will end because sum of (x + y) is constantly decreasing
	with each move of the knight. Each knight can be treated as a separate 
	sub-game. To find grundy(x, y), we find grundy(nx, ny) where (nx, ny)
	is reachable from (x, y). grundy(x, y) will then simply be the mex.
*/

int t, n, x, y;
int dx[] = {1, -1, -1, -2, -2, -3};
int dy[] = {-2, -3, -2, -1, 1, -1};
int dp[N][N];

inline int grundy(int x, int y){
	if(x < 0 || y < 0) return N;
	if(dp[x][y] != -1) return dp[x][y];
	bool is_present[7];
	memset(is_present, false, sizeof is_present);
	for(int i = 0; i < 6; i++){
		int nx = x + dx[i], ny = y + dy[i];
		int num = grundy(nx, ny);
		if(num != N) is_present[num] = true;
	}
	int mex = 0;
	while(is_present[mex]) ++mex;
	return dp[x][y] = mex;
}


int main(){
	freopen("ioi.in", "r", stdin);
	memset(dp, -1, sizeof dp);
	scanf("%d", &t);
	for(int qq = 1; qq <= t; qq++){
		scanf("%d", &n);
		int ans = 0;
		for(int i = 1; i <= n; i++){
			scanf("%d %d", &x, &y);
			ans ^= grundy(x, y);
		}
		printf("Case %d: ", qq);
		if(ans) puts("Alice");
		else puts("Bob");
	}
}