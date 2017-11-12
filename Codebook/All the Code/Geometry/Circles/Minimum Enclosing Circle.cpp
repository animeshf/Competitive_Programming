/*

Given a range of points on the 2D cartesian plane, determine
the equation of the circle with smallest possible area which
encloses all of the points. Note: in an attempt to avoid the
worst case, the circles are randomly shuffled before the
algorithm is performed. This is not necessary to obtain the
correct answer, and may be removed if the input order must
be preserved.

Time Complexity: O(n) average on the number of points given.

REQUIRES:
line_line
circle_center (for both three points and diameter made from two points)
*/

template<class It> circle smallest_circle(It lo, It hi) {
  if (lo == hi) return circle_center(0, 0, 0);
  if (lo + 1 == hi) return circle_center(*lo, 0);
  random_shuffle(lo, hi);
  circle res = circle_center(*lo, *(lo + 1));
  for (It i = lo + 2; i != hi; ++i) {
    if (contains(res, *i)) continue;
    res = circle_center(*lo, *i);
    for (It j = lo + 1; j != i; ++j) {
      if (contains(res, *j)) continue;
      res = circle_center(*i, *j);
      for (It k = lo; k != j; ++k)
        if (!contains(res, *k)) res = circle_center(*i, *j, *k);
    }
  }
  return res;
}

int main() {
  vector<PT> v; // some points
  circle ans = smallest_circle(v.begin(), v.end());
}