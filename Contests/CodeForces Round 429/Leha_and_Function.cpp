#include "bits/stdc++.h"
using namespace std;

const int N = 222222;

int n, a[N], sol[N];
pair < int, int > b[N];

int main() {
    freopen ("inp.in", "r", stdin);
    ios :: sync_with_stdio(false);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    sort(a + 1, a + n + 1);
    for (int i = 1; i <= n; i++) {
        int x; cin >> x;
        b[i] = {x, i};
    }
    sort(b + 1, b + n + 1);
    for (int i = 1; i <= n; i++) {
        sol[b[i].second] = a[n - i + 1];
    }
    for (int i = 1; i <= n; i++) {
        cout << sol[i];
        if (i < n) cout << ' ';
        else cout << endl;
    }
}

/**
look at all k sized subsets
find minimal element
take average over all

so like number of subsets = n choose k = denominator
n - (n - k + 1)
numerator = 1(n - 1 choose k - 1) + 2(n - 2 choose k - 1) + 3(n - 3 choose k - 1) ... + (n - k + 1)(k - 1 choose k - 1)
= n choose k + (n - 2 choose k - 1) + 2(n - 3 choose k - 1) + ...
= n choose k + (n - 1) choose k  + (n - 2).....
= (n + 1) choose (k + 1)
n choose k = (n - 1 choose k - 1) + (n - 2 choose k - 1).. + (n - k + 1)(k - 1 choose k - 1)
1, 2, 3, 4, 5.....n

F(n, k) = (n + 1) choose (k + 1) / n choose k
        = (n + 1)! k! (n - k)!
        ------------------------
          (k + 1)! (n - k)! (n)!
        = (n + 1)
        -----------
          (k + 1)

        = (n + 1) / (k + 1)
k = 1
n + 1 / 2
*/
