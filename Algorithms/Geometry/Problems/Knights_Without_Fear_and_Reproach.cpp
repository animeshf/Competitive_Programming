// 2015-2016 Samara Regional Intercollegiate Programming Contest
// Problem H

#include "bits/stdc++.h"
#define y1 p1 // C++ is weird
using namespace std;

const long double EPS = 1e-9;

// Start of Geometry Template

struct PT {
    long double x, y, z;
    PT() {}
    PT(long double x, long double y, long double z = 0) : x(x), y(y), z(z) {}
    PT(const PT &p) : x(p.x), y(p.y) {}
    bool operator == (PT other) const {
     return (fabs(x - other.x) < EPS && (fabs(y - other.y) < EPS)); }
    PT operator + (const PT &p)  const { return PT(x + p.x, y + p.y, z + p.z); }
    PT operator - (const PT &p)  const { return PT(x - p.x, y - p.y, z - p.z); }
    PT operator * (long double c)     const { return PT(x*c, y*c, z*c); }
    PT operator / (long double c)     const { return PT(x / c, y / c, z / c); }
};

long double dist(PT p, PT q) {
    long double dx = p.x - q.x;
    long double dy = p.y - q.y;
    return sqrt(dx * dx + dy * dy);
}

PT RotateCCW90(PT p) { return PT(-p.y, p.x); }

struct Circle {
    PT p;
    long double radius;
    Circle(PT x = PT(0, 0), long double r = 0.0) {
        p = x;
        radius = r;
    }
};

vector<PT> CircleCircleIntersection(Circle C, Circle D) {
    PT a = C.p; long double r = C.radius;
    PT b = D.p; long double R = D.radius;
    vector<PT> ret;
    long double d = dist(a, b);
    if (d > r + R || d + min(r, R) < max(r, R)) return ret;
    long double x = (d*d - R*R + r*r) / (2 * d);
    long double y = sqrt(r*r - x*x);
    PT v = (b - a) / d;
    ret.push_back(a + v*x + RotateCCW90(v)*y);
    if (y > 0)
        ret.push_back(a + v*x - RotateCCW90(v)*y);
    for (PT pt: ret) {
        if (isnan(pt.x)) {
            cout << y << endl;
            cout << a.x << ' ' << a.y << ' ' << r << endl;
            cout << b.x << ' ' << b.y << ' ' << R << endl;
            exit(0);
        }
    }
    return ret;
}

// End of Geometry Template

long double r;
long double x1, x2, y1, y2;
long double sol, ax, ay;
vector < Circle > candidates;

// Trying to avoid some dumb edge cases by adding some "nice" points.
inline void moo(long double radius) {
    // Stuff from center
    candidates.push_back(Circle(PT(0, 0), radius));
    candidates.push_back(Circle(PT(r - radius, 0), radius));
    candidates.push_back(Circle(PT(0, -r + radius), radius));
    candidates.push_back(Circle(PT(-r + radius, 0), radius));
    candidates.push_back(Circle(PT(0, r - radius), radius));

    // Stuff from the first tree
    candidates.push_back(Circle(PT(x1 + radius, y1), radius));
    candidates.push_back(Circle(PT(x1 - radius, y1), radius));
    candidates.push_back(Circle(PT(x1, y1 + radius), radius));
    candidates.push_back(Circle(PT(x1, y1 - radius), radius));

    // Stuff from the second tree
    candidates.push_back(Circle(PT(x2 + radius, y2), radius));
    candidates.push_back(Circle(PT(x2 - radius, y2), radius));
    candidates.push_back(Circle(PT(x2, y2 + radius), radius));
    candidates.push_back(Circle(PT(x2, y2 - radius), radius));
}

inline bool check(Circle c) {
    PT cur = c.p;
    if (isnan(cur.x)) return false;
    long double radius = c.radius;
    long double distance_from_center = dist(PT(0, 0), cur);
    // Should lie within the garden.
    if (distance_from_center + radius > r + EPS) {
        return false;
    }
    // Should not touch the two trees.
    long double distance_from_tree = dist(cur, PT(x1, y1));
    if (distance_from_tree < radius - EPS) {
        return false;
    }
    distance_from_tree = dist(cur, PT(x2, y2));
    if (distance_from_tree < radius - EPS) {
        return false;
    }
    sol = radius;
    ax = cur.x;
    ay = cur.y;
    return true;
}

inline bool solve(long double radius) {
    candidates.clear();
    moo(radius);
    // Claim: The optimal solution touches the circumference and
    // atleast one of the two trees in the garden.
    // We draw three circles:
    // 1) Center = (0, 0), Radius = (r - radius)
    // 2) Center = (x1, y1), Radius = radius
    // 3) Center = (x2, y2), Radius = radius
    // If our claim is true, we just need to consider intersections of these
    // circles as candidate points for our center.
    Circle un = Circle(PT(0, 0), (long double) r - radius);
    Circle deux = Circle(PT(x1, y1), radius);
    Circle trois = Circle(PT(x2, y2), radius);
    vector < PT > points = CircleCircleIntersection(un, deux);
    for (PT cur: points) {
        candidates.push_back(Circle(cur, radius));
    }
    points = CircleCircleIntersection(deux, trois);
    for (PT cur: points) {
        candidates.push_back(Circle(cur, radius));
    }
    points = CircleCircleIntersection(un, trois);
    for (PT cur: points) {
        candidates.push_back(Circle(cur, radius));
    }
    bool ok = false;
    for (Circle candidate: candidates) {
        ok |= check(candidate);
    }
    return ok;
}

int main() {
    freopen ("inp.in", "r", stdin);
    cin >> r;
    cin >> x1 >> y1;
    cin >> x2 >> y2;
    cout << fixed << setprecision(20);
    long double qs = 0.0, qe = r;
    for (int i = 1; i <= 100; i++) {
        long double mid = (qs + qe) * 0.5;
        if (solve(mid)) {
            qs = mid;
        } else {
            qe = mid;
        }
    }
    cout << sol << endl << ax << ' ' << ay << endl;
}