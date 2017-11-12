/*
        SPOJ RaceTime
	1) Update A[i] = X for given i and X
	2) Print # of i such that L <= i <= R and A[i] <= X, for given L, R and X
*/

#include <bits/stdc++.h>
#define pii pair < int, int >
using namespace std;

const int MAXN  = 100005;
const int MAXQ  = 50005;
const int LN    = 20;
const int EMPTY = (MAXN + MAXQ) * LN - 1;

int N, n, q, arr[MAXN];
int treap_roots[MAXN];

struct treap_node{
	int val, pri, siz, lc, rc;
}treap[(MAXN + MAXQ) * LN];

inline int create_node(int val){
	N = N + 1;
	treap[N].val = val;
	treap[N].pri = rand();
	treap[N].siz = 1;
	treap[N].lc = treap[N].rc = EMPTY;
	return N;
}

inline void refresh(int root){
	treap[root].siz = treap[treap[root].lc].siz + 1 + treap[treap[root].rc].siz;
}

/*
	splits treap into two treaps parts.first and parts.second such that
	parts.first comprises all elements with val <= key and parts.second comprises
	all elements with val > key.
*/

inline pii split(int root, int key){
	pii parts = pii(EMPTY, EMPTY);
	if(root == EMPTY) return parts;
	if(treap[root].val <= key){
		parts = split(treap[root].rc, key);
		treap[root].rc = parts.first;
		refresh(root);
		parts.first = root;
		return parts;
	}
	else{
		parts = split(treap[root].lc, key);
		treap[root].lc = parts.second;
		refresh(root);
		parts.second = root;
		return parts;
	}
}

/*
	Merge treaps l, r.
	Note largest key in l must be <= smallest key in r
*/

inline int merge(int l, int r){
	if(l == EMPTY) return r;
	if(r == EMPTY) return l;
	if(treap[l].pri > treap[r].pri){
		treap[l].rc = merge(treap[l].rc, r);
		refresh(l);
		return l;
	}
	else{
		treap[r].lc = merge(l, treap[r].lc);
		refresh(r);
		return r;
	}
}

/*
	Insert treap_node named 'add' with value 'treap[add].val' into 
	treap rooted at 'root'
*/

inline int insert(int root, int add){
	if(root == EMPTY) return add;
	pii parts = split(root, treap[add].val - 1);
	return merge(merge(parts.first, add), parts.second);
}

/*
	Remove 'rem_value' from treap rooted at 'root'
*/

inline int erase(int root, int rem_value){
	if(root == EMPTY) return EMPTY;
	if(treap[root].val == rem_value){
		return merge(treap[root].lc, treap[root].rc);
	}
	if(treap[root].val > rem_value){
		treap[root].lc = erase(treap[root].lc, rem_value);
		refresh(root);
		return root;
	}
	else{
		treap[root].rc = erase(treap[root].rc, rem_value);
		refresh(root);
		return root;
	}
}

/*
	Returns # of elements in the treap rooted at 'root' that
	has a value <= k.
*/

inline int query_k(int root, int k){
	if(root == EMPTY) return 0;
	if(treap[root].val <= k){
		return treap[treap[root].lc].siz + 1 + query_k(treap[root].rc, k);
	}
	else{
		return query_k(treap[root].lc, k);
	}
}

/*
	Maintain a BIT in which each node is a TREAP.
	treap_roots[] denotes the roots of the treaps.
*/

inline void update(int idx, int val, int type){
	for(int i = idx; i <= n; i += i & -i){
		if(type) treap_roots[i] = insert(treap_roots[i], create_node(val));
		else     treap_roots[i] = erase(treap_roots[i], val);
	}
}

inline int query(int idx, int k){
	int res = 0;
	for(int i = idx; i > 0; i -= i & -i){
		res += query_k(treap_roots[i], k);
	}
	return res;
}

int main(){
	scanf("%d %d", &n, &q);
	for(int i = 1; i <= n; i++){
		treap_roots[i] = EMPTY;
	}
	for(int i = 1; i <= n; i++){
		scanf("%d", arr + i);
		update(i, arr[i], 1);
	}
	char buf[1];
	while(q--){
		scanf("%s", buf);
		if(buf[0] == 'M'){
			int i, x;
			scanf("%d %d", &i, &x);
			update(i, arr[i], 0);
			arr[i] = x;
			update(i, arr[i], 1);
		}
		else{
			int st, en, x;
			scanf("%d %d %d", &st, &en, &x);
			printf("%d\n", query(en, x) - query(st - 1, x));
		}
	}
}
