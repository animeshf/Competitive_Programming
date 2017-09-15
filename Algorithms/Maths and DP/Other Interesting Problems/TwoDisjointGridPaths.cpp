// Find two disjoint paths from (0, 0) to (n - 1, m - 1) which maximizes the A[i][j] values visited in the path.
// All the intermediate nodes in the two paths should be disjoint.
// Legal Moves are Down and Right

// O(N^3)


#include "bits/stdc++.h"
using namespace std;

const int N = 205;

int n, m;
int arr[N][N];
int dp[N][N][N];

int dx[] = {0, 1};
int dy[] = {1, 0};

inline bool isValid(int x, int y) {
    return (0 <= x && x < n && 0 <= y && y < m);
}

inline int solve(int a1, int b1, int a2) {
    int b2 = (a1 + b1) - (a2);
    if (dp[a1][b1][a2] != -1) {
        return dp[a1][b1][a2];
    }
    if ((a1 == a2) && (b1 == b2)) {
        if ((a1 + b1 > 0) && (a1 + b1 < n + m - 2)) {
            return 0;
        }
    }
    if (a1 == n - 1 && b1 == m - 1) {
        return dp[a1][b1][a2] = arr[a1][b1];
    }
    int score = arr[a1][b1];
    if (a1 != a2 || b1 != b2) {
        score += arr[a2][b2];
    }
    int result = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int na1 = a1 + dx[i];
            int nb1 = b1 + dy[i];
            int na2 = a2 + dx[j];
            int nb2 = b2 + dy[j];
            if (isValid(na1, nb1) && isValid(na2, nb2)) {
                result = max(result, solve(na1, nb1, na2));
            }
        }
    }
    return dp[a1][b1][a2] = result + score;
}

int main() {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> arr[i][j];
        }
    }
    memset(dp, -1, sizeof dp);
    cout << solve(0, 0, 0) << endl;
}