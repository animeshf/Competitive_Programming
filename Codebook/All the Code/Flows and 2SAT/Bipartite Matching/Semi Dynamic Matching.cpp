#include "bits/stdc++.h"
using namespace std;

const int MAX_N = 56789;
const int MAX_S = 12;
const int MAX_V = 1234567;

int t, n, ans, timer;
int dice[MAX_N][MAX_S];
int match[MAX_N];
int vis[MAX_V];
vector < int > values[MAX_V];

void cleanup() {
    ans = 0;
    timer = 0;
    for (int i = 0; i < MAX_V; i++) {
        values[i].clear();
        vis[i] = 0;
    }
}

void init() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= 6; j++) {
            cin >> dice[i][j];
            values[dice[i][j]].push_back(i);
        }
    }
}

/**
    For visualization purposes, let's assume the left partite are the numbers
    and the right partite are the dice indices. We want to check if there exists
    a perfect matching, i.e. all the numbers are covered.

    Let's maintain an array match[] where match[x] denotes the number that dice x has
    been matched to. We already have values[] computed in the above init() function
    which tells us which dice have a particular number.

    We use the recursive bipartite matching algorithm to check for existence.
**/

bool dfs(int x) {
    // Can we augment x into our existing matching?
    vis[x] = timer;
    for (int dice_id: values[x]) {
        if (match[dice_id] == -1) {
            // We found a dice which has not been matched to anyone, yay!
            match[dice_id] = x;
            return true;
        }
    }
    // Ok, so there isn't any "free" dice.
    // We can fix a dice, recurse and see if we can get a solution.
    for (int dice_id: values[x]) {
        int num = match[dice_id];
        if (vis[num] != timer && dfs(num)) {
            match[dice_id] = x;
            return true;
        }
    }
    return false;
}

bool check(int l, int r) {
    if (r - l + 1 > n) {
        return false;
    }
    // We have some perfect matching (possibly empty).
    // We want to add the number (r) to it and see if it still holds.

    // Mark all numbers in this range as unvisited.
    ++timer;

    // We'll try to augment our matching by adding (r) to it.
    // This follows the recursive bipartite matching algorithm.
    return dfs(r);
}

void slide(int l) {
    // We cannot get any more straights with (l), so lets slide our window
    // Basically mark all dice associated with (l) as free.
    for (int dice_id: values[l]) {
        if (match[dice_id] == l) {
            match[dice_id] = -1;
        }
    }
}

void solve() {
    for (int i = 1; i <= n; i++) {
        match[i] = -1; // All dice are unmatched.
    }
    int l = 1, r = 1;
    while (r < MAX_V) {
        if (check(l, r)) {
            ans = max(ans, r - l + 1);
            r += 1;
        } else {
            slide(l);
            l += 1;
            r = max(l, r);
        }
    }
}

int main() {
    freopen("inp.in", "r", stdin);
    freopen("A_large.out", "w", stdout);
    ios :: sync_with_stdio(false);
    cin >> t;
    for (int qq = 1; qq <= t; qq++) {
        cleanup();
        init();
        solve();
        printf("Case #%d: %d\n", qq, ans);
    }
}