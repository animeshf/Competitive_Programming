/*
	Codeforces - Lucky Common Subsequence
	Prints Longest Common Subsequence of 's' and 't' that doesn't contain the substring 'pattern'
*/


#include <bits/stdc++.h>
using namespace std;

const int N = 105;
const int L = 26;

string s, t, pattern;
int lps[N], f[N][L], dp[N][N][N];


/* 
	f[i][j] = If I have matched the first "i" characters of "pattern", and I append
			  character "j", what will be the new match length (lps) of the resulting string

	pattern => "ababa"
	f[3][b] = 4 --> This means I had an "aba" and I appended  a "b", now the new match length is "abab"
	f[3][a] = 1 --> This means I had an "aba" and I appended an "a", now the new match length is "a"

*/

inline void precompute() {
	for(int i = 1; i < pattern.size(); i++){
		int j = lps[i - 1];
		while (j > 0 and pattern[j] != pattern[i]){
			j = lps[j - 1];
		}
		j += pattern[i] == pattern[j];
		lps[i] = j;
	}
	for(int j = 0; j < 26; j++){
		f[0][j] = (pattern[0] - 'A') == j ? 1 : 0;
	}
	for(int i = 1; i < pattern.size(); i++){
		for(int j = 0; j < 26; j++){
			f[i][j] = (pattern[i] - 'A') == j ? i + 1 : f[lps[i - 1]][j];
		}
	}
	for (int j = 0; j < 26; j++) {
		f[pattern.size()][j] = f[lps[(int)pattern.size() - 1]][j];
	}
}

/*
	dp[i][j][match] = i and j denote positions in the string S1 and S2
	match denotes the length of the pattern which is the same as the suffix
	of our currently built string. It must never be equal to the entire pattern
	else we are violating our rule.
*/

inline int solve(int i, int j, int match){
	if(match == pattern.size()) return -(N * N);
	if(i == s.size() || j == t.size()) return 0;
	if(dp[i][j][match] != -1) return dp[i][j][match];
	int res = max(solve(i + 1, j, match), solve(i, j + 1, match));
	if(s[i] == t[j]){
		res = max(res, 1 + solve(i + 1, j + 1, f[match][s[i] - 'A']));
	}
	return dp[i][j][match] = res;
}

int main(){
	cin >> s >> t >> pattern;
	precompute();
	memset(dp, -1, sizeof dp);
	int lcs = solve(0, 0, 0);
	if(!lcs){
		cout << lcs << '\n';
		return 0;
	}
	int i = 0, j = 0, k = 0;
	while(lcs){
		int x = dp[i + 1][j][k];
		int y = dp[i][j + 1][k];
		int z = (s[i] == t[j]) ? (1 + dp[i + 1][j + 1][f[k][s[i] - 'A']]) : (0);
		if(x == dp[i][j][k]){
			i++;
		}
		else if(y == dp[i][j][k]){
			j++;
		}
		else{
			cout << s[i];
			k = f[k][s[i] - 'A'];
			lcs--, i++, j++;
		}
	}
}