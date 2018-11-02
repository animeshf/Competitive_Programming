// Compute number of ways to place knights on a n * m chessboard.
// Two knights cannot attack each other.
// n <= 4, m <= 10^9
// Solution: Optimize dynamic programming with matrix exponentiation
// Keep track of last two columns: compute states that can be adjacent to
// each other (so they must have a common middle column).
// Finally apply matrix exponentiation to compute number of ways.

#include "bits/stdc++.h"
using namespace std;

const int N = 4;

/*------ Matrix Exponentiation Template ------*/

const int ORD = 1 << (2 * N); // Order of Square Matrix
const int MOD = 1000000009; // Modulo

inline int prod(int x, int y){
	long long res = x * 1LL * y;
	if(res >= MOD) res %= MOD;
	return res;
}

inline int add(int x, int y){
	int res = x + y;
	if(res >= MOD) res -= MOD;
	return res;
}

struct matrix{
	int mat[ORD][ORD];
	matrix(){
		for(int i = 0; i < ORD; i++)
			for(int j = 0; j < ORD; j++)
				mat[i][j] = 0;
	}
	inline friend matrix operator * (matrix x, matrix y){
		matrix res;
		for(int i = 0; i < ORD; i++)
			for(int j = 0; j < ORD; j++)
				for(int k = 0; k < ORD; k++)
					res.mat[i][j] = add(res.mat[i][j], prod(x.mat[i][k], y.mat[k][j]));
		return res;
	}
};

matrix initMatrix;

inline matrix power(matrix cur, int p){
    matrix res = cur;
    p -= 1;
    while (p > 0) {
        if (p & 1) {
            res = res * cur;
            p -= 1;
        } else {
            cur = cur * cur;
            p >>= 1;
        }
    }
    return res;
}

/*------------ End of Template -------------*/

int testGrid[N][N];
int dx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
int dy[] = {-1, 1, -2, 2, 2, -2, 1, -1};


int ans2 = 0;

inline bool isValid(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j++) {
            if (testGrid[i][j]) {
                for (int k = 0; k < 8; k++) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    if (ni >= 0 && ni < n && nj >= 0 && nj < 3) {
                        if (testGrid[ni][nj]) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

inline void computeGraph(int n) {
    for (int en = 0; en < (1 << n); en++) {
        for (int mi = 0; mi < (1 << n); mi++) {
            memset(testGrid, 0, sizeof testGrid);
            for (int i = 0; i < n; i++) {
                testGrid[i][2] = (en & (1 << i));
            }
            for (int i = 0; i < n; i++) {
                testGrid[i][1] = (mi & (1 << i));
            }
            if (isValid(n)) {
                ans2 += 1;
            }
            for (int st = 0; st < (1 << n); st++) {
                memset(testGrid, 0, sizeof testGrid);
                for (int i = 0; i < n; i++) {
                    testGrid[i][2] = (en & (1 << i));
                }
                for (int i = 0; i < n; i++) {
                    testGrid[i][1] = (mi & (1 << i));
                }
                for (int i = 0; i < n; i++) {
                    testGrid[i][0] = (st & (1 << i));
                }
                if (isValid(n)) {
                    int x = (mi << n) | (en);
                    int y = (st << n) | (mi);
                    initMatrix.mat[x][y] = 1;
                }
            }
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    computeGraph(n);
    if (m == 1) {
        cout << (1 << n) << '\n';
    } else if (m == 2) {
        cout << ans2 << '\n';
    } else {
        matrix res = power(initMatrix, m - 2);
        int ans = 0;
        for (int i = 0; i < ORD; i++) {
            for (int j = 0; j < ORD; j++) {
                ans = add(ans, res.mat[i][j]);
            }
        }
        cout << ans << '\n';
    }
}
