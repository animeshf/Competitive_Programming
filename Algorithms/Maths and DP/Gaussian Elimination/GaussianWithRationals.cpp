/*
    Probability Paradox

*/

#include "bits/stdc++.h"
using namespace std;

inline long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

struct rational {
    long long p, q;
    void red() {
        if (q < 0) {
            p *= -1;
            q *= -1;
        }
        long long t = gcd((p >= 0 ? p : -p), q);
        p /= t;
        q /= t;
    }
    rational() {} rational(long long p_): p(p_), q(1)
               {}rational(long long p_, long long q_): p(p_), q(q_) {
                red();
            }
    bool operator==(const rational& rhs) const {
        return p == rhs.p && q == rhs.q;
    }
    bool operator!=(const rational& rhs) const {
        return p != rhs.p || q != rhs.q;
    }
    bool operator<(const rational& rhs) const {
        return p * rhs.q < rhs.p * q;
    }
    bool operator>(const rational& rhs) const {
        return p * rhs.q > rhs.p * q;
    }
    const rational operator+(const rational& rhs) const {
        return rational(p * rhs.q + q * rhs.p, q * rhs.q);
    }
    const rational operator-(const rational& rhs) const {
        return rational(p * rhs.q -q * rhs.p, q * rhs.q);
    }
    const rational operator*(const rational& rhs) const {
        return rational(p * rhs.p, q * rhs.q);
    }
    const rational operator/(const rational& rhs) const {
        return rational(p * rhs.q, q * rhs.p);
    }
};

inline rational FABS(rational r) {
    if (r < rational(0, 1)) {
        return r * -1;
    } else {
        return r;
    }
}

const double EPS = 1e-10;

typedef vector < int > VI;
typedef rational T ;
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
*/

T GaussJordan(VVT &a, VVT &b) {
  const int n = a.size();
  const int m = b[0].size();
  VI irow(n), icol(n), ipiv(n);
  T det = T(1, 1);

  for (int i = 0; i < n; i++) {
    int pj = -1, pk = -1;
    for (int j = 0; j < n; j++) if (!ipiv[j])
      for (int k = 0; k < n; k++) if (!ipiv[k])
    if (pj == -1 || FABS(a[j][k]) > FABS(a[pj][pk])) { pj = j; pk = k; }
    ipiv[pk]++;
    swap(a[pj], a[pk]);
    swap(b[pj], b[pk]);
    if (pj != pk) det = det * -1;
    irow[i] = pj;
    icol[i] = pk;
    T c = T(a[pk][pk].q, a[pk][pk].p);
    det = det * a[pk][pk];
    a[pk][pk] = T(1, 1);
    for (int p = 0; p < n; p++) a[pk][p] = a[pk][p] * c;
    for (int p = 0; p < m; p++) b[pk][p] = b[pk][p] * c;
    for (int p = 0; p < n; p++) if (p != pk) {
      c = a[p][pk];
      a[p][pk] = 0;
      for (int q = 0; q < n; q++) a[p][q] = a[p][q] - (a[pk][q] * c);
      for (int q = 0; q < m; q++) b[p][q] = b[p][q] - (b[pk][q] * c);
    }
  }

  for (int p = n - 1; p >= 0; p--) if (irow[p] != icol[p]) {
    for (int k = 0; k < n; k++) swap(a[k][irow[p]], a[k][icol[p]]);
  }

  return det;
}

map < string, int > id;
map < int, string > inv;

inline int transition(string res) {
    for (int i = 0; i < (int) res.size(); i++) {
        string cur = res.substr(i);
        if (id.count(cur)) {
            return id[cur];
        }
    }
    return 0;
}

inline void solve(string a, string b) {
    id.clear(); inv.clear();
    int n = (int) a.size();
    int index = 0;
    for (int i = 0; i <= n; i++) {
        string curPrefix = a.substr(0, i);
        if (id.count(curPrefix) == 0) {
            id[curPrefix] = index++;
            inv[index - 1] = curPrefix;
        }
    }
    for (int i = 0; i <= n; i++) {
        string curPrefix = b.substr(0, i);
        if (id.count(curPrefix) == 0) {
            id[curPrefix] = index++;
            inv[index - 1] = curPrefix;
        }
    }
    int m = (int)(id.size());
    VVT A(m, VT(m));
    VVT B(m, VT(1));
    for (int i = 0; i < m; i++) {
        B[i][0] = rational(0, 1);
    }
    B[id[a]][0] = rational(1, 1);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            A[i][j] = rational(0, 1);
        }
    }
    for (int i = 0; i < m; i++) {
        A[i][i] = rational(1, 1);
        if ((int) inv[i].size() == n) {
            continue;
        }
        A[i][transition(inv[i] + "H")] = A[i][transition(inv[i] + "H")] + rational(-1, 2);
        A[i][transition(inv[i] + "T")] = A[i][transition(inv[i] + "T")] + rational(-1, 2);
    }
    T det = GaussJordan(A, B);
    cout << B[0][0].p << '/' << B[0][0].q << '\n';
}


int main() {
    freopen("inp.in", "r", stdin);
    while (true) {
        string a; string b;
        cin >> a;
        if (a == "$") {
            return 0;
        }
        cin >> b;
        solve(a, b);
    }
}



