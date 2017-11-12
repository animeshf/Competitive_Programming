/*
Determines the convex hull from a range of PTs, that is, the
smallest convex polygon (a polygon such that every line which
crosses through it will only cross through it once) that contains
all of the PTs. This function uses the monotone chain algorithm
to compute the uperp and lower hulls separately.
Returns: a vector of the convex hull PTs in clockwise order.
Complexity: O(n log n) on the number of PTs given
Notes: To yield the hull PTs in counterclockwise order,
       replace every  usageof GE() in the function with LE().
       To have the first PT on the hull repeated as the last,
       replace the last line of the function to res.resize(k);
*/

//change < 0 comparisons to > 0 to produce hull PTs in CCW order
double cw(const PT & o, const PT & a, const PT & b) {
  return cross(a - o, b - o) < -eps;
}

//convex hull from a range [lo, hi) of PTs
//monotone chain in O(n log n) to find hull PTs in CW order
//notes: the range of input PTs will be sorted lexicographically
template<class It> vector<PT> convex_hull(It lo, It hi) {
  int k = 0;
  if (hi - lo <= 1) return vector<PT>(lo, hi);
  vector<PT> res(2 * (int)(hi - lo));
  sort(lo, hi, [](PT a, PT b) {
                return MP(a.X, a.Y) < MP(b.X, b.Y);
                });
  for (It it = lo; it != hi; ++it) {
    while (k >= 2 && !cw(res[k - 2], res[k - 1], *it)) k--;
    res[k++] = *it;
  }
  int t = k + 1;
  for (It it = hi - 2; it != lo - 1; --it) {
    while (k >= t && !cw(res[k - 2], res[k - 1], *it)) k--;
    res[k++] = *it;
  }
  res.resize(k - 1);
  return res;
}

int main() {
    vector<PT> v; // init points.
    vector<PT> hull = convex_hull(v.begin(), v.end());
}