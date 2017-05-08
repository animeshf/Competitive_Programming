#include "bits/stdc++.h"
using namespace std;

inline string get(string x, string y) {
    if (y.empty()) {
        return x;
    } else if (x.empty()) {
        return y;
    } else {
        return (x < y) ? (x) : (y);
    }
}

inline string solve(int n, int &r, int &p, int &s, char winner) {
    string emp = "";
    if (n == 1) {
        if (winner == 'S') {
            if (s && p) {
                s--;
                p--;
                return "PS";
            }
            return emp;
        } else if (winner == 'P') {
            if (p && r) {
                p--;
                r--;
                return "PR";
            }
            return emp;
        } else {
            if (r && s) {
                r--;
                s--;
                return "RS";
            }
            return emp;
        }
    }
    char win_left = winner;
    char win_right;
    if (winner == 'S') {
        win_right = 'P';
    } else if (winner == 'P') {
        win_right = 'R';
    } else {
        win_right = 'S';
    }
    string lef = solve(n - 1, r, p, s, win_left);
    string rig = solve(n - 1, r, p, s, win_right);
    if (lef.empty() || rig.empty()) {
        return emp;
    }
    return get(lef + rig, rig + lef);
}

int main() {
    freopen ("A.in", "r", stdin);
    freopen ("A.out", "w", stdout);
    int t; cin >> t;
    for (int qq = 1; qq <= t; qq++) {
        int n, r, p, s; cin >> n >> r >> p >> s;
        int nn, rr, pp, ss;
        string ans = "";
        // Winner = Rock
        nn = n, rr  = r, pp = p, ss = s;
        ans = get(ans, solve(nn, rr, pp, ss, 'R'));
        // Winner = Scissors
        nn = n, rr  = r, pp = p, ss = s;
        ans = get(ans, solve(nn, rr, pp, ss, 'S'));
        // Winner = Paper
        nn = n, rr  = r, pp = p, ss = s;
        ans = get(ans, solve(nn, rr, pp, ss, 'P'));
        cout << "Case #" << qq << ": ";
        if (ans.empty()) {
            cout << "IMPOSSIBLE\n";
        } else {
            cout << ans << "\n";
        }
    }
}