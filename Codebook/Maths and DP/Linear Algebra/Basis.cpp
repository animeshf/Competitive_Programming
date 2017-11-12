// Reduced row echelon form via Gauss-Jordan elimination
// with partial pivoting.  This can be used for computing
// the rank of a matrix.
//
// Running time: O(n^3)
//
// INPUT:    a[][] = an nxm matrix
//
// OUTPUT:   rref[][] = an nxm matrix (stored in a[][])
//           returns rank of a[][]

//           To get basis, we store the indices of the columns in the
//           original matrix.

#include <bits/stdc++.h>
using namespace std;
const double EPSILON = 1e-10;

typedef double T;
typedef vector<T> VT;
typedef vector<VT> VVT;

bool is_linear_combination(const VVT& A, const vector<int> indx, VT v) {
  for (int i = 0; i < v.size(); i++) {
    if (!v[i]) continue;
    bool good = false;
    for (int k : indx) {
      if (A[i][k]) {
        good = true;
        break;
      }
    }
    if (!good) return false;
  }
  return true;
}

int get_basis(VVT &a, vector<int>& basis_indx) {
  int n = a.size();
  int m = a[0].size();
  int r = 0;
  for (int c = 0; c < m && r < n; c++) {
    int j = r;
    for (int i = r + 1; i < n; i++)
      if (fabs(a[i][c]) > fabs(a[j][c])) j = i;
    if (fabs(a[j][c]) < EPSILON) continue;
    swap(a[j], a[r]);

    T s = 1.0 / a[r][c];
    for (int j = 0; j < m; j++) a[r][j] *= s;
    for (int i = 0; i < n; i++) if (i != r) {
      T t = a[i][c];
      for (int j = 0; j < m; j++) {
        a[i][j] -= t * a[r][j];
      }
    }
    basis_indx.push_back(c);
    r++;
  }
  return r;
}

int main() {

  VVT AA = A;
  vector<int> basis_indx;
  int rank = get_basis(A, basis_indx);
  int free = N - rank;

  if (is_linear_combination(AA, basis_indx, v)) {
    // vector v is a linear combination of columns of matrix AA
  }

  return 0;
}

