#include "bits/stdc++.h"
using namespace std;

const int N = 1500;
const long long INF = 1LL << 60LL;

struct rectangle {
    int x1, y1;
    int x2, y2;
    rectangle(int a = 0, int b = 0, int c = 0, int d = 0) {
        x1 = a, y1 = b, x2 = c, y2 = d;
    }
}arr[N];

inline int get(pair < int, int > a, pair < int, int > b) {
    assert (a.first <= a.second);
    assert (b.first <= b.second);
    if (a.second < b.first) {
        return b.first - a.second - 1;
    } else if (b.second < a.first) {
        return a.first - b.second - 1;
    } else {
        return 0;
    }
}

inline int get(rectangle x, rectangle y) {
    pair < int, int > x_x = {x.x1, x.x2};
    pair < int, int > y_x = {y.x1, y.x2};
    int res = get(x_x, y_x);
    pair < int, int > x_y = {x.y1, x.y2};
    pair < int, int > y_y = {y.y1, y.y2};
    res = max(res, get(x_y, y_y));
    return res;
}

long long ans[N];
int dist[N][N];

int main() {
    freopen ("inp.in", "r", stdin);
    freopen ("C.out", "w", stdout);
    int tc; cin >> tc;
    for (int qq = 1; qq <= tc; qq++) {
        cout << "Case #" << qq << ": ";
        int w, h, b; cin >> w >> h >> b;
        for (int i = 0; i < N; i++) {
            ans[i] = INF;
        }
        arr[0] = rectangle(-1, 0, -1, h - 1);
        for (int z = 1; z <= b; z++) {
            int x, y, xx, yy;
            cin >> x >> y >> xx >> yy;
            arr[z] = rectangle(x, y, xx, yy);
        }
        arr[b + 1] = rectangle(w, 0, w, h - 1);
        for (int i = 0; i <= b + 1; i++) {
            for (int j = 0; j <= b + 1; j++) {
                if (i == j) {
                    dist[i][j] = 0;
                } else {
                    dist[i][j] = get(arr[i], arr[j]);
                }
            }
        }
        set < pair < long long, int > > dijkstra;
        dijkstra.insert(make_pair(0, 0));
        while (!dijkstra.empty()) {
            pair < long long, int > cur = *dijkstra.begin();
            dijkstra.erase(dijkstra.begin());
            long long distance = cur.first;
            int id = cur.second;
            if (distance < ans[id]) {
                ans[id] = distance;
                for (int i = 0; i <= b + 1; i++) {
                    if (id == i) {
                        continue;
                    }
                    dijkstra.insert(make_pair(distance + dist[id][i], i));
                }
            }
        }
        cout << ans[b + 1] << "\n";
    }
}