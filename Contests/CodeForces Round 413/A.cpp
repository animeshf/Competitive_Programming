#include "bits/stdc++.h"
using namespace std;

const int N = 200000;
const int MAGIC = 10000;

int n, t, k, d;

int main() {
    freopen ("inp.in", "r", stdin);
    ios :: sync_with_stdio(false);
    cin >> n >> t >> k >> d;
    int num_cycles = (n + k - 1) / k;
    int time_taken = t * num_cycles;
    for (int x = 1; x <= MAGIC; x++) {
        int cakes_baked_by_second = x * k;
        int cycles_first = (d + x * t) / t;
        int cakes_baked_by_first = cycles_first * k;
        if (cakes_baked_by_first + cakes_baked_by_second >= n) {
            if (d + x * t < time_taken) {
                cout << "YES\n";
                return 0;
            }
        }
    }
    cout << "NO\n";
}