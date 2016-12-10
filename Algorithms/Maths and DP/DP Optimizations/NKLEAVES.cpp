// SPOJ NKLEAVES
#include "bits/stdc++.h"
using namespace std;

const int N = 100000 + 5;
const int K = 10 + 5;

int n, k;
int weights[N];
long long sumWeights[N], dp[N][K];

/*
    If there is just one pile, we need to pay Cost C = w2 + 2w3 + 3w4..... + (n - 1)wn.
    Now if we create another pile, say at position (4), we deduct off
    3 * (w4 + w5 + w6.... + wn) from our total cost. Suppose we then create
    another pile at position (7), we can deduct off (7 - 4) * (w7... + wn)
    again from our total cost. So we transform this minimization problem to
    maximization of the deductions.

    dp[i][p] = Maximum you can deduct by making (p) piles from (i..n),
    with the pth pile at position i. What we seek is dp[1][k].

    This represents the naive O(n^2 k) DP.
    for (int i = 1; i <= n; i++) {
        dp[i][1] = 0;
    }
    for (int p = 2; p <= k; p++) {
        for (int i = n; i >= 1; i--) {
            for (int j = i + 1; j <= n; j++) {
                dp[i][p] = max(dp[i][p], (j - i) * sumWeights[j] + dp[j][p - 1]);
            }
        }
    }
    Optimize it with CHT to obtain O(nklog n) complexity.
*/

/** ----- Minimum Convex Hull Trick Template ------- */

struct cht{
    struct line{
        long long a , b;
        double xleft;
        bool type;
        line(long long _a , long long _b){
            a = _a;
            b = _b;
            type = 0;
        }
        bool operator < (const line &other) const{
            if(other.type){
                return xleft < other.xleft;
            }
            return a > other.a;
        }
    };
    inline double intersect(line x , line y){
        return 1.0 * (y.b - x.b) / (x.a - y.a);
    }
    multiset < line > hull;
    cht(){
        hull.clear();
    }
    typedef set < line > :: iterator iter;
    inline bool has_left(iter node){
        return node != hull.begin();
    }
    inline bool has_right(iter node){
        return node != prev(hull.end());
    }
    inline void update_border(iter node){
        if(has_right(node)){
            line temp = *next(node);
            hull.erase(hull.find(temp));
            temp.xleft = intersect(*node, temp);
            hull.insert(temp);
        }
        if(has_left(node)){
            line temp = *node;
            temp.xleft = intersect(*prev(node), temp);
            hull.erase(node);
            hull.insert(temp);
        }
        else{
            line temp = *node;
            hull.erase(node);
            temp.xleft = -1e18;
            hull.insert(temp);
        }
    }
    inline bool useless(line left , line middle , line right){
        return intersect(left, middle) > intersect(middle, right);
    }
    inline bool useless(iter node){
        if(has_left(node) && has_right(node)){
            return useless(*prev(node), *node, *next(node));
        }
        return 0;
    }
    // add line with equation y = (a * x + b)
    inline void add(long long a, long long b){
        a = -a, b = -b;
        line temp = line(a, b);
        auto it = hull.lower_bound(temp);
        if(it != hull.end() && it -> a == a){
            if(it -> b > b){
                hull.erase(it);
            }
            else{
                return;
            }
        }
        hull.insert(temp);
        it = hull.find(temp);
        if(useless(it)){
            hull.erase(it);
            return;
        }
        while(has_left(it) && useless(prev(it))){
            hull.erase(prev(it));
        }
        while(has_right(it) && useless(next(it))){
            hull.erase(next(it));
        }
        update_border(it);
    }
    // get minimum value of (m * x + c) for given x
    inline long long query(long long x){
        if(hull.empty()){
            return 5e18;
        }
        line query(0, 0);
        query.xleft = x;
        query.type = 1;
        auto it = hull.lower_bound(query);
        it = prev(it);
        return it -> a * x + it -> b;
    }
};


/** ----------- End of Template ------------ **/

int main() {
    scanf("%d %d", &n, &k);
    for (int i = 1; i <= n; i++) {
        scanf("%d", weights + i);
    }
    sumWeights[n] = weights[n];
    for (int i = n - 1; i >= 1; i--) {
        sumWeights[i] = weights[i] + sumWeights[i + 1];
    }
    for (int i = 1; i <= n; i++) {
        dp[i][1] = 0;
    }
    for (int p = 2; p <= k; p++) {
        cht lines;
        for (int i = n; i >= 1; i--) {
            dp[i][p] = -(lines.query(i));
            lines.add(-sumWeights[i], i * sumWeights[i] + dp[i][p - 1]);
        }
    }
    long long res = 0;
    for (int i = 1; i <= n; i++) {
        res += sumWeights[i];
    }
    res = res - sumWeights[1] - dp[1][k];
    printf("%lld\n", res);
}