#include "bits/stdc++.h"
using namespace std;

const int N = 1000 + 50;

map < int, int > memo; 
int t, n, arr[N];

/*
	This is the classical Nim_Game with the subtle change that we can remove atmost
	half the stones from a pile in our chance. Like classic nim, a subgame in this 
	is a single pile. We will use an observation to solve the problem. Writing down
	the grundy numbers for the first 12 values, we see this
	grundy[0] = 0
	grundy[1] = 0
	grundy[2] = 1
	grundy[3] = 0
	grundy[4] = 2
	grundy[5] = 1
	grundy[6] = 3
	grundy[7] = 0
	grundy[8] = 4
	grundy[9] = 2
	grundy[10] = 5

	Thus, for even x, grundy[x] = x / 2
		  for odd  x, grundy[x] = grundy[floor(x / 2)]
	
	Thus you can maintain a map and recursively compute grundy(x). 
	You will create N log N entries each time. hence time complexity and
	space complexity is N log N per test case.
*/


inline int grundy(int x){
	if(memo.find(x) != memo.end()) return memo[x];
	if(x % 2 == 0){
		return memo[x] = (x >> 1);
	}
	else{
		return memo[x] = grundy(x >> 1);
	}
}

int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d", &t);
	for(int qq = 1; qq <= t; qq++){
		scanf("%d", &n);
		int ans = 0;
		for(int i = 1; i <= n; i++){
			scanf("%d", arr + i);
			ans ^= grundy(arr[i]);
		}
		printf("Case %d: ", qq);
		if(ans) puts("Alice");
		else puts("Bob");
	}
}