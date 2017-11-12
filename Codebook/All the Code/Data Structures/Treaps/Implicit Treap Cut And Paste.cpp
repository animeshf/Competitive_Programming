// Hackerrank Zalando Codesprint

#include "bits/stdc++.h"
using namespace std;

struct node{
	int left, right, pr, sz, val;
};

const int MAX = 100000 + 50;

node tree[MAX];
int N, root, null = 100000 + 5;

inline int create_node(int val){
	tree[N].pr  = rand();
	tree[N].sz  = 1;
	tree[N].val = val;
	tree[N].left = tree[N].right = null;
	return N++;
}

// Update Treap Information (when you're moving it around)
inline int upd(int x){
	int l = tree[x].left, r = tree[x].right;
	tree[x].sz = tree[l].sz + 1 + tree[r].sz;
	return x;
}

/*
	Takes the treap rooted at "rt" and puts the k smallest elements 
	in it into sp.first, and the rest into sp.second
*/

inline pair < int, int > split(int rt, int k){
	if(rt >= null) return make_pair(null, null);
	pair < int, int > sp;
	if(tree[tree[rt].left].sz >= k){
		sp = split(tree[rt].left, k);
		tree[rt].left = sp.second;
		sp.second = upd(rt);
		return sp;
	}
	else{
		k -= tree[tree[rt].left].sz;
		sp = split(tree[rt].right, k - 1);
		tree[rt].right = sp.first;
		sp.first = upd(rt);
		return sp;
	}
}

// Merges treaps (l) and (r)
inline int merge(int l, int r){
	if(l >= null) return r;
	if(r >= null) return l;
	if(tree[l].pr > tree[r].pr){
		tree[l].right = merge(tree[l].right,r);
		return upd(l);
	}
	else{
		tree[r].left = merge(l,tree[r].left);
		return upd(r);
	}
}

/*
	Inserts element at position (i + 1) i.e after position (i) in the treap
*/

inline int insert(int i){
	pair < int, int > sp = split(root, i);
	int val; scanf("%d", &val);
	int x = create_node(val);
	sp.first = merge(sp.first,x);
	return merge(sp.first, sp.second);
}

/*
	Let arr[1...N] be the current array :- 
	It looks like arr[1...i - 1] -> arr[i...j] -> arr[j + 1...N]
	work(i, j) takes subarray arr[i...j], cuts it, and pastes it at the beginning
	So the new array looks like arr[i..j] -> arr[1, i - 1] -> arr[j + 1..n]
*/

inline void work(int i, int j){
	pair < int, int > sp, sp2;
	sp = split(root, j);
	sp2 = split(sp.first, i - 1);
	root = merge(merge(sp2.second, sp2.first), sp.second);
}

/*
	Prints the in-order traversal of the treap i.e the sorted order of elements.
*/

inline void print(int idx){
	upd(idx);
	if(idx >= null) return;
	print(tree[idx].left);
	cout << tree[idx].val << ' ';
	print(tree[idx].right);
}

int main(){
	int n, q, i, j;
	scanf("%d", &n);
	root = null;
	while(n--) root = insert(N);
	scanf("%d", &q);
	while(q--){
		scanf("%d %d", &i, &j);
		work(i, j);
	}
	print(root);
}
