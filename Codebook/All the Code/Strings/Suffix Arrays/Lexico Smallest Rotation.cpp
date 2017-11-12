// ACM ICPC - Hidden Password

#include <bits/stdc++.h>
using namespace std;

const int N  = 2e5 + 5;
const int LN = 20;

char str[N];

/*----------- Suffix Array Template ------------*/
// sa[i] = index of i'th smallest suffix in str[]
// "ana" --> {a, ana, na} -> sa[0] = 2, sa[1] = 0, sa[2] = 1

int pos[LN][N], sa[N], tmp[N];
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
inline void build_suffix_array(){
	len = strlen(str);
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

/*----------- End of Template ------------*/


/* Returns the lexicographically smallest x length substring of str[]
   In case of multiple options, it chooses the one which has the lowest start_index */

inline void compute(int x){
	int st_idx = 0, idx = 0;
	for(int i = 0; i < len; i++){
		if(len - sa[i] >= x){
			st_idx = sa[i];
			idx = i;
			break;
		}
	}
	for(int i = idx + 1; i < len; i++){
		if(lcp(st_idx, sa[i]) >= x) 
			st_idx = min(st_idx, sa[i]);
	}
	printf("%d\n", st_idx);
}

int main(){
	int t;
	scanf("%d", &t);
	while(t--){
		scanf("%d %s", &len, str);
		for(int i = len; i < len * 2; i++) str[i] = str[i - len];
		build_suffix_array();
		compute(len >> 1);
	}
}