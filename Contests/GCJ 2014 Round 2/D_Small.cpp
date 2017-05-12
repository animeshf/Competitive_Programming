#include "bits/stdc++.h"
using namespace std;

const int INF = INT_MAX;

const int N = 10;

int m, n;
int maxVal, maxCnt;
int assign[N];
string str[N];

inline int solve(vector < string > &a) {
    unordered_set < string > prefixes;
    prefixes.clear();
    int sz = (int) a.size();
    int score = 1; // null string.
    for (int i = 0; i < sz; i++) {
        string cur = a[i];
        string pref = "";
        for (int j = 0; j < (int) cur.size(); j++) {
            pref += cur[j];
            if (prefixes.count(pref)) {
                continue;
            } else {
                prefixes.insert(pref);
                score++;
            }
        }
    }
    return score;
}

inline void solve() {
    int score = 0;
    for (int i = 1; i <= n; i++) {
        vector < string > values;
        values.clear();
        for (int j = 1; j <= m; j++) {
            if (assign[j] == i)
                values.push_back(str[j]);
        }
        if (values.empty()) {
            return;
        }
        score += solve(values);
    }
    if (score > maxVal) {
        maxVal = score;
        maxCnt = 1;
    } else if (score == maxVal) {
        maxCnt += 1;
    }
}

inline void bruteForce(int id) {
    if (id == m + 1) {
        solve();
        return;
    }
    for (int i = 1; i <= n; i++) {
        assign[id] = i;
        bruteForce(id + 1);
    }
}

int main() {
    freopen ("inp.in", "r", stdin);
    freopen ("D.out", "w", stdout);
    int tc; cin >> tc;
    for (int qq = 1; qq <= tc; qq++) {
        cout << "Case #" << qq << ": ";
        cin >> m >> n;
        cin.ignore();
        for (int i = 1; i <= m; i++) {
            cin >> str[i];
        }
        maxVal = 0;
        maxCnt = 0;
        bruteForce(1);
        cout << maxVal << ' ' << maxCnt << "\n";
    }
}