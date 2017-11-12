/*
Given a range of PTs specifying a polygon on the Cartesian
plane, as well as two PTs specifying an infinite line, "cut"
off the right part of the polygon with the line and return the
resulting polygon that is the left part.
Time Complexity: O(n) on the number of PTs in the poylgon.

NEED: line_intersection from Lines section
*/

int orientation(const PT & o, const PT & a, const PT & b) {
  double c = cross(a - o, b - o);
  return LT(c, 0) ? -1 : (GT(c, 0) ? 1 : 0);
}

template<class It>
vector<PT> convex_cut(It lo, It hi, const PT & p, const PT & q) {
  vector<PT> res;
  for (It i = lo, j = hi - 1; i != hi; j = i++) {
    int d1 = orientation(p, q, *j), d2 = orientation(p, q, *i);
    if (d1 >= 0) res.push_back(*j);
    if (d1 * d2 < 0) {
      PT r;
      line_intersection(p, q, *j, *i, &r);
      res.push_back(r);
    }
  }
  return res;
}

int main() {
    vector<PT> polygon;
    vector<PT> cut = convex_cut(polygon.begin(), polygon.end(),
            PT(100, h), PT(-100, h)); // some points determining a line
    return 0;
}