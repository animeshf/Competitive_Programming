/*

	Hackerearth - Benny and Two Strings
	Maximise occruences of pattern in text by modifying text following some constraints.
	KMP + DP problem

*/

#include <bits/stdc++.h>
using namespace std;

const int N = 205;
const int K = 505;

string text, pattern;
int n, m, k;
int f[N][26], dp[N][N][K], lps[N];

inline int cost(int a, int b) {
    if (a > b) {
        swap(a, b);
    }
    int o1 = b - a;
    int o2 = a + 26 - b;
    return min(o1, o2);
}

/*
	
	dp[u][l][k] = The best I can do at position (u), current match length (l), cost remaining (k)
	l = x implies that in the currently built string, the last x characters are the same as the
	first x characters of "pattern".
	
*/

inline int solve(int u, int l, int k) {
    if (k < 0) { 
        return -1e9;
    }
    if (u == text.size()) {
        return l == (int)pattern.size();
    }
    if (dp[u][l][k] != -1) {
        return dp[u][l][k];
    }
    int ans = -1e9;
    for (int i = 0; i < 26; ++i) {
        ans = max(ans, (l == (int)pattern.size()) + solve(u + 1, f[l][i], k - cost(text[u] - 'a', i)));
    }
    return dp[u][l][k] = ans;
}

/* 
	f[i][j] = If I have matched the first "i" characters of "pattern", and I append
	          character "j", what will be the new match length (lps) of the resulting string

	pattern => "ababa"
	f[3][b] = 4 --> This means I had an "aba" and I appended  a "b", now the new match length is "abab"
	f[3][a] = 1 --> This means I had an "aba" and I appended an "a", now the new match length is "a"

*/

inline void precompute() {
    for (int i = 1; i < pattern.size(); ++i) {
        int j = lps[i - 1];
        while (j > 0 and pattern[j] != pattern[i]) {
            j = lps[j - 1];
        }
        j += pattern[i] == pattern[j];
        lps[i] = j;
    }
    for (int j = 0; j < 26; ++j) {
        f[0][j] = (pattern[0] - 'a') == j ? 1 : 0;
    }
    for (int i = 1; i < pattern.size(); ++i) {
        for (int j = 0; j < 26; ++j) {
            f[i][j] = (pattern[i] - 'a') == j ? i + 1 : f[lps[i - 1]][j];
        }
    }
    for (int j = 0; j < 26; ++j) {
        f[pattern.size()][j] = f[lps[(int)pattern.size() - 1]][j];
    }
}

int main() {
    cin >> n >> m >> k;
    cin >> pattern >> text;
    precompute();
    memset(dp, -1, sizeof dp);
    cout << solve(0, 0, k) << '\n';
}
