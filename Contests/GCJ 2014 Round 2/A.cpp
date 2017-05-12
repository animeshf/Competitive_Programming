#include "bits/stdc++.h"
using namespace std;

const int N = 10005;

int n, x;
int arr[N];
bool taken[N];

int main() {
    freopen ("inp.in", "r", stdin);
    freopen ("A.out", "w", stdout);
    int tc; cin >> tc;
    for (int qq = 1; qq <= tc; qq++) {
        cout << "Case #" << qq << ": ";
        cin >> n >> x;
        for (int i = 1; i <= n; i++) {
            cin >> arr[i];
            taken[i] = false;
        }
        sort (arr + 1, arr + n + 1);
        int discs = 0;
        for (int i = n; i >= 1; i--) {
            if (taken[i]) {
                continue;
            }
            discs += 1;
            taken[i] = true;
            for (int j = i - 1; j >= 1; j--) {
                if (taken[j]) {
                    continue;
                }
                if (arr[j] <= x - arr[i]) {
                    taken[j] = true;
                    break;
                }
            }
        }
        cout << discs << "\n";
    }
}