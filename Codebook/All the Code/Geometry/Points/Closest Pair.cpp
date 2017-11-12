/*

Given a range containing distinct PTs on the Cartesian plane,
determine two PTs which have the closest possible distance.
A divide and conquer algorithm is used. Note that the ordering
of PTs in the input range may be changed by the function.

Time Complexity: O(n log^2 n) where n is the number of PTs.

*/

bool cmp_x(const PT& a, const PT& b) { return a.X < b.X; }
bool cmp_y(const PT& a, const PT& b) { return a.Y < b.Y; }

template<class It>
double rec(It lo, It hi, pair<PT, PT>& res, double mindist) {
  if (lo == hi) return DBL_MAX;
  It mid = lo + (hi - lo) / 2;
  double midx = mid->X;
  double d1 = rec(lo, mid, res, mindist);
  mindist = min(mindist, d1);
  double d2 = rec(mid + 1, hi, res, mindist);
  mindist = min(mindist, d2);
  sort(lo, hi, cmp_y);
  int size = 0;
  It t[hi - lo];
  for (It it = lo; it != hi; ++it)
    if (fabs(it->X - midx) < mindist)
      t[size++] = it;
  FOR(i,0,size) {
    FOR(j,i+1,size) {
      PT a = *t[i], b = *t[j];
      if (b.Y - a.Y >= mindist) break;
      double dist = norm(a - b);
      if (mindist > dist) {
        mindist = dist;
        res = MP(a, b);
      }
    }
  }
  return mindist;
}

template<class It> pair<PT, PT> closest_pair(It lo, It hi) {
  pair<PT, PT> res;
  sort(lo, hi, cmp_x);
  rec(lo, hi, res, DBL_MAX);
  return res;
}

int main() {
  vector<PT> v; // some points
  pair<PT, PT> sol = closest_pair(v.begin(), v.end());
  return 0;
}