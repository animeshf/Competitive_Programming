#include "bits/stdc++.h"
using namespace std;

const int N = 10000;

int n, c, m;
int freq[N], seats[N];

int main() {
    freopen ("inp.in", "r", stdin);
    freopen ("C.out", "w", stdout);
    ios :: sync_with_stdio(false);
    int tc; cin >> tc;
    for (int qq = 1; qq <= tc; qq++) {
        cout << "Case #" << qq << ": ";
        cin >> n >> c >> m;
        memset(seats, 0, sizeof seats);
        memset(freq, 0, sizeof freq);
        int rides = 1; // Atleast one ride.
        for (int i = 1; i <= m; i++) {
            int seat, id; cin >> seat >> id;
            freq[id]++; seats[seat]++;
            rides = max(rides, freq[id]);
        }
        int prefix_sum = 0;
        for (int i = 1; i <= n; i++) {
            prefix_sum += seats[i];
            rides = max(rides, (prefix_sum + i - 1) / i);
        }
        cout << rides << " ";
        long long promotions = 0;
        for (int i = 1; i <= n; i++) {
            if (seats[i] > rides) {
                promotions += seats[i] - rides;
            }
        }
        cout << promotions << "\n";
    }
}