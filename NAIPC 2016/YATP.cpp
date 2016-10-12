#include "bits/stdc++.h"
using namespace std;

struct convexHullTrick{
    struct Line{
        long long a , b;
        double xleft;
        bool type;
        Line(long long _a , long long _b){
            a = _a;
            b = _b;
            type = 0;
        }
        bool operator < (const Line &other) const{
            if(other.type){
                return xleft < other.xleft;
            }
            return a > other.a;
        }
    };
    inline double intersect(Line x , Line y){
        return 1.0 * (y.b - x.b) / (x.a - y.a);
    }
    multiset < Line > hull;
    convexHullTrick(){
        hull.clear();
    }
    typedef set < Line > :: iterator iter;
    inline bool hasLeft(iter node){
        return node != hull.begin();
    }
    inline bool hasRight(iter node){
        return node != prev(hull.end());
    }
    inline void updateBorder(iter node){
        if(hasRight(node)){
            Line temp = *next(node);
            hull.erase(hull.find(temp));
            temp.xleft = intersect(*node, temp);
            hull.insert(temp);
        }
        if(hasLeft(node)){
            Line temp = *node;
            temp.xleft = intersect(*prev(node), temp);
            hull.erase(node);
            hull.insert(temp);
        }
        else{
            Line temp = *node;
            hull.erase(node);
            temp.xleft = -1e18;
            hull.insert(temp);
        }
    }
    inline bool isBad(Line left , Line middle , Line right){
        return intersect(left, middle) > intersect(middle, right);
    }
    inline bool isBad(iter node){
        if(hasLeft(node) && hasRight(node)){
            return isBad(*prev(node), *node, *next(node));
        }
        return 0;
    }
    // add Line with equation y = (a * x + b)
    inline void addLine(long long a, long long b){
        Line temp = Line(a, b);
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
        if(isBad(it)){
            hull.erase(it);
            return;
        }
        while(hasLeft(it) && isBad(prev(it))){
            hull.erase(prev(it));
        }
        while(hasRight(it) && isBad(next(it))){
            hull.erase(next(it));
        }
        updateBorder(it);
    }
    // get minimum value of (m * x + c) for given x
    inline long long getBest(long long x){
        if(hull.empty()){
            return 5e18;
        }
        Line getBest(0, 0);
        getBest.xleft = x;
        getBest.type = 1;
        auto it = hull.lower_bound(getBest);
        it = prev(it);
        return it -> a * x + it -> b;
    }
};

const int N = 200000 + 50;
int n, a, b, c, sz;
int penalty[N], subtreeSize[N];
bool done[N];
long long bestValue[N];
vector < pair < int, int > > adj[N];
convexHullTrick dp;

inline void dfsPre(int node, int parent) {
    subtreeSize[node] = 1;
    for (auto next : adj[node]) {
        if ((next.first != parent) && (!done[next.first])) {
            dfsPre(next.first, node);
            subtreeSize[node] += subtreeSize[next.first];
        }
    }
}

inline int findCentroid(int node, int parent) {
    for (auto next : adj[node]) {
        if ((next.first != parent) && (!done[next.first])) {
            if (subtreeSize[next.first] > sz) {
                return findCentroid(next.first, node);
            }
        }
    }
    return node;
}

inline void exploreChild(int node, int parent, long long dist) {
    bestValue[node] = min(bestValue[node], dp.getBest(penalty[node]) + dist);
    for (auto next : adj[node]) {
        if ((next.first != parent) && (!done[next.first])) {
            exploreChild(next.first, node, dist + next.second);
        }
    }
}

inline void addChild(int node, int parent, long long dist) {
    dp.addLine(penalty[node], dist);
    for (auto next : adj[node]) {
        if ((next.first != parent) && (!done[next.first])) {
            addChild(next.first, node, dist + next.second);
        }
    }
}

inline void solve(int node) {
    dp.hull.clear();
    dp.addLine(penalty[node], 0);
    for (int i = 0; i < (int) adj[node].size(); i++) {
        pair < int, int > next = adj[node][i];
        if (!done[next.first]) {
            exploreChild(next.first, node, next.second);
            addChild(next.first, node, next.second);
        }
    }
    dp.hull.clear();
    reverse(adj[node].begin(), adj[node].end());
    dp.addLine(penalty[node], 0);
    for(int i = 0; i < (int) adj[node].size(); i++){
        pair < int, int > next = adj[node][i];
        if(!done[next.first]){
            exploreChild(next.first, node, next.second);
            addChild(next.first, node, next.second);
        }
    }
    bestValue[node] = min(bestValue[node], dp.getBest(penalty[node]));
}

inline void decompose(int node) {
    dfsPre(node , 0);
    sz = (subtreeSize[node] >> 1);
    int centroid = findCentroid(node, 0);
    solve(centroid);
    done[centroid] = 1;
    for (auto next : adj[centroid]) {
        if (!done[next.first]) {
            decompose(next.first);
        }
    }
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", penalty + i);
        bestValue[i] = penalty[i] * 1LL * penalty[i];
    }
    for (int i = 1; i < n; i++) {
        scanf("%d %d %d", &a, &b, &c);
        adj[a].emplace_back(make_pair(b, c));
        adj[b].emplace_back(make_pair(a, c));
    }
    decompose(1);
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        ans += bestValue[i];
    }
    printf("%lld\n", ans);
}
