#include "bits/stdc++.h"
using namespace std;

const int A = 26;

int n, m;
bool adj[A][A];

int main() {
    ios :: sync_with_stdio(false);
    cin >> m >> n;
    for (int i = 0; i < A; i++) {
        adj[i][i] = true;
    }
    for (int i = 1; i <= m; i++) {
        char x, y; cin >> x >> y;
        adj[x - 'a'][y - 'a'] = true;
    }
    for (int k = 0; k < A; k++) {
        for (int i = 0; i < A; i++) {
            for (int j = 0; j < A; j++) {
                adj[i][j] |= (adj[i][k] & adj[k][j]);
            }
        }
    }
    while (n--) {
        string a, b; cin >> a >> b;
        if (a.size() != b.size()) {
            cout << "no\n";
        } else {
            bool rekt = false;
            for (int i = 0; i < a.size(); i++) {
                int p = a[i] - 'a';
                int q = b[i] - 'a';
                if (!adj[p][q]) {
                    rekt = true;
                }
            }
            if (rekt) {
                cout << "no\n";
            } else {
                cout << "yes\n";
            }
        }
    }
}