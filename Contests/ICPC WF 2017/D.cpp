#include "bits/stdc++.h"
using namespace std;

const int N = 500000 + 50;

int m, n;
int p[N], d[N];
int q[N], e[N];
long long res;
vector<pair<int, int>> producers;
vector<pair<int, int>> sellers;
vector<pair<int, int>> norm_p;
vector<pair<int, int>> norm_s;

inline void solve(int l, int r, int qs, int qe) {
    if (r < l) {
        return;
    }
    int m = (l + r) / 2;
    int best = -1;
    long long ans = 0;

    int valid_l = INT_MAX;
    int bl = qs, br = qe;
    while (bl <= br) {
        int mv = (bl + br) >> 1;
        if (norm_s[mv].first >= norm_p[m].first) {
            valid_l = mv;
            br = mv - 1;
        } else {
            bl = mv + 1;
        }
    }

    for (int i = valid_l; i <= qe; i++) {
        if (norm_s[i].second > norm_p[m].second) {
            long long profit = (norm_s[i].first - norm_p[m].first) * 1LL * (norm_s[i].second - norm_p[m].second);
            if (profit > ans) {
                ans = profit;
                best = i;
            }
        } else {
            break;
        }
    }

    if (best != -1) {
        solve(l, m - 1, qs, best);
    } else {
        solve(l, m - 1, qs, qe);
    }
    solve(m + 1, r, max(qs, best), qe);
    res = max(res, ans);
}

int main() {
    scanf("%d %d", &m, &n);
    for (int i = 1; i <= m; i++) {
        scanf("%d %d", &p[i], &d[i]);
        producers.push_back(make_pair(d[i], p[i]));
    }
    for (int i = 1; i <= n; i++) {
        scanf("%d %d", &q[i], &e[i]);
        sellers.push_back(make_pair(e[i], q[i]));
    }
    sort(producers.begin(), producers.end());
    for (pair<int, int> item: producers) {
        if (!norm_p.size() || norm_p.back().second > item.second) {
            norm_p.push_back(item);
        }
    }
    sort(sellers.begin(), sellers.end());
    reverse(sellers.begin(), sellers.end());
    for (pair<int, int> item: sellers) {
        if (!norm_s.size() || norm_s.back().second < item.second) {
            norm_s.push_back(item);
        }
    }
    m = (int) norm_p.size();
    n = (int) norm_s.size();
    reverse(norm_s.begin(), norm_s.end());
    solve(0, m - 1, 0, n - 1);
    printf("%lld\n", res);
}