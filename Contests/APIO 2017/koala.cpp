#include "koala.h"
#include "bits/stdc++.h"
using namespace std;

const int NMAX = 100;

int B[NMAX], R[NMAX];
vector < int > large;
int nn, ww;

int minValue(int N, int W) {
    for (int i = 0; i < N; i++) {
        B[i] = R[i] = 0;
    }
    B[0] = 1;
    playRound(B, R);
    for (int i = 0; i < N; i++) {
        if (R[i] == 0) {
            return i;
        }
    }
    return 0;
}

int maxValue(int N, int W) {
    large.clear();
    for (int i = 0; i < N; i++) {
        large.push_back(i);
    }
    while ((int) large.size() > 1) {
        for (int i = 0; i < N; i++) {
            B[i] = R[i] = 0;
        }
        int token = W / (int) large.size();
        for (int id : large) {
            B[id] = token;
        }
        playRound(B, R);
        large.clear();
        for (int i = 0; i < N; i++) {
            if (R[i] > token) {
                large.push_back(i);
            }
        }
    }
    return large[0];
}

int greaterValue(int N, int W) {
    int l = 1, r = min(W / 2, 9);
    while (l <= r) {
        int x = (l + r) >> 1;
        for (int i = 0; i < N; i++) {
            B[i] = R[i] = 0;
        }
        B[0] = B[1] = x; // Let's use this price.
        playRound(B, R);
        bool bought_zero = R[0] > B[0];
        bool bought_one = R[1] > B[1];
        if (bought_zero == bought_one) {
            if (bought_one == true) {
                // Increase price
                l = x + 1;
            } else {
                // Decrease price
                r = x - 1;
            }
        } else {
            if (bought_one) {
                // Item one has greater value for the same price
                return 1;
            } else {
                // Item zero has greater value for the same price
                return 0;
            }
        }
    }
    return 0;
}

inline bool comp(int N, int W, int x, int y) {
    // returns true if P[x] < P[y]
    if (x == y) {
        return false;
    }
    int l = 1, r = min(W / 2, 13);
    while (l <= r) {
        int price = (l + r) >> 1;
        for (int i = 0; i < N; i++) {
            B[i] = R[i] = 0;
        }
        B[x] = B[y] = price; // Let's use this price.
        playRound(B, R);
        bool bought_x = R[x] > B[x];
        bool bought_y = R[y] > B[y];
        if (bought_x == bought_y) {
            if (bought_x == true) {
                // Increase price
                l = price + 1;
            } else {
                // Decrease price
                r = price - 1;
            }
        } else {
            if (bought_x) {
                // Item x has greater value for the same price i.e. P[x] > P[y]
                return 0;
            } else {
                // Item y has greater value for the same price i.e P[x] < P[y]
                return 1;
            }
        }
    }
    return 0;
}

inline vector < int > conquer(vector < int > l, vector < int > r) {
    vector < int > ans;
    int n = (int) l.size();
    int m = (int) r.size();
    ans.clear();
    ans.resize(n + m);
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < n && j < m) {
        bool comparator = comp(nn, ww, l[i], r[j]);
        if (comparator == true) {
            ans[k++] = l[i++];
        } else {
            ans[k++] = r[j++];
        }
    }
    while (i < n) {
        ans[k++] = l[i++];
    }
    while (j < m) {
        ans[k++] = r[j++];
    }
    return ans;
}

inline vector < int > mergeSort(vector < int > arr) {
    int l = 0, r = (int) arr.size() - 1;
    if (r == 0) {
        return arr;
    }
    int mid = (l + r) / 2;
    vector < int > lef;
    vector < int > rig;
    for (int i = l; i <= mid; i++) {
        lef.push_back(arr[i]);
    }
    for (int i = mid + 1; i <= r; i++) {
        rig.push_back(arr[i]);
    }
    lef = mergeSort(lef);
    rig = mergeSort(rig);
    return conquer(lef, rig);
}

void allValues(int N, int W, int *P) {
    if (W == N * 2) {
        cout << "LOLZ, NOT NOW!\n";
        return;
    }
    nn = N; ww = W;
    vector < int > arr;
    arr.clear();
    arr.resize(N);
    for (int i = 0; i < N; i++) {
        arr[i] = i;
    }
    arr = mergeSort(arr);
    for (int i = 0; i < N; i++) {
        // P[i] = Position of (i) in sorted list arr.
        for (int j = 0; j < N; j++) {
            if (arr[j] == i) {
                P[i] = j + 1;
            }
        }
    }
}
