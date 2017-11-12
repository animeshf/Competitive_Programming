/*

Determines the diametral pair of a range of PTs. The diamter
of a set of PTs is the largest distance between any two
PTs in the set. A diametral pair is a pair of PTs in the
set whose distance is equal to the set's diameter. The following
program uses rotating calipers method to find a solution.

Time Complexity: O(n log n) on the number of PTs in the set.

*/

double area(const PT & o, const PT & a, const PT & b) {
  return fabs(cross(o, a, b));
}

template<class It> pair<PT, PT> diametral_pair(It lo, It hi) {
  vector<PT> h = convex_hull(lo, hi);
  int m = h.size();
  if (m == 1) return make_pair(h[0], h[0]);
  if (m == 2) return make_pair(h[0], h[1]);
  int k = 1;
  while (area(h[m - 1], h[0], h[(k + 1) % m]) > area(h[m - 1], h[0], h[k]))
    k++;
  double maxdist = 0, d;
  pair<PT, PT> res;
  for (int i = 0, j = k; i <= k && j < m; i++) {
    d = norm(h[i] - h[j]);
    if (d > maxdist) {
      maxdist = d;
      res = make_pair(h[i], h[j]);
    }
    while (j < m && area(h[i], h[(i + 1) % m], h[(j + 1) % m]) >
                    area(h[i], h[(i + 1) % m], h[j])) {
      d = norm(h[i] - h[(j + 1) % m]);
      if (d > maxdist) {
        maxdist = d;
        res = make_pair(h[i], h[(j + 1) % m]);
      }
      j++;
    }
  }
  return res;
}

/*** Example Usage ***/

int main() {
  vector<PT> v;
  v.push_back(PT(0, 0));
  v.push_back(PT(3, 0));
  v.push_back(PT(0, 3));
  v.push_back(PT(1, 1));
  v.push_back(PT(4, 4));
  pair<PT, PT> res = diametral_pair(v.begin(), v.end());
  cout << "diametral pair: (" << res.first.X << "," << res.first.Y << ") ";
  cout << "(" << res.second.X << "," << res.second.Y << ")\n";
  cout << "diameter: " << abs(res.first - res.second) << "\n";
  return 0;
}