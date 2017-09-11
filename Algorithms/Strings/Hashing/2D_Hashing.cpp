#include "bits/stdc++.h"
using namespace std;

const int W = 2005;
const int MOD1 = 1000000007;
const int MOD2 = 1000000009;
const int BASE = 17;

int r, c;
int R, C;

char pat[W][W];
char arr[W][W];

pair < int, int > row[W][W];
pair < int, int > col[2][W];

pair < int, int > h[W * 10], p[W * 10];

// ------ Hashing Template -------

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

inline void build(string str){
    h[0] = {0, 0};
    int n = (int) str.size();
    for(int i = 1; i <= n; i++){
        h[i].first  = add1(prod1(h[i - 1].first, BASE), str[i - 1] - '0' + 1);
        h[i].second = add2(prod2(h[i - 1].second, BASE), str[i - 1] - '0' + 1);
    }
}

pair < int, int > getHash(int l, int r){
    pair < int, int > ans = {0, 0};
    ans.first  = add1(h[r].first,  -(prod1(h[l - 1].first, p[r - l + 1].first)));
    ans.second = add2(h[r].second, -(prod2(h[l - 1].second, p[r - l + 1].second)));
    return ans;
}

// ------ End of Hashing Template -------


// ------ KMP Template -------

const int MAX_LEN = 1e5 + 5;
int lps[MAX_LEN];

// lps[] table is 1 based, strings are 0 based.
inline void compute_table(vector < pair < int, int > > pattern) {
    lps[0] = -1, lps[1] = 0;
    int pref = 0;
    for (int i = 2; i <= pattern.size(); i++) {
        while (pref != -1 && pattern[i - 1] != pattern[pref]) {
            pref = lps[pref];
        }
        pref++;
        lps[i] = pref;
    }
}

// Function returns frequency of 'pattern' in 'text'
inline int kmp(vector < pair < int, int > > text, vector < pair < int, int > > pattern){
    compute_table(pattern);
    int pref = 0, count = 0;
    for (int i = 0; i < text.size(); i++) {
        while (pref != -1 && text[i] != pattern[pref]) {
            pref = lps[pref];
        }
        pref++;
        if (pref == pattern.size()) {
            pref = lps[pref];
            count++;
        }
    }
    return count;
}

// ---- End of KMP Template ----

int main(){
    ios :: sync_with_stdio(false);
    freopen ("inp.in", "r", stdin);

    cin >> r >> c >> R >> C;
    for (int i = 1; i <= r; i++) {
        for (int j = 1; j <= c; j++) {
            cin >> pat[i][j];
        }
    }
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++) {
            cin >> arr[i][j];
        }
    }

    // Hash the rows
    p[0] = {1, 1};
    for(int i = 1; i < W * 10; i++){
        p[i].first  = prod1(p[i - 1].first, BASE);
        p[i].second = prod2(p[i - 1].second, BASE);
    }

    for (int i = 1; i <= R; i++) {
        string cur = "";
        for (int j = 1; j <= C; j++)
            cur += arr[i][j];
        build(cur);
        for (int j = 1; j <= C - c + 1; j++) {
            row[i][j] = getHash(j, j + c - 1);
        }
    }

    // Do KMP on columns
    vector < pair < int, int > > pattern;
    long long result = 0;

    for (int i = 1; i <= r; i++) {
        string cur = "";
        for (int j = 1; j <= c; j++)
            cur += pat[i][j];
        build(cur);
        pattern.push_back(getHash(1, c));
    }
    compute_table(pattern);

    for (int j = 1; j <= C - c + 1; j++) {
        vector < pair < int, int > > text;
        for (int i = 1; i <= R; i++) {
            text.push_back(row[i][j]);
        }
        result += kmp(text, pattern);
    }

    cout << result << endl;
}
