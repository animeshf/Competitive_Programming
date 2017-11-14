// Exciting Finish - SER 2017
// Enlarging Enthusiasm - PacNW 2017
// This is the editorial solution.

#include "bits/stdc++.h"
using namespace std;

const int N = 12;
const int X = 700;

int n, x;
int p[N];
int dp[1 << N][N][X + 1];

inline int points_needed(int i, int j) {
    // Minimum number of points needed for person i to beat person j.
    return max(0, p[j] + 1 - p[i]);
}

int main() {
    cin >> n >> x;
    for (int i = 0; i < n; i++) {
        cin >> p[i];
    }
    sort(p, p + n);
    // Initialize the base cases
    for (int i = 0; i < n - 1; i++) {
        int cost = points_needed(i, n - 1);
        int cost_add = n * cost;
        if (cost_add <= x) {
            dp[1 << i][i][cost_add] = 1;
        }
    }
    // Propagate the DP values
    for (int mask = 1; mask < (1 << n); mask++) {
        int number_of_people = __builtin_popcount(mask);
        for (int last = 0; last < n; last++) {
            if (!((mask >> last) & 1)) {
                continue;
            }
            for (int add = 0; add < n; add++) {
                if ((mask >> add) & 1) {
                    continue;
                }
                int cost = points_needed(add, last);
                int cost_add = (n - number_of_people) * cost;
                for (int current_cost = 0; current_cost + cost_add <= x; current_cost++) {
                    dp[mask | (1 << add)][add][current_cost + cost_add] += dp[mask][last][current_cost];
                }
            }
        }
    }
    // Add results
    int result = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= x; j++) {
            result += dp[(1 << n) - 1][i][j];
        }
    }
    cout << result << endl;
}