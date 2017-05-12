#include "bits/stdc++.h"
using namespace std;

const int N = 10005;
const int INF = INT_MAX;

int n;
vector < int > arr;

int main() {
    freopen ("inp.in", "r", stdin);
    freopen ("B.out", "w", stdout);
    int tc; cin >> tc;
    for (int qq = 1; qq <= tc; qq++) {
        cout << "Case #" << qq << ": ";
        cin >> n;
        arr.clear();
        for (int i = 1; i <= n; i++) {
            int x; cin >> x;
            arr.push_back(x);
        }
        int ans = 0;
        for (int it = 1; it <= n; it++) {
            int min_val = INF;
            int min_id = INF;
            for (int i = 0; i < (int) arr.size(); i++) {
                if (arr[i] < min_val) {
                    min_val = arr[i];
                    min_id = i + 1;
                }
            }
            ans += min(n - it + 1 - min_id, min_id - 1);
            arr.erase(arr.begin() + min_id - 1);
        }
        cout << ans << "\n";
    }
}