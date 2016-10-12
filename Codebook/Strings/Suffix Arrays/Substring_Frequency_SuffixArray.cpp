/*
	Finding Frequency of String B in String A using Suffix Arrays.
	Procedure : Make a String S = A + "$" + B. Build the Suffix Array
	on S. Let idx = length(A) + 1. (The position where B starts)
	Compute number of indices (x) such that lcp(x, idx) == length(B)
	
	Time Complexity : O(N log N)
	Space Complexity : O(N log N)
*/

#include "bits/stdc++.h"
using namespace std;

const int N = 1e6 + 6; 
const int LN = 20;
char a[N], b[N], str[N * 2];
int t;

/*----------- Suffix Array Template ------------*/
// sa[i] = index of i'th smallest suffix in str[]
// "ana" --> {a, ana, na} -> sa[0] = 2, sa[1] = 0, sa[2] = 1

int pos[LN][N * 2], sa[N * 2], tmp[N * 2];
int gap, len, level;

// Comparison function -> O(1)
inline bool suffix_cmp(int i, int j){
	if(pos[level][i] != pos[level][j]){
		return (pos[level][i] < pos[level][j]);
	}
	i += gap, j += gap;
	if(i < len && j < len){
		return (pos[level][i] < pos[level][j]);
	}
	return (i > j);
}

// Builds suffix array in len log^2 len
inline void build_suffix_array(int length){
	len = length;
	level = 0;
	for(int i = 0; i < len; i++){
		pos[level][i] = str[i];
		sa[i] = i;
	}
	for(gap = 1; ; gap *= 2){
		sort(sa, sa + len, suffix_cmp);
		for(int i = 1; i < len; i++){
			tmp[i] = tmp[i - 1] + suffix_cmp(sa[i - 1], sa[i]); 
		}
		level = level + 1;
		for(int i = 0; i < len; i++){
			pos[level][sa[i]] = tmp[i];
		}
		if(tmp[len - 1] == len - 1) break;
	}
}

// Returns LCP of str[x..len-1] and str[y..len-1] in O(log len)
inline int lcp(int x, int y){
	int res = 0;
	for(int i = level; i >= 0; i--){
		if(x < len && y < len && pos[i][x] == pos[i][y]){
			res += (1 << i);
			 x  += (1 << i);
			 y  += (1 << i);
		}
	}
	return res;
}

/*-------------- End of Template --------------*/

inline int compute(int idx){
	int ans = 0;
	for(int i = 0; i < idx; i++)
		ans += (lcp(i, idx) == len - idx);
	return ans;
}

int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d", &t);
	for(int qq = 1; qq <= t; qq++){
		scanf("%s %s", a, b);
		int len1 = strlen(a), len2 = strlen(b);
		for(int i = 0; i < len1; i++) str[i] = a[i];
		str[len1] = '$';
		for(int i = 0; i < len2; i++) str[i + len1 + 1] = b[i];
		build_suffix_array(len1 + len2 + 1);
		int ans = compute(len1 + 1);
		printf("Case %d: %d\n", qq, ans);
	}
}
