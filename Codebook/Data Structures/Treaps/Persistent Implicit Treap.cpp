/*
	UVA Online Judge - Version Controlled IDE
*/

#include "bits/stdc++.h"
using namespace std;

const int NMAX = 10000000 + 50;
const int QMAX = 50000 + 50;
const int LMAX = 100 + 10;

struct node{
	int lft, rht, pr, sz, time;
	char ch;
};

int N, Q, cur_time;
int roots[QMAX];
char str[LMAX];
node T[NMAX];

const int null = (10000000) + 5;

inline int create_node(char c){
	assert(N < null);
	T[N].lft = T[N].rht = null;
	T[N].sz = 1;
	T[N].pr = rand();
	T[N].ch = c;
	T[N].time = cur_time;
	return N++;
}

/*
	For each insert and delete operation, create a new node instead of
	working with the original nodes (Persistence).
	This new node will have same data as the original. (except the time parameter)

	Since the expected height of the treap is log N, we will not create more than
	log N nodes for every split() operation.
*/

inline int copy(int x){ // Path Copying
	if(T[x].time == cur_time) return x;
	T[N] = T[x];
	T[N].time = cur_time;
	return N++;
}

// Update Treap Information
inline int upd(int x){
	T[x].sz = T[T[x].lft].sz + T[T[x].rht].sz + 1;
	return x;
}

/*
	Takes the treap rooted at "rt" and puts the k smallest elements
	in it into sp.first, and the rest into sp.second
*/

inline pair < int, int > split(int rt, int k, bool persistence){
	if(rt >= null) return make_pair(null,null);
	int l, x, r;
	pair < int, int > sp;
	// Do not destroy the original state if persistence is needed.
	if(persistence) rt = copy(rt); // Work with a copy of rt
	l = T[rt].lft, r = T[rt].rht;
	if(T[l].sz >= k){
		sp = split(l, k, persistence);
		T[rt].lft = sp.second;
		sp.second = upd(rt);
		return sp;
	}
	else{
		k -= T[l].sz;
		--k;
		sp = split(r, k, persistence);
		T[rt].rht = sp.first;
		sp.first = upd(rt);
		return sp;
	}
}

// Standard Treap Merge : Note that largest element of l <= smallest of r
inline int merge(int l, int r){
	if(l >= null) return r;
	if(r >= null) return l;
	if(T[l].pr > T[r].pr){
		T[l].rht = merge(T[l].rht, r);
		return upd(l);
	}
	else{
		T[r].lft = merge(l, T[r].lft);
		return upd(r);
	}
}

// Build Treap with n nodes, filled with characters in str[]
inline int build_treap(char *str, int n){
	int rt = null, i;
	for(i = 0; i < n; ++i)
		rt = merge(rt, create_node(str[i]));
	return rt;
}

// Insert required substring
inline int insert(int rt, int i, int n, char *str){
	int x = build_treap(str, n);
	pair < int, int > sp;
	sp = split(rt, i, 1);
	x = merge(sp.first, x);
	x = merge(x, sp.second);
	return x;
}

// Delete required substring
inline int remove(int rt, int i, int c){
	pair < int, int > sp1, sp2;
	sp1 = split(rt, i, 1);
	sp2 = split(sp1.second, c, 1);
	return merge(sp1.first, sp2.second);
}

// Prints in-order traversal of treap
inline int print_word(int rt){
	if(rt >= null) return 0;
	int ct = print_word(T[rt].lft);
	putchar(T[rt].ch);
	ct += (T[rt].ch == 'c');
	return ct + print_word(T[rt].rht);
}

// Print the required substring
inline int print(int rt, int i, int c){
	pair < int, int > sp1, sp2;
	sp1 = split(rt, i, 0);
	sp2 = split(sp1.second, c, 0);
	int ct = print_word(sp2.first);
	sp1.second = merge(sp2.first, sp2.second);
	merge(sp1.first, sp1.second);
	return ct;
}

int main(){
	scanf("%d", &Q);
	int i, t, c, ct, x;
	ct = 0;
	roots[cur_time++] = null;
	while(Q--){
		scanf("%d", &t);
		if(t == 1){
			scanf("%d %s", &i, str);
			i -= ct;
			roots[cur_time] = insert(roots[cur_time - 1], i, strlen(str), str);
			++cur_time;
		}
		else if(t == 2){
			scanf("%d %d", &i, &c);
			i -= ct, c -= ct;
			--i;
			roots[cur_time] = remove(roots[cur_time - 1], i, c);
			++cur_time;
		}
		else{
			scanf("%d %d %d", &x, &i, &c);
			x -= ct, i -= ct, c -= ct;
			--i;
			ct += print(roots[x], i, c);
			printf("\n");
		}
	}
}
