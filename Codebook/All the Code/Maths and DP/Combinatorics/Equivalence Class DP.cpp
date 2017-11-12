// Codeforces - On the bench

#include "bits/stdc++.h"
using namespace std;

const int MOD = 1000000007;

inline int prod(int x, int y) {
    long long res = x * 1LL * y;
    if (res >= MOD) {
        res %= MOD;
    }
    return res;
}

inline int add(int x, int y) {
    int res = x + y;
    if (res < 0) {
        res += MOD;
    }
    if (res >= MOD) {
        res -= MOD;
    }
    return res;
}

const int N = 333;

int n;
int arr[N];
int c[N];
vector < int > values;

inline int normalize(int x) {
    int z = 0;
    for (int i = 2; i * i <= x; i++) {
        int t = 0;
        while (x % i == 0) {
            x /= i;
            t += 1;
        }
        if (t & 1) {
            if (z == 0) z = 1;
            z *= i;
        }
    }
    if (x != 1) {
        if (z == 0) z = 1;
        z *= x;
    }
    return z;
}

inline void compress() {
    sort(values.begin(), values.end());
    for (int i = 1; i <= n; i++) {
        arr[i] = lower_bound(values.begin(), values.end(), arr[i]) - values.begin() + 1;
    }
}

/**
    A more general problem:

    Given a list of G types of balls where:
    1) Balls of the same type are indistinguishable, and
    2) We have g_i balls of type i for all 1 <= i <= G.

    Compute the number of ways to arrange all balls in a line such that
    no two adjacent balls are of the same type (we define such a pair
    as a bad pair). The below code returns this answer modulo 1000000007.
**/

const int MAX = 333;

int fact[MAX];
int choose[MAX][MAX];
int dp[MAX][MAX];

inline void precompute() {
    fact[0] = 1;
    for (int i = 1; i < N; i++) {
        fact[i] = prod(fact[i - 1], i);
    }
    choose[0][0] = 1;
    for (int i = 1; i < N; i++) {
        choose[i][0] = choose[i][i] = 1;
        for (int j = 1; j < i; j++) {
            choose[i][j] = add(choose[i - 1][j], choose[i - 1][j - 1]);
        }
    }
}

inline int ncr(int n, int r) {
    if (r < 0 || r > n) {
        return 0;
    }
    return choose[n][r];
}

inline int solve(vector < int > group_sizes) {
    precompute();
    int g = (int) group_sizes.size();
    int total = 0;
    for (int s: group_sizes) {
        total += s;
    }
    assert(total < MAX);
    group_sizes.insert(group_sizes.begin(), 0); // Just to make stuff 1-indexed.

    // dp[i][x] = Number of permutations using first (i) groups of balls
    // that have exactly (x) bad pairs of neighbours.
    memset(dp, 0, sizeof dp);

    // Base Case with just one group.
    dp[1][group_sizes[1] - 1] = 1;

    int placed_till_now = 0, gaps_till_now = 0;
    for (int i = 1; i < g; i++) { // Augment dp[i + 1][...] states using dp[i][...] states
        placed_till_now += group_sizes[i];
        gaps_till_now = placed_till_now - 1; // number of "holes" we can place stuff at, excluding the boundaries.
        for (int x = 0; x <= gaps_till_now; x++) { // Iterating over bad pairs which must be <= gaps_till now
            for (int p = 1; p <= min(group_sizes[i + 1], gaps_till_now + 2); p++) { // Number of parts we break the (i + 1)th group into.
                for (int b = 0; b <= min(x + 1, p); b++) { // Number of bad pairs we eliminate.
                    // We had x bad pairs, we eliminated b of them, but we added some more because of the blocks
                    // leaving us with exactly z bad pairs.
                    int z = x - b + (group_sizes[i + 1] - p);
                    // Let's compute number of ways to achieve z bad pairs. First, we divide
                    // the (i + 1)th group we are looking at into p nonempty parts.
                    // This formula considers ordering within the parts as well, so no need to multiply by p!
                    int ways_to_divide_group = ncr(group_sizes[i + 1] - 1, p - 1);

                    // From all permutations that we are currently looking at, we will:
                    int num_permutations = dp[i][x]; // 1) pick a permutation
                    int num_bad_slots = ncr(x, b); // 2) pick b bad slots to eliminate from that permutation.
                    int num_good_slots = ncr(gaps_till_now + 2 - x, p - b); // 3) pick good slots to place remaining parts.

                    int ways_to_place = prod(num_permutations, prod(num_bad_slots, num_good_slots)); // Product of above three

                    // Finally, we merge the two quanitities we computed to get total number of ways
                    int total_ways = prod(ways_to_divide_group, ways_to_place);

                    // Augment dp state
                    dp[i + 1][z] = add(dp[i + 1][z], total_ways);
                }
            }
        }
    }
    // Use all balls and have 0 bad pairs.
    return dp[g][0];
}

// End of Generic Template

int main() {
    ios :: sync_with_stdio(false);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
        arr[i] = normalize(arr[i]);
        values.push_back(arr[i]);
    }
    compress();
    for (int i = 1; i <= n; i++) {
        c[arr[i]] += 1;
    }
    vector < int > class_sizes;
    for (int i = 1; i <= n; i++) {
        if (c[i] > 0) {
            class_sizes.push_back(c[i]);
        }
    }
    int ans = solve(class_sizes);
    // In this problem, objects of the same type are distinguishable
    // so we multiply answer by factorials of group sizes.
    for (int i = 1; i <= n; i++) {
        ans = prod(ans, fact[c[i]]);
    }
    cout << ans << endl;
}