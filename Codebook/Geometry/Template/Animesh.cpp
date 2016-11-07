// Save The Trees
#include <bits/stdc++.h>
using namespace std;

const int MAX = 1e5 + 5;
int t, n, cur, idx, arr[MAX];

struct point{
    int x, y;
    point(int _x = 0, int _y = 0){
        x = _x, y = _y;
    }
    friend bool operator < (point a, point b){
        return (a.x == b.x) ? (a.y < b.y) : (a.x < b.x);
    }
};

point pt[MAX * 2], hull[MAX * 2];
set < point > vals;

// Returns closest distance between point (px, py) and the segment (x1, y1) --> (x2, y2)
inline double dist_to_segment(int x1, int y1, int x2, int y2){
    double dx = x2 - x1;
    double dy = y2 - y1;
    double t = ((px - x1) * 1.0 * dx + (py - y1) * 1.0 * dy) * 1.0 / (dx * 1.0 * dx + dy * 1.0 * dy);
    // See if this represents one of the segment end points or a point in the middle.
    if (t < 0){
        dx = px - x1;
        dy = py - y1;
    }
    else if (t > 1){
        dx = px - x2;
        dy = py - y2;
    }
    else{
        double cx = x1 + t * 1.0 * dx, cy = y1 + t * 1.0 * dy;
        dx = px - cx;
        dy = py - cy;
    }
    return sqrt(dx * 1LL * dx + dy * 1LL * dy);
}

// Area of the triangle formed by the ordered triplet (a, b, c)
// Returns a positive value if a -> b -> c forms a counterclockwise turn 
inline long long area(point a, point b, point c){
    return (b.x - a.x) * 1LL * (c.y - a.y) - (b.y - a.y) * 1LL * (c.x - a.x);
}

// Distance between points a and b
inline long long dist(point a, point b){
    return (a.x - b.x) * 1LL * (a.x - b.x) + (a.y - b.y) * 1LL * (a.y - b.y);
}

// Returns true if b is to the right quadrant with respect to a as the origin
inline bool is_right(point a, point b){
    int dx = (b.x - a.x);
    int dy = (b.y - a.y);
    return (dx > 0) || (dx == 0 && dy > 0);
}

// Sorts all points in counterclockwise order with respect to pt[1]
inline bool compare(point b, point c){
    long long det = area(pt[1], b, c);
    if(det == 0){
        // If one is to the right of pt[1] and other isn't
        if(is_right(pt[1], b) != is_right(pt[1], c))
            return is_right(pt[1], b);
        return (dist(pt[1], b) < dist(pt[1], c));
    }
    return (det > 0);
}

// Convex Hull of pt[] array : 
// hull[] stores the convex hull in ccw order, idx denotes # of points in hull[]
inline void convexHull(){
    int min_x = pt[1].x, min_y = pt[1].y, min_idx = 1;
    // Find lowest point, it's always in the hull
    for(int i = 2; i <= cur; i++){
        if(pt[i].y < min_y || (pt[i].y == min_y && pt[i].x < min_x)){
            min_x = pt[i].x;
            min_y = pt[i].y;
            min_idx = i;
        }
    }
    swap(pt[1], pt[min_idx]);
    // Sort all points with respect to lowest point
    sort(pt + 2, pt + 1 + cur, compare);
    idx = 3;
    // Add first 3 points to the stack and simulate the process
    hull[1] = pt[1], hull[2] = pt[2], hull[3] = pt[3];
    for(int i = 4; i <= cur; i++){
        // Clockwise turn --> pop()
        while(area(hull[idx - 1], hull[idx], pt[i]) < 0) idx--;
        hull[++idx] = pt[i];
    }
}

int main(){
    scanf("%d", &t);
    while(t--){
        scanf("%d", &n);
        vals.clear();
        for(int i = 1; i <= n; i++) scanf("%d", arr + i);
        int mn = arr[1], mx = arr[1];
        cur = 0;
        for(int i = 2; i <= n; i++){
            if(vals.find(point(mn, arr[i])) == vals.end()){
                pt[++cur] = point(mn, arr[i]);
                vals.insert(pt[cur]);
            }
            if(vals.find(point(mx, arr[i])) == vals.end()){
                pt[++cur] = point(mx, arr[i]);
                vals.insert(pt[cur]);
            }
            mn = min(mn, arr[i]), mx = max(mx, arr[i]);
        }
        if(cur <= 2){ // No convex hull if less than 3 points!
            printf("0\n");
            continue;
        }
        convexHull();
        long long res = 0;
        // Triangulate the area : Do not need abs() sign as the points are in ccw order
        for(int i = 2; i < idx; i++){
            res += area(hull[1], hull[i], hull[i + 1]);
        }
        printf("%lld\n", res);
    }
}
