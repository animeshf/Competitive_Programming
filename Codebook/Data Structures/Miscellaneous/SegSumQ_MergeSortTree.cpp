// Codechef snackdown elimination 2016
#include "bits/stdc++.h"
using namespace std;

const int N = 1e5 + 5;
const double INF = 1e18;

int n, q;
long long a[N], b[N];
vector < pair < long long, long long > > tree[2][N * 4];

// Sort by a[i] / b[i]
inline bool compare(pair < long long, long long > x, pair < long long, long long > y){
	double valx, valy;
	if(x.second != 0) valx  = x.first * 1.0 / x.second;
    else valx = INF;
    if(y.second != 0) valy  = y.first * 1.0 / y.second;
    else valy = INF;
    return (valx >= valy);
}

inline pair < long long, long long > query(int node, int l, int r, int qs, int qe, long long c, long long d){
	if(l > qe || r < qs) return {0, 0};
	if(l >= qs && r <= qe){
		int l = 0, r = tree[0][node].size() - 1;
		int idx = 0;
		while(l <= r){
			int mid = (l + r) / 2;
			if(compare(tree[0][node][mid], {c, d})){
				idx = mid;
				l = mid + 1;
			}
			else r = mid - 1;
		}
		if(compare(tree[0][node][idx], {c, d})) return tree[1][node][idx];
        return {0, 0};
	}
	int mid = (l + r) >> 1;
	pair < long long, long long > x = query(node * 2, l, mid, qs, qe, c, d);
	pair < long long, long long > y = query(node * 2 + 1, mid + 1, r, qs, qe, c, d);
	return {x.first + y.first, x.second + y.second};
}


inline void build(int node, int l, int r){
	if(l == r){
		tree[0][node].push_back({a[l], b[l]});
		tree[1][node].push_back({a[l], b[l]});
		return;
	}
	int mid = (l + r) >> 1;
	build(node * 2, l, mid);
	build(node * 2 + 1, mid + 1, r);
	tree[0][node].resize(tree[0][node * 2].size() + tree[0][node * 2 + 1].size());
	merge(tree[0][node * 2].begin(), tree[0][node * 2].end(), tree[0][node * 2 + 1].begin(),
		  tree[0][node * 2 + 1].end(), tree[0][node].begin(), compare);
	pair < long long, long long > prefix_sum = {0, 0};
	for(int i = 0; i < (int) tree[0][node].size(); i++){
		prefix_sum.first  += tree[0][node][i].first;
		prefix_sum.second += tree[0][node][i].second;
		tree[1][node].push_back(prefix_sum);
	}
}

int main(){
	scanf("%d", &n);
	for(int i = 1; i <= n; i++) scanf("%lld", a + i);
	for(int i = 1; i <= n; i++) scanf("%lld", b + i);
	build(1, 1, n);
	scanf("%d", &q);
	while(q--){
		long long l, r, c, d;
		scanf("%lld %lld %lld %lld", &l, &r, &c, &d);
		pair < long long, long long > res = query(1, 1, n, l, r, d, c);
		fflush(stdout);
		printf("%lld\n", res.first * 1LL * c - res.second * 1LL * d);
		fflush(stdout);
	}
}