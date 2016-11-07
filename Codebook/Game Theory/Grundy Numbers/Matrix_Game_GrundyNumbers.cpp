#include "bits/stdc++.h"
using namespace std;

const int N = 55;
int t, n, m, arr[N][N];
 
inline void scan(int &x){
	register int c = getchar_unlocked();
	x = 0;
	for(; (c < 48 || c > 57); c = getchar_unlocked());
	for(; (c > 47 && c < 58); c = getchar_unlocked()){
		x = (x << 1) + (x << 3) + c - 48;
	}
}

/*
	Each row can be considered as a sub-game in this problem.
	A row can be visualized as a pile with the leftmost element at the top of the pile 
	and the rightmost at the bottom of the pile. A move is removing some non-zero amount
	from the top of the pile. We need to compute the grundy number for each row. 
	Then we can xor all the rows' grundy numbers to get the solution.

	If there is only one number in the row, then the grundy value of the row is that element
	itself. Suppose there are 2 numbers (7, 6) in the row. We will process numbers from right
	to left (from the bottom of the pile to the top of the pile), and compute grundy numbers.
	Now, by definition from 7, we can remove any nonzero amount and come to a state.
	
	For (1, 6) : Grundy value equals 0 (You can reach the state (6) which has grundy 6, thus
	mex is 0)
	For (2, 6) : Grundy value equals 1 (You can reach states (1, 6) and (6) with grundies 0
	and 6 respectively, hence mex is 1)
	....
	For(6, 6) :  Grundy value equals 5 (You can reach states (1, 6)...(5, 6) and (6) with grundies
	0, 1, 2, 4 and 6 respectively, hence mex is 6)
	For(7, 6) :  Grundy value equals 7 (You can reach (1, 6).. (6, 6) and (6) and cover grundies
	0 through 6, Hence mex is 7)

	Suppose the row has M elements. We want grundy[1...M]. grundy[i...M] can be determined by 
	grundy[i + 1...M], by checking which one is greater/smaller.

	Note the method becomes simple because you just need to look at the next immediate state and
	not all the reachable values from a particular state to solve a problem.
*/

int main(){
	freopen("ioi.in", "r", stdin);
	scan(t);
	for(int qq = 1; qq <= t; qq++){
		scan(n), scan(m);
		for(int i = 1; i <= n; i++)
			for(int j = 1; j <= m; j++)
				scan(arr[i][j]);
		int ans = 0;
		for(int i = 1; i <= n; i++){
			int cur = arr[i][m];
			for(int j = m - 1; j >= 1; j--){
				if(arr[i][j] > cur)
					cur = arr[i][j];
				else 
					cur = arr[i][j] - 1;
			}
			ans ^= cur;
		}
		if(ans) puts("FIRST");
		else puts("SECOND");
	}
}