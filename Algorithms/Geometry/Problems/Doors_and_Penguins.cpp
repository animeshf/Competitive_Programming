// Rocky Mountain Regional 2006.
// AC on Contest Data (Downloaded locally and checked)

#include "bits/stdc++.h"
using namespace std;

const int N = 10000;

/**
    The code below finds the convex hull of a given set of points.
    It assumes number of points it receives is at least three.

    It uses Graham Scan to compute the convex hull in O(n log n).

    It should work well with integer coordinates.
    The tie-breaking schema in the compare() function might break
    down if the coordinates are doubles. It is advisable to use the
    upper hull/lower hull method in those cases.
**/

struct point{
    int x, y;
    point(int _x = 0, int _y = 0){
        x = _x, y = _y;
    }
    friend bool operator < (point a, point b){
        return (a.x == b.x) ? (a.y < b.y) : (a.x < b.x);
    }
};

int idx;
point root;
point hull[N];

inline long long area(point a, point b, point c){
    return (b.x - a.x) * 1LL * (c.y - a.y) - (b.y - a.y) * 1LL * (c.x - a.x);
}

inline long long dist(point a, point b){
    return (a.x - b.x) * 1LL * (a.x - b.x) + (a.y - b.y) * 1LL * (a.y - b.y);
}

inline bool is_right(point a, point b){
    int dx = (b.x - a.x);
    int dy = (b.y - a.y);
    return (dx > 0) || (dx == 0 && dy > 0);
}

inline bool compare(point b, point c){
    point a = root;
    long long det = area(a, b, c);
    if(det == 0){
        if(is_right(a, b) != is_right(a, c))
            return is_right(a, b);
        return (dist(a, b) < dist(a, c));
    }
    return (det > 0);
}

inline void convexHull(vector < point > pt){
    int num_points = (int) pt.size();
    int min_x = pt[0].x, min_y = pt[0].y, min_idx = 0;
    for(int i = 1; i < num_points; i++){
        if(pt[i].y < min_y || (pt[i].y == min_y && pt[i].x < min_x)){
            min_x = pt[i].x;
            min_y = pt[i].y;
            min_idx = i;
        }
    }
    swap(pt[0], pt[min_idx]);
    root = pt[0];
    sort(pt.begin() + 1, pt.end(), compare);
    idx = 3;
    hull[1] = pt[0], hull[2] = pt[1], hull[3] = pt[2];
    for(int i = 3; i < num_points; i++){
        while(area(hull[idx - 1], hull[idx], pt[i]) < 0) idx--;
        hull[++idx] = pt[i];
    }
}

// End of Convex Hull Template

int d, p, tc;
vector < point > colors[2];

inline void show(bool result) {
    if (tc > 0) cout << endl;
    if (result) {
        cout << "Case " << tc + 1 << ": It is possible to separate the two groups of vendors." << endl;
    } else {
        cout << "Case " << tc + 1 << ": It is not possible to separate the two groups of vendors." << endl;
    }
    tc++;
}

inline bool check(point a, point b, int color) {
    bool pos = false, neg = false;
    for (point c: colors[color ^ 1]) {
        long long z = area(a, b, c);
        if (z == 0) {
            return false;
        }
        if ((z < 0 && pos) || (z > 0 && neg)) {
            return false;
        }
        pos |= (z > 0);
        neg |= (z < 0);
    }
    for (point c: colors[color]) {
        long long z = area(a, b, c);
        if (z == 0) {
            continue;
        }
        if ((z < 0 && neg) || (z > 0 && pos)) {
            return false;
        }
    }
    return true;
}

inline bool solve() {
    convexHull(colors[0]);
    for (int i = 1; i <= idx; i++) {
        int j = (i == idx) ? (1) : (i + 1);
        if (check(hull[i], hull[j], 0)) {
            return true;
        }
    }
    convexHull(colors[1]);
    for (int i = 1; i <= idx; i++) {
        int j = (i == idx) ? (1) : (i + 1);
        if (check(hull[i], hull[j], 1)) {
            return true;
        }
    }
    return false;
}

int main() {
    ios :: sync_with_stdio(false);
    while (true) {
        cin >> d >> p;
        if (d == 0 && p == 0) {
            break;
        }
        colors[0].clear(); colors[1].clear();
        int xa, ya, xb, yb;
        for (int i = 1; i <= d; i++) {
            cin >> xa >> ya >> xb >> yb;
            colors[0].push_back(point(xa, ya));
            colors[0].push_back(point(xb, yb));
            colors[0].push_back(point(xb, ya));
            colors[0].push_back(point(xa, yb));
        }
        for (int i = 1; i <= p; i++) {
            cin >> xa >> ya >> xb >> yb;
            colors[1].push_back(point(xa, ya));
            colors[1].push_back(point(xb, yb));
            colors[1].push_back(point(xb, ya));
            colors[1].push_back(point(xa, yb));
        }
        show(solve());
    }
}