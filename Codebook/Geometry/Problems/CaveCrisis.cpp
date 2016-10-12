#include "bits/stdc++.h"
using namespace std;

const double EPS = 1e-9;

int cmp(double x, double y = 0, double tol = EPS) {
    return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}

struct Point {
    double x, y, z;
    Point(){}
    Point(double x, double y, double z = 0.0) : x(x), y(y), z(z) {}

    // Dot product
    double operator * (const Point &p) const {
        return x * p.x + y * p.y + z * p.z;
    }

    Point operator - (const Point &p) const {
        return Point(x - p.x, y - p.y, z - p.z);
    }

    Point operator + (const Point &p) const {
        return Point(x + p.x, y + p.y, z + p.z);
    }

    double length() const {
        return sqrtl(*this * *this);
    }
};

// Scalar * vector product
Point operator * (double t, const Point &p) {
    return Point(t * p.x, t * p.y, t * p.z);
}

// Returns the shortest distance from point p to the segment from a to b.
// This works on 2D and 3D.
inline double distanceBetweenPointAndLine(const Point &a, const Point &b, const Point &p) {
    Point u = b - a;
    Point v = p - a;
    double t = (u * v) / (u * u);
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;
    // Actual intersection point is a + t * (b - a).
    Point boom = a + t*u;
    return (boom - p).length();
}

// Returns the shortest distance between any point in the segment
// from a1 to b1 and any point in the segment from a2 to b2.
// Works in 3D.
inline double distanceBetweenLines(const Point &a1, const Point &b1, const Point &a2, const Point &b2) {
    Point u = b1 - a1;
    Point v = b2 - a2;
    Point w = a1 - a2;

    double a = u * u;
    double b = u * v;
    double c = v * v;
    double d = u * w;
    double e = v * w;

    double den = a*c - b*b;
    double t1, t2;

    if (cmp(den, 0.0) == 0) { // the lines are parallel
        t1 = 0;
        t2 = d / b;
    } else {
        t1 = (b * e - c * d) / den;
        t2 = (a * e - b * d) / den;
    }

    // The shortest distance between the two infinite lines happens from
    // p = a1 + t1 * (b1 - a1) on segment 1 and
    // and q = a2 + t2 * (b2 - a2) on segment 2.

    if (0 <= t1 and t1 <= 1 and 0 <= t2 and t2 <= 1) {
        // We were lucky, the closest distance between the two infinite
        // lines happens right inside both segments.
        Point p = a1 + t1 * u;
        Point q = a2 + t2 * v;
        return (p - q).length();
    } else {
        double option1 = min(distanceBetweenPointAndLine(a1, b1, a2), distanceBetweenPointAndLine(a1, b1, b2));
        double option2 = min(distanceBetweenPointAndLine(a2, b2, a1), distanceBetweenPointAndLine(a2, b2, b1));
        return min(option1, option2);
    }
}

typedef vector < Point > Polygon;
const int n = 110;
Polygon pol[n];
double closestDist[n][n];
bool vis[n];

inline double distanceBetweenPolygons(const Polygon &a, const Polygon &b) {
    double ans = 1e18;
    for (int i = 0; i < a.size(); ++i) {
        int ii = (i + 1) % a.size();
        for (int j = 0; j < b.size(); ++j) {
            int jj = (j + 1) % b.size();
            ans = min(ans, distanceBetweenLines(a[i], a[ii], b[j], b[jj]));
        }
    }
    return ans;
}

inline void dfs(int u, double tolerance, int n) {
    if (vis[u]) return;
    vis[u] = true;
    for (int v = 0; v < n; ++v) {
        if (cmp(closestDist[u][v], tolerance) <= 0) dfs(v, tolerance, n);
    }
}

inline bool isBlocked(double tolerance, int n) {
    fill(vis, vis + n, false);
    dfs(n - 2, tolerance, n);
    return vis[n - 1];
}

int main(){
    int w, n;
    while (cin >> w >> n) {
        if (w == 0 and n == 0) break;
        for (int i = 0; i < n; ++i) {
            int size; cin >> size;
            pol[i].resize(size);
            for (int k = 0; k < size; ++k) {
                cin >> pol[i][k].x >> pol[i][k].y;
                pol[i][k].z = 0.0;
            }
        }
        double h = w / 2.0;
        // Ceiling
        pol[n].clear();
        pol[n].push_back(Point(0, h));
        pol[n].push_back(Point(1000, h));
        pol[n].push_back(Point(1000, h + 1));
        pol[n].push_back(Point(0, h + 1));
        n++;
        // Floor
        pol[n].clear();
        pol[n].push_back(Point(0, -h - 1));
        pol[n].push_back(Point(1000, -h - 1));
        pol[n].push_back(Point(1000, -h));
        pol[n].push_back(Point(0, -h));
        n++;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                closestDist[i][j] = closestDist[j][i] = distanceBetweenPolygons(pol[i], pol[j]);
            }
            closestDist[i][i] = 0;
        }
        if (isBlocked(0.0, n)) {
            cout << "impossible" << '\n';
        } else {
            double low = 0, high = w;
            for (int k = 0; k < n; ++k) {
                for (int i = 0; i < pol[k].size(); ++i) {
                    int ii = (i + 1) % pol[k].size();
                    high = min(high, 2 * distanceBetweenPointAndLine(pol[k][i], pol[k][ii], Point(0, 0)));
                }
            }
            for (int k = 0; k < 150; ++k) {
                double mid = (low + high) / 2;
                if (isBlocked(mid, n)) {
                    high = mid;
                } else {
                    low = mid;
                }
            }
            cout << setprecision(2) << fixed << low / 2 << '\n';
        }
    }
}