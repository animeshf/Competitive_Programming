#include "bits/stdc++.h"
using namespace std;

const int N = 100;
const int X = 10000;

/*
	Let's find grundy(x), which is the grundy number for this game on a pile
	with x stones. Note that we can xor grundy(x_i) for each x_i in the test
	case and that would be sufficient to know the answer.

	Now, let's focus on computing grundy(x) for some pile size (x).
	We know that if x <= 2, grundy(x) = 0, as (x) is a losing position.

	Now, let's look at some x > 2, we can split x into (i, j) such that
	(i != j) and (i + j = x).

	Every such pair {i, j} can be treated as two disjoint subgames, one with
	pile size (i) and the other with pile size (j) : Thus the grundy of this
	state would be xor of these two subgames.

	So, For all valid pairs {i, j} add grundy(i) ^ grundy(j) to a set.
	Thus you will have the set of all states reachable from the current state (x).
	Find the mex in that to find grundy(x)
*/

int t, n, arr[N + 5], dp[X + 5];

inline int solve(int x){
	if(x <= 2) return 0;
	if(dp[x] != -1) return dp[x];
	vector < int > values;
	for(int i = 1; i <= x >> 1; i++){
		if(i != (x - i))
			values.push_back(solve(i) ^ solve(x - i));
	}
	sort(values.begin(), values.end());
	values.resize(unique(values.begin(), values.end()) - values.begin());
	int mex = (int) values.size();
	for(int i = 0; i < (int) values.size(); i++){
		if(values[i] != i){
			mex = i;
			break;
		}
	}
	return dp[x] = mex;
}

int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d", &t);
	memset(dp, -1, sizeof dp);
	for(int i = 1; i <= X; i++) dp[i] = solve(i);
	for(int qq = 1; qq <= t; qq++){
		scanf("%d", &n);
		int ans = 0;
		for(int i = 1; i <= n; i++){
			scanf("%d", arr + i);
			ans ^= dp[arr[i]];
		}
		printf("Case %d: ", qq);
		if(ans) puts("Alice");
		else puts("Bob");
	}
}