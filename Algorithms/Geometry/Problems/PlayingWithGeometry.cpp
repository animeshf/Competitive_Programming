// Playing with Geometry - SWERC 2014

#include "bits/stdc++.h"
#define ii pair < int, int >
using namespace std;

const int N = 505;

int n, m;

vector < ii > compress(vector < ii > values) {
    vector < ii > result(values);
    vector < int > x, y;
    for (ii cur: values) {
        x.push_back(cur.first);
        y.push_back(cur.second);
    }
    sort(x.begin(), x.end());
    x.resize(unique(x.begin(), x.end()) - x.begin());
    sort(y.begin(), y.end());
    y.resize(unique(y.begin(), y.end()) - y.begin());
    for (ii &e: result) {
        e.first = lower_bound(x.begin(), x.end(), e.first) - x.begin();
        e.second = lower_bound(y.begin(), y.end(), e.second) - y.begin();
    }
    return result;
}

// CCW Rotation
// cos(theta) -sin(theta)
// sin(theta) cos(theta)
// theta = 90 ===> -y, x
vector < ii > rotate(vector < ii > values) {
    vector < ii > result(values);
    for (ii &cur: result) {
        int x = cur.first;
        int y = cur.second;
        cur.first = -y;
        cur.second = x;
    }
    return result;
}

bool check(vector < ii > a, vector < ii > b) {
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    cin >> n;
    vector < ii > a(n);
    for (int i = 0; i < n; i++) {
        int x, y; cin >> x >> y;
        a[i] = {x, y};
    }
    cin >> m;
    vector < ii > b(m);
    for (int i = 0; i < m; i++) {
        int x, y; cin >> x >> y;
        b[i] = {x, y};
    }
    if (n != m) {
        cout << "no" << endl;
        return 0;
    }
    a = compress(a);
    b = compress(b);
    for (int k = 0; k < 4; k++) {
        if (check(a, b)) {
            cout << "yes" << endl;
            return 0;
        }
        b = compress(rotate(b));
    }
    cout << "no" << endl;
}