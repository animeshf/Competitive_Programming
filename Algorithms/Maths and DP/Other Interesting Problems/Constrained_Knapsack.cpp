// Constrained Knapsack Problem:
// This is exactly like traditional 0/1 Knapsack, except you can now take upto C[i] occurrences
// of the (i)th item instead of just 0/1 occurrence.
// This solves it in O(N * V * log(N)): Note that it can be done in O(N * V) with some more effort.


#include "bits/stdc++.h"
using namespace std;

const int N = 205;
const int V = 100005;
const int LN = 12;

int n, v;
int vol[N], wt[N], cnt[N];
long long dp[V];
long long tr[V];

// Suppose we have 10 objects.
// We can replace 10 objects with a 1-pack, a 2-pack, a 4-pack, and a 3-pack.
// In general, we can decompose n objects into ceil(log(n)) objects and solve
// standard knapsack on it.

// How we decompose it uses the following idea:
// Suppose we want to cover all numbers in the range [0, x]:
// Let y be the highest (2^k - 1) such that y <= x.
// It is well known we can cover everything from [0, y] by using [1, 2, 2^1, ..., 2^{k - 1}]
// Now, just append (x - y) to this list above, and you can make everything from [0, x] {You can make
// everything from [0, y] without this last element, so adding it will help you make everything from [0, x].
// Additionally, it is guaranteed that you can never make anything > x, because the sum of the elements = x :)


int main() {
    cin >> n >> v;
    vector < pair < int, int > > items;
    for (int i = 1; i <= n; i++) {
        cin >> vol[i] >> wt[i] >> cnt[i];
        int power = 0;
        while ((1 << power) <= cnt[i]) {
            items.push_back({(1 << power) * vol[i], (1 << power) * wt[i]});
            cnt[i] -= (1 << power);
            power += 1;
        }
        if (cnt[i]) {
            items.push_back({cnt[i] * vol[i], cnt[i] * wt[i]});
        }
    }
    int m = (int) items.size();
    dp[0] = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 1; j <= v; j++) {
            if (items[i].first <= j) {
                tr[j] = dp[j - items[i].first] + items[i].second;
            }
        }
        for (int j = 1; j <= V; j++) {
            dp[j] = max(dp[j], tr[j]);
        }
    }
    cout << dp[v] << endl;
}