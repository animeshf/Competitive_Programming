// SWERC 2016 - Problem I

#include "bits/stdc++.h"
using namespace std;
#define FOR(i,m,n) for(int i = (m); i < (n); i++)
#define ROF(i,m,n) for(int i = (n)-1; i >= (m); i--)
typedef long long LL;
typedef unsigned long long ULL;
typedef vector<int> VI;
typedef vector<LL> VLL;
#define SZ(x) ((int)(x).size())
#define MP make_pair
typedef pair<int,int> PII;
#define ll long long
typedef pair<LL,LL> PLL;
#define A first
#define B second
using namespace std;

inline long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

long long power(long long x, long long y, int mod=13)  {
    if (y == 1) {
        return x;
    }
    if (y == 0) {
        return 1;
    }
    long long res = power(x, y >> 1, mod);
    res = res * res;
    if (y & 1) {
        res *= x;
    }
    if (res >= mod) {
        res %= mod;
    }
    return res;
}

long long inv(long long x)  {
    return power(x, 11);
}

const double EPS = 1e-10;

typedef vector < int > VI;
typedef int T ;
typedef vector < T > VT;
typedef vector < VT > VVT;

/*
    aX = b
    b is the column vector representing the solutions
    a is the coefficients of the linear equation
    X is just the column vector representing x1, x2, x3..... xN
    GaussJordan returns determinant and modifies b passed which stores the values of
    x1, x2...xN
    T is generics which is nice so just use whatever data type you want

    NOTE: a MUST BE a square Matrix

    You can change the mod value in this function to anything you want, but it should be prime.
    Appropriately, you should change the mod values in the power() and invert() functions above as well.
*/

void GaussJordan(VVT &a, VVT &b) {
  const int n = a.size();
  const int m = b[0].size();
  VI irow(n), icol(n), ipiv(n);

  for (int i = 0; i < n; i++) {
    int pj = -1, pk = -1;
    for (int j = 0; j < n; j++) if (!ipiv[j])
      for (int k = 0; k < n; k++) if (!ipiv[k])
    if (pj == -1 || abs(a[j][k]) > abs(a[pj][pk])) { pj = j; pk = k; }
    ipiv[pk]++;
    swap(a[pj], a[pk]);
    swap(b[pj], b[pk]);
    irow[i] = pj;
    icol[i] = pk;
    T c = inv(a[pk][pk]);
    a[pk][pk] = 1;
    for (int p = 0; p < n; p++) {
        a[pk][p] = a[pk][p] * c;
        a[pk][p] %= 13;
    }
    for (int p = 0; p < m; p++) {
        b[pk][p] = b[pk][p] * c;
        b[pk][p] %= 13;
    }
    for (int p = 0; p < n; p++) if (p != pk) {
      c = a[p][pk];
      a[p][pk] = 0;
      for (int q = 0; q < n; q++) {
        a[p][q] = a[p][q] - (a[pk][q] * c) + (c * 13);
        a[p][q] %= 13;
      }
      for (int q = 0; q < m; q++) {
        b[p][q] = b[p][q] - (b[pk][q] * c) + (c * 13);
        b[p][q] %= 13;
      }
    }
  }

}

const int N = 505;
const int V = 205;

int n, a, r, t;
int edge_to_id[N][N];
PII id_to_edge[N];
int timer;
int adj[N][N];

int main() {
    scanf("%d %d %d %d", &n, &a, &r, &t);
    VVT A(t, VT(t));
    VVT B(t, VT(1));

    for (int i = 0; i < t; i++) {
        for (int j = 0; j < t; j++) {
            A[i][j] = 0;
        }
    }

    for (int j = 0; j < t; j++) {
        B[j][0] = 0;
    }

    memset(edge_to_id, -1, sizeof edge_to_id);

    for (int i = 0; i < t; i++) {
        int d, p; scanf("%d %d", &d, &p);
        B[i][0] = d;
        vector < int > cities;
        for (int j = 0; j < p; j++) {
            int city; scanf("%d", &city);
            cities.push_back(city);
        }
        for (int u = 0; u < p - 1; u++) {
            int v = u + 1;
            int p = cities[u];
            int q = cities[v];
            if (p > q) swap(p, q);
            if (edge_to_id[p][q] == -1) {
                edge_to_id[p][q] = timer++;
                id_to_edge[timer - 1] = MP(p, q);
            }
            A[i][edge_to_id[p][q]]++;
            A[i][edge_to_id[p][q]] %= 13;
        }
    }

    GaussJordan(A, B);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            adj[i][j] = 1e9;
        }
        adj[i][i] = 0;
    }
    for (int j = 0; j < t; j++) {
        int w = B[j][0];
        PII edge = id_to_edge[j];
        adj[edge.first][edge.second] = w;
        adj[edge.second][edge.first] = w;
    }
    for (int k = 1; k <= n; k ++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][j]);
            }
        }
    }
    printf("%d\n", adj[a][r]);
    return 0;
}