/*
  Problem: SEERC 2018: Problem K: Points and Rectangles

  This is a highly efficient implementation of a Compressed 2D BIT, supporting:
  1) Point update, 2D Range query
  2) 2D Range update, Point query
  3) Point update, Point query
  It can handle coordinates upto 10^9 (and also 10^18 if you just change things
  to long long instead of int).


  Limitations: This method works only when the updates+queries can be processed
  in an offline manner. We must be allowed to read all the updates and queries
  in order to do some preprocessing on them to make it space efficient.
  In other words, the answer to a query should not be demanded before reading
  the next line of input.

  Time Complexity: O(Q log^2(Q)) with no attached constants.
  Memory Complexity: O(Q log Q)
  Here, Q denotes the number of queries.

  Usage:
  1) Note that the Fenwick trees are 1-indexed.
  2) MAXQ denotes the maximum number of distinct points that can be in one row
     of the fenwick tree.
  3) The code performs coordinate compression on the input coordinates, so this
     must be allowed.
*/
#include "bits/stdc++.h"
using namespace std;

const int MAXQ = 200005;

int q;
int x[MAXQ], y[MAXQ];
int X_LIM, Y_LIM;
int x1[MAXQ], yy[MAXQ], x2[MAXQ], y2[MAXQ];
int type[MAXQ];
vector<int> fake_bit[2][MAXQ];
vector<int> bit[2][MAXQ];

inline void update(int id, int x, int y, int val) {
  for (int i = x; i < X_LIM; i += i & -i) {
    int pos = lower_bound(fake_bit[id][i].begin(), fake_bit[id][i].end(), y) - fake_bit[id][i].begin() + 1;
    for (int j = pos; j < bit[id][i].size(); j += j & -j) {
      bit[id][i][j] += val;
    }
  }
}

inline int query(int id, int x, int y) {
  if (x <= 0 || y <= 0) return 0;
  int res = 0;
  for (int i = x; i > 0; i -= i & -i) {
    int pos = lower_bound(fake_bit[id][i].begin(), fake_bit[id][i].end(), y) - fake_bit[id][i].begin() + 1;
    for (int j = pos; j > 0; j -= j & -j) {
      res += bit[id][i][j];
    }
  }
  return res;
}

inline void fake_query(int id, int x, int y) {
  for (int i = x; i > 0; i -= i & -i) {
    fake_bit[id][i].push_back(y);
  }
}

inline void fake_update(int id, int x, int y, int val) {
  for (int i = x; i < X_LIM; i += i & -i) {
    fake_bit[id][i].push_back(y);
  }
}

int main() {
  // Read queries and perform coordinate compression.
  scanf("%d", &q);
  vector<int> all_x, all_y;
  for (int i = 1; i <= q; i++) {
    scanf("%d", &type[i]);
    if (type[i] == 1) {
      scanf("%d %d", &x[i], &y[i]);
      all_x.push_back(x[i]);
      all_y.push_back(y[i]);
    } else {
      scanf("%d %d %d %d", &x1[i], &yy[i], &x2[i], &y2[i]);
      all_x.push_back(x1[i]);
      all_y.push_back(yy[i]);
      all_x.push_back(x2[i]);
      all_y.push_back(y2[i]);
    }
  }
  sort(all_x.begin(), all_x.end());
  all_x.resize(unique(all_x.begin(), all_x.end()) - all_x.begin());
  X_LIM = all_x.size() + 2;
  sort(all_y.begin(), all_y.end());
  all_y.resize(unique(all_y.begin(), all_y.end()) - all_y.begin());
  Y_LIM = all_y.size() + 2;
  for (int i = 1; i <= q; i++) {
    if (type[i] == 1) {
      x[i] = lower_bound(all_x.begin(), all_x.end(), x[i]) - all_x.begin() + 1;
      y[i] = lower_bound(all_y.begin(), all_y.end(), y[i]) - all_y.begin() + 1;
    } else {
      x1[i] = lower_bound(all_x.begin(), all_x.end(), x1[i]) - all_x.begin() + 1;
      x2[i] = lower_bound(all_x.begin(), all_x.end(), x2[i]) - all_x.begin() + 1;
      yy[i] = lower_bound(all_y.begin(), all_y.end(), yy[i]) - all_y.begin() + 1;
      y2[i] = lower_bound(all_y.begin(), all_y.end(), y2[i]) - all_y.begin() + 1;
    }
  }

  // We know that in total, O(Q log^2(Q)) fenwick tree cells will be touched
  // across all updates and queries. Instead of finding all O(Q log^2(Q)) cells,
  // the code below finds only O(Q log Q) important Fenwick Tree cells that we
  // need to consider, optimizing our memory usage to O(Q log Q).
  for (int i = 1; i <= q; i++) {
    if (type[i] == 1) {
      fake_query(1, x[i], y[i]);
      fake_update(0, x[i], y[i], +1);
    } else {
      fake_query(0, x2[i], y2[i]);
      fake_query(0, x1[i] - 1, y2[i]);
      fake_query(0, x2[i], yy[i] - 1);
      fake_query(0, x1[i] - 1, yy[i] - 1);
      fake_update(1, x1[i], yy[i], +1);
      fake_update(1, x1[i], y2[i] + 1, -1);
      fake_update(1, x2[i] + 1, yy[i], -1);
      fake_update(1, x2[i] + 1, y2[i] + 1, +1);
    }
  }

  // Resize each BIT to use only the amount of cells that it actually requires.
  for (int id = 0; id < 2; id++) {
    for (int i = 0; i < X_LIM; i++) {
      sort(fake_bit[id][i].begin(), fake_bit[id][i].end());
      fake_bit[id][i].resize(unique(fake_bit[id][i].begin(), fake_bit[id][i].end()) - fake_bit[id][i].begin());
      bit[id][i].resize(fake_bit[id][i].size() + 1);
    }
  }

  // Now answer each query normally.
  long long ans = 0;
  for (int i = 1; i <= q; i++) {
    if (type[i] == 1) {
      ans += query(1, x[i], y[i]);
      printf("%lld\n", ans);
      update(0, x[i], y[i], +1);
    } else {
      ans += query(0, x2[i], y2[i]);
      ans -= query(0, x1[i] - 1, y2[i]);
      ans -= query(0, x2[i], yy[i] - 1);
      ans += query(0, x1[i] - 1, yy[i] - 1);
      printf("%lld\n", ans);
      update(1, x1[i], yy[i], +1);
      update(1, x1[i], y2[i] + 1, -1);
      update(1, x2[i] + 1, yy[i], -1);
      update(1, x2[i] + 1, y2[i] + 1, +1);
    }
  }
}
