// Bubble Cup 2017 - Problem E - Property

#include "bits/stdc++.h"
using namespace std;

const int N = 50005;

int n;
int id[N];
vector < pair < int, int > > val;
vector < int > sol;

int main() {
    freopen ("inp.in", "r", stdin);
    ios :: sync_with_stdio(false);
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> id[i];
    }
    id[n] = id[0];
    for (int i = 0; i < n; i++) {
        val.push_back(make_pair((n - id[i]) - id[i + 1], i));
    }
    sort(val.begin(), val.end());
    sol.resize(n);
    for (int i = 0; i < n; i++) {
        sol[val[i].second] = n - i - 1;
    }
    for (int i = 0; i < n; i++) {
        cout << sol[i] << ' ';
    }
    cout << endl;
    return 0;
}