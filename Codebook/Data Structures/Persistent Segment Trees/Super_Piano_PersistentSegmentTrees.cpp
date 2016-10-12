/*
        WCIPEG Problem
	Prints sum of K maximum sum subarrays, each of L <= length <= R
	Array has negative elements as well.

	Add f[i]th best subarray starting at index (i) of valid length for each (i)
	into a priority queue. Initially, let f[i] = 1 for all (i). 
	Pop the best value from the priority queue k times, increment f[i] each time
	and add a new value to it after each pop.
*/

#include <bits/stdc++.h>
using namespace std;

const int MAX = 500005;
const int INF = 1000000000;

struct node{
	node *lc, *rc;
	int val;
	node(node *x = NULL, node *y = NULL, int v = 0){
		lc = x, rc = y, val = v;
	}
	inline void create(node *&x){
		if(!x) x = new node();
	}
	inline int sum(node *x){
		return (x) ? (x -> val) : (0);
	}
	inline node *insert(int l, int r, int value){
		node *nw = new node();
		if(l == r){
			nw -> val = val + 1;
			return nw;
		}
		int mid = (l + r) >> 1;
		if(mid >= value){
			nw -> rc = rc;
			create(lc);
			nw -> lc = lc -> insert(l, mid, value);
		}
		else{
			nw -> lc = lc;
			create(rc);
			nw -> rc = rc -> insert(mid + 1, r, value);
		}
		nw -> val = sum(nw -> lc) + sum(nw -> rc);
		return nw;
	}
	inline int query(node *r1, node *r2, int l, int r, int k){
		if(l == r) return r;
		int goRight = sum(r1 -> rc) - sum(r2 -> rc);
		int mid = (l + r) >> 1;
		if(goRight >= k){
			create(r1 -> rc), create(r2 -> rc);
			return query(r1 -> rc, r2 -> rc, mid + 1, r, k);
		}
		else{
			create(r1 -> lc), create(r2 -> lc);
			return query(r1 -> lc, r2 -> lc, l, mid, k - goRight);
		}
	}
};

node *root[MAX], *dummy;
map < int, int > compress;
int n, k, l, r, lim;
int arr[MAX], f[MAX], original[MAX];

inline int get(int i, int j){
	if(i + l - 1 > n || j > r - l + 1) return -INF;
	return original[dummy -> query(root[min(i + r - 1, n)], root[i + l - 2], 1, lim, j)] - arr[i - 1];
}

int main(){
	scanf("%d %d %d %d", &n, &k, &l, &r);
	compress[-INF];
	for(int i = 1; i <= n; i++){
		scanf("%d", arr + i);
		arr[i] += arr[i - 1];
		compress[arr[i]];
	}
	for(auto &it : compress) it.second = ++lim;
	for(auto  it : compress) original[it.second] = it.first;
	root[0] = dummy = new node();
	for(int i = 1; i <= n; i++){
		root[i] = root[i - 1] -> insert(1, lim, compress[arr[i]]);
	}
	priority_queue < pair < int, int > > sums;
	for(int i = 1; i <= n; i++){
		sums.push({get(i, ++f[i]), i});
	}
	long long res = 0;
	while(k--){
		res += (sums.top().first);
		sums.push({get(sums.top().second, ++f[sums.top().second]), sums.top().second});
		sums.pop();
	}
	printf("%lld\n", res);
}
