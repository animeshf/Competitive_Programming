// Some CodeForces Round

#include "bits/stdc++.h"
using namespace std;

const int N = 1e5 + 5;
const double PI = 3.14159265358979323846264338327;
int n, px, py;
int x[N], y[N];

// Returns distance between points (x1, y1) and (x2, y2)
inline double dist(int x1, int y1, int x2, int y2){
	return sqrt((x1 - x2) * 1LL * (x1 - x2) + (y1 - y2) * 1LL * (y1 - y2));
}

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


int main(){
	freopen("ioi.in", "r", stdin);
	cin >> n >> px >> py;
	for(int i = 1; i <= n; i++){
		cin >> x[i] >> y[i];
	}
	double r1 = 0;
	for(int i = 1; i <= n; i++){
		r1 = max(r1, dist(px, py, x[i], y[i]));
	}
	double r2 = 1e18;
	for(int i = 1; i <= n; i++){
		int j = (i < n) ? (i + 1) : (1);
		r2 = min(r2, dist_to_segment(x[i], y[i], x[j], y[j]));
	}
	double ans = PI * (r1 * r1 - r2 * r2);
	cout << fixed << setprecision(18) << ans << '\n';
}
