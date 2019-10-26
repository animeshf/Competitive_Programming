#include "bits/stdc++.h"
using namespace std;

const int NMAX = 40010;

struct node{
	int left, right, pr, sz, rev;
};

// null should be NMAX-1 ideally
node tree[NMAX];
int N, null, root;

// Augment this to store values and other quanities.
inline int create_node(){
	tree[N].pr = rand();
	tree[N].sz = 1;
	tree[N].left = tree[N].right = null;
	tree[N].rev = 0;
	return N++;
}

// Update quantities here.
inline int upd(int x){
	int l = tree[x].left, r = tree[x].right;
	tree[x].sz = tree[l].sz + tree[r].sz + 1;
	return x;
}

// Swap left child and right child if it needs to be reversed.

// Update lazy flags here.
inline void down(int rt){
	if(!tree[rt].rev) return;
	swap(tree[rt].left, tree[rt].right);
	tree[rt].rev = 0;
	tree[tree[rt].left].rev ^= 1, tree[tree[rt].right].rev ^= 1;
}


/*
	Takes the treap rooted at "rt" and puts the k smallest elements 
	in it into sp.first, and the rest into sp.second
*/

inline pair < int, int > split(int rt, int k){
	if(rt >= null) return make_pair(null, null);
	down(rt);
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

// Standard Treap Merge : down() is called to initiate reverse when needed.

inline int merge(int l, int r){
	if(l >= null) return r;
	if(r >= null) return l;
	if(tree[l].pr > tree[r].pr){
		down(l);
		tree[l].right = merge(tree[l].right, r);
		return upd(l);
	}
	else{
		down(r);
		tree[r].left = merge(l, tree[r].left);
		return upd(r);
	}
}


/*
	Returns the index (node no. in treap) of the (k + 1)th smallest value
	in the treap. In this problem, index equals value so printing the index
	suffices. However, if array values are different, then you should maintain
	a parameter 'val' in each treap node and print treap[idx].val.

	Note that this is an implicit treap, hence here we are simply returning
	the (k + 1)th value in the array, since the treap is ordered based on array
	indices.
*/

inline int search(int rt, int k){
	if(rt >= null) return rt;
	down(rt);
	if(tree[tree[rt].left].sz > k){
		return search(tree[rt].left, k);
	}
	else{
		k -= tree[tree[rt].left].sz;
		if(!k) return rt;
		return search(tree[rt].right, k - 1);
	}
}

/*
	Suppose array[1..N] is present. reverse(i, j) takes subarray [i...j] of it (1 based)
	and reverses it.
*/

inline void reverse(int i, int j){
	pair < int, int > sp, sp2;
	sp  = split(root, j); // sp.first = arr[1..j], sp.second = arr[j + 1...N]
	sp2 = split(sp.first, i - 1); // sp2.first = arr[1..i - 1], sp2.second = arr[i..j]
	tree[sp2.second].rev = 1; // sp2.second needs to be reversed, mark it.
	sp.first = merge(sp2.first,sp2.second); // Now merge everything normally!
	root = merge(sp.first, sp.second); // Merge
}

/* 
    Insert element at position (i + 1) in the array i.e. after position (i)
    Here element value is not inputted since it's equal to index. 
    Look at other codes for utilising this function
*/

inline int insert(int i){
	pair < int, int > sp = split(root, i);
	int x = create_node();
	sp.first = merge(sp.first, x);
	return merge(sp.first, sp.second);
}

int main(){
	int i, j, n;
	scanf("%d", &n);
	null = 40001;
	root = null;
	while(n--) root = insert(N);
	while(true){
		scanf("%d",&n);
		if(n >= 2) break;
		if(n){
			scanf("%d %d", &i, &j);
			reverse(i, j);
		}
		else{
			scanf("%d", &i);
			int ans = search(root, i - 1);
			printf("%d\n", ans + 1); 
		}
	}
}
