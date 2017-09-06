// X Samara - Problem K
#include "bits/stdc++.h"
using namespace std;

const int N = 400005;
const int INF = 1000 * 1000 * 1000 + 1;

struct seg {
    int st, en, id, pid;
    long long value;
    long long duration;
    seg(int _st = 0, int _en = 0, int _id = -1, int _pid = -1, long long _value = 0, long long _duration = 0) {
        st = _st;
        en = _en;
        id = _id;
        pid = _pid;
        value = _value;
        duration = _duration;
    }
    inline friend bool operator < (seg foo, seg bar) {
        if (foo.value != bar.value) {
            return (foo.value < bar.value);
        }
        return (foo.duration > bar.duration);
    }
};

int n;
int a[N], b[N], c[N];
seg bit[N], arr[N], dp[N];
vector < int > compress;

inline bool compare(seg foo, seg bar) {
    return foo.st < bar.st;
}

inline int small(int x) {
    return lower_bound(compress.begin(), compress.end(), x) - compress.begin() + 1;
}

inline void update(int x, seg val) {
    for (int i = x; i < N; i += i & -i) {
        bit[i] = max(bit[i], val);
    }
}

inline seg query(int x) {
    seg res = seg();
    for (int i = x; i > 0; i -= i & -i) {
        res = max(res, bit[i]);
    }
    return res;
}

int main() {
    ios :: sync_with_stdio(false);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i] >> b[i] >> c[i];
        arr[i] = seg(a[i], b[i], i, -1, c[i], b[i] - a[i]);
        compress.push_back(a[i]);
        compress.push_back(b[i]);
    }
    seg dummy = seg(INF, INF, n + 1, -1, 0, 0);
    compress.push_back(INF);
    arr[n + 1] = dummy;
    sort (compress.begin(), compress.end());
    compress.resize(unique(compress.begin(), compress.end()) - compress.begin());
    sort (arr + 1, arr + n + 1, compare);
    for (int i = 1; i < N; i++) {
        bit[i] = seg();
    }
    // Base Case
    dp[1] = seg(0, 0, 1, -1, arr[1].value, arr[1].duration);
    update(small(arr[1].en), dp[1]);
    // Transition
    for (int i = 2; i <= n + 1; i++) {
        seg best = query(small(arr[i].st));
        dp[i] = seg(best.st, best.en, i, best.id, best.value + arr[i].value, best.duration + arr[i].duration);
        update(small(arr[i].en), dp[i]);
    }
    vector < int > sol;
    int x = n + 1;
    do {
        int take = dp[x].id;
        if (take != n + 1) {
            sol.push_back(arr[take].id);
        }
        int move = dp[x].pid;
        x = move;
    } while (x != -1);
    reverse(sol.begin(), sol.end());
    cout << (int) sol.size() << ' ' << dp[n + 1].value << ' ' << dp[n + 1].duration << endl;
    for (int cid: sol) {
        cout << cid << ' ';
    }
    cout << endl;
}