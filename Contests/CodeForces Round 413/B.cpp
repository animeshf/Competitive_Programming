#include "bits/stdc++.h"
using namespace std;

const int N = 200005;
const int COLORS = 4;

set < pair < int, int > > colors[COLORS];
int n, m;
int p[N], a[N], b[N];

int main() {
    freopen ("inp.in", "r", stdin);
    ios :: sync_with_stdio(false);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> p[i];
    }
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    for (int i = 1; i <= n; i++) {
        cin >> b[i];
    }
    for (int i = 1; i <= n; i++) {
        colors[a[i]].insert({p[i], i});
        colors[b[i]].insert({p[i], i});
    }
    cin >> m;
    for (int i = 1; i <= m; i++) {
        int cj; cin >> cj;
        if (!colors[cj].size()) {
            cout << "-1 ";
            continue;
        }
        pair < int, int > item = *colors[cj].begin();
        int price = item.first;
        int index = item.second;
        cout << price << " ";
        colors[cj].erase(item);
        int oc = (cj == a[index]) ? (b[index]) : (a[index]);
        if (colors[oc].count(item)) {
            colors[oc].erase(item);
        }
    }
}