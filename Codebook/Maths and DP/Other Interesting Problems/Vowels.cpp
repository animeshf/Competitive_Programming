#include "bits/stdc++.h"
using namespace std;

/*
    Vowels - Codeforces
    For each mask, we need to compute number of x such that mask & x is nonzero.
    This is equivalent to finding number of x such that mask & x is equal to zero.
    Number of x such that (mask & x == 0) = Number of x such that (inv(mask) & x == x)
    The problem has thus reduced to computing dp[mask] = number of x such that mask & x == x
    This can be done by SOS bitmask dp.
*/

const int LN = 24;
const int  N = 1 << 24;

int n;
int dp[N];

inline int inv(int x){
    int res = x;
    for(int i = 0; i < LN; i++){
        res ^= (1 << i);
    }
    return res;
}

int main(){
    freopen("ioi.in", "r", stdin);
    cin >> n;
    for(int i = 1; i <= n; i++){
        string s;
        cin >> s;
        int mask = 0;
        for(int j = 0; j < (int) s.size(); j++){
            mask |= (1 << (s[j] - 'a'));
        }
        ++dp[mask];
    }
    // The (i) loop should appear before the (mask) loop. Think why!
    for(int i = 0; i < LN; i++){
        for(int mask = 0; mask < N; mask++){
            if(mask & (1 << i))
                dp[mask] += (dp[mask ^ (1 << i)]);
        }
    }
    int res = 0;
    for(int i = 0; i < N; i++){
        // Number of valid words for mask i = n - dp[inv(i)].
        int valid = n - dp[inv(i)];
        res ^= (valid * valid);
    }
    cout << res << '\n';
}