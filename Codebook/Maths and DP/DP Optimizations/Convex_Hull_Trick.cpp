#include "bits/stdc++.h"
using namespace std;

const int N = 3e5 + 5;


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
	inline void add(long long a , long long b){
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


cht tree[N * 4];

inline void update_tree(int node, int l, int r, int qs, int qe, int m, int c){
	if(l > qe || r < qs) return;
	if(l >= qs && r <= qe){
		tree[node].add(-m, -c);
		return;
	}
	int mid = (l + r) >> 1;
	update_tree(node * 2, l, mid, qs, qe, m, c);
	update_tree(node * 2 + 1, mid + 1, r, qs, qe, m, c);
}

inline long long query_tree(int node, int l, int r, int pos, int x){
	long long cur = -tree[node].query(x);
	if(l != r && pos){
		int mid = (l + r) >> 1;
		if(mid >= pos) cur = max(cur, query_tree(node * 2, l, mid, pos, x));
		else cur = max(cur, query_tree(node * 2 + 1, mid + 1, r, pos, x));
	}
	return cur;
}

int t, q, x, id;
pair < int, int > query[N], range[N];

int main(){
	scanf("%d", &q);
	for(int i = 1; i <= q; i++){
		scanf("%d", &t);
		if(t == 1){
			scanf("%d %d", &query[i].first, &query[i].second);
			range[i] = {i, q};
		}
		else if(t == 2){
			scanf("%d", &id);
			range[id] = {range[id].first, i - 1};
		}
		else{
			scanf("%d", &x);
			range[i] = {x, q + 1}; 
		}
	}
	for(int i = 1; i <= q; i++){
		if(range[i].first == 0 && range[i].second == 0) continue;
		if(range[i].second <= q){
			update_tree(1, 1, q, range[i].first, range[i].second, query[i].first, query[i].second);
		}
		else{
			long long res = query_tree(1, 1, q, i, range[i].first);
			if(res == -5e18) puts("EMPTY SET");
			else printf("%lld\n", res);
		}
	}
}