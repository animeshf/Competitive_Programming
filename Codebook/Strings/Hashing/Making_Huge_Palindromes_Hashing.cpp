/*
        LightOJ
	Add minimum number of characters to a string S to make it a palindrome.
	Answer = length(S) - length(Longest Palindromic Suffix(S))
*/

#include "bits/stdc++.h"
using namespace std;

const int MAX  = 1000005;
const int MOD1 = 1000000007;
const int MOD2 = 1000000009;
const int BASE = 137;

char str[MAX];
int t, n;
pair < int, int > h[MAX], rh[MAX], p[MAX];

inline int prod1(int x, int y){
	long long res = x * 1LL * y;
	if(res >= MOD1) res %= MOD1;
	return res;
}

inline int prod2(int x, int y){
	long long res = x * 1LL * y;
	if(res >= MOD2) res %= MOD2;
	return res;
}

inline int add1(int x, int y){
	int res = x + y;
	if(res < 0) res += MOD1;
	if(res >= MOD1) res -= MOD1;
	return res;
}

inline int add2(int x, int y){
	int res = x + y;
	if(res < 0) res += MOD2;
	if(res >= MOD2) res -= MOD2;
	return res;
}

// Build tables
inline void build(){
	p[0] = make_pair(1, 1);
	for(int i = 1; i < MAX; i++){
		p[i].first  = prod1(p[i - 1].first, BASE);
		p[i].second = prod2(p[i - 1].second, BASE);
	}
	h[0] = make_pair(0, 0);
	for(int i = 1; i <= n; i++){
		h[i].first  = add1(prod1(h[i - 1].first, BASE), str[i] - 'a' + 1);
		h[i].second = add2(prod2(h[i - 1].second, BASE), str[i] - 'a' + 1);
	}
	rh[n + 1] = make_pair(0, 0);
	for(int i = n; i >= 1; i--){
		rh[i].first  = add1(prod1(rh[i + 1].first, BASE), str[i] - 'a' + 1);
		rh[i].second = add2(prod2(rh[i + 1].second, BASE), str[i] - 'a' + 1);
	}
}

// Returns hash of the substring [l, r]
inline pair < int, int > getHash(int l, int r){
	pair < int, int > ans;
	ans.first  = add1(h[r].first, -(prod1(h[l - 1].first, p[r - l + 1].first)));
	ans.second = add2(h[r].second,-(prod2(h[l - 1].second, p[r - l + 1].second)));
	return ans;
}

// Returns hash of the substring [r, l]
inline pair < int, int > getReverseHash(int l, int r){
	pair < int, int > ans;
	ans.first  = add1(rh[l].first, -(prod1(rh[r + 1].first,  p[r - l + 1].first)));
	ans.second = add2(rh[l].second,-(prod2(rh[r + 1].second, p[r - l + 1].second)));
	return ans;
}

inline bool isPalindrome(int i, int j){
	return (getHash(i, j) == getReverseHash(i, j));
}

int main(){
	scanf("%d", &t);
	for(int qq = 1; qq <= t; qq++){
		scanf("%s", str + 1);
		n = strlen(str + 1);
		build();
		for(int i = 1; i <= n; i++){
			if(isPalindrome(i, n)){
				printf("Case %d: %d\n", qq, n + n - (n - i + 1));
				break;
			}
		}
	}
}
