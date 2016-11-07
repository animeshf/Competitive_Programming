#include "bits/stdc++.h"
using namespace std;

const int TMAX = 2050;
const int NMAX = 1005;

class segtree_y{

    long long whole[TMAX];
    long long within[TMAX];
    int N;

    inline void update(int node, int s, int e, int i, int j, long long val){
        if(s > j or e < i) return;
        if(s >= i and e <= j){
            whole[node] += val;
            return;
        }
        int mid = (s + e) >> 1;
        int lc = (node + 1) * 2 - 1;
        int rc = lc + 1; 
        update(lc, s, mid, i, j, val);
        update(rc, mid + 1, e, i, j, val);
        within[node] += (min(e, j) - max(i, s) + 1) * val;
    }

    inline long long query(int node, int s, int e, int i, int j){
        if(s > j or e < i) return 0;
        if(s >= i and e <= j) return (1LL * whole[node] * (e - s + 1) + within[node]);
        int mid = (s + e) >> 1;
        int lc = (node + 1) * 2 - 1;
        int rc = lc + 1; 
        return query(lc, s, mid, i, j) + query(rc, mid + 1, e, i, j) + whole[node] * (min(e,j) - max(s, i) + 1);
    }

public:

    void init(int n){
        N = n;
        memset(whole, 0, sizeof(whole));
        memset(within, 0, sizeof(within));
    }

    long long query(int i, int j){
        return query(0, 0, N - 1, i, j);
    }

    void update(int i, int j, int val){
        update(0, 0, N - 1, i, j, val);
    }

};

class segtree_x{

    segtree_y whole[TMAX];
    segtree_y within[TMAX];

    int N;

    inline void init(int node, int s, int e){
        whole[node].init(N);
        within[node].init(N);
        if(s == e){
            return;
        }
        int mid = (s + e) >> 1;
        int lc = (node + 1) * 2 - 1;
        int rc = lc + 1; 
        init(lc, s, mid); init(rc, mid + 1, e);
    }

    inline void update(int node, int s, int e, int i, int j, int b1, int b2, long long val){
        if(s > j or e < i) return;
        if(s >= i and e <= j){
            whole[node].update(b1, b2, val);
            return;
        }
        int mid = (s + e) >> 1;
        int lc = (node + 1) * 2 - 1;
        int rc = lc + 1; 
        update(lc, s, mid, i, j, b1, b2, val);
        update(rc, mid + 1, e, i, j, b1, b2, val);
        within[node].update(b1, b2, val * (min(j, e) - max(i, s) + 1));
    }

    inline long long query(int node, int s, int e, int i, int j, int b1, int b2){
        if(s > j or e < i) return 0;
        if(s >= i and e <= j){
            return within[node].query(b1, b2) + whole[node].query(b1, b2) * (e - s + 1);
        }
        int mid = (s + e) >> 1;
        int lc = (node + 1) * 2 - 1;
        int rc = lc + 1; 
        return query(lc, s, mid, i, j, b1, b2) + query(rc, mid + 1, e, i, j, b1, b2) + whole[node].query(b1, b2) * (min(j, e) - max(s, i) + 1);
    }

public:
    void init(int n){
        N = n;
        init(0, 0, N - 1);
    }

    void update(int a1, int b1, int a2, int b2, int val){
        update(0, 0, N - 1, a1, a2, b1, b2, val);
    }

    long long query(int a1, int b1, int a2, int b2){
        return query(0, 0, N - 1, a1, a2, b1, b2);
    }

};


segtree_x tree;

int N, M;
int t, a1, a2, b1, b2;

int main(){
    scanf("%d %d", &N, &M);
    tree.init(N);
    while(M--){
        scanf("%d %d %d %d %d", &t, &a1, &b1, &a2, &b2);
        --a1, --a2, --b1, --b2;
        if(t == 1){
            printf("%lld\n", tree.query(a1, b1, a2, b2));
        }
        else{
            scanf("%d", &t);
            tree.update(a1, b1, a2, b2, t);
        }
    }
}