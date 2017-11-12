/*

SPOJ TREAP

I k : Insert k into S, if k is not in S

D k : Delete k from S, if k is in S

N i j : Print min{abs(S[x] - S[y]) | i <= x, y <= j} or -1 if the range has 1 element

X i j : Print max{abs(S[x] - S[y]) | i <= x, y <= j} or -1 if the range has 1 element

*/

#include <bits/stdc++.h>
#define pii pair < int, int >
using namespace std;

const int MAXQ  = 2e5 + 5;
const int INF   = 1e9 + 9;
const int EMPTY = MAXQ - 1;

int N, Q, ROOT;
map < int, int > is_present;
char cmd[1];

struct treap_node{
	int val, pri, siz;
	int lc, rc;
	int maxv, minv, min_diff; 
}treap[MAXQ];

inline int create_node(int val){
	N = N + 1;
	treap[N].val = val;
	treap[N].pri = rand();
	treap[N].siz = 1;
	treap[N].lc  = treap[N].rc = EMPTY;
	treap[N].maxv = treap[N].minv = val;
	treap[N].min_diff = INF;
	return N;
}

inline void refresh(int idx){
	treap[idx].siz = 1;
	treap[idx].maxv = treap[idx].minv = treap[idx].val;
	treap[idx].min_diff = INF;
	if(treap[idx].lc != EMPTY){
		treap[idx].siz += treap[treap[idx].lc].siz;
		treap[idx].maxv = max(treap[idx].maxv, treap[treap[idx].lc].maxv);
		treap[idx].minv = min(treap[idx].minv, treap[treap[idx].lc].minv);
		treap[idx].min_diff = min(treap[idx].min_diff, treap[treap[idx].lc].min_diff);
		treap[idx].min_diff = min(treap[idx].min_diff, treap[idx].val - treap[treap[idx].lc].maxv);
	}
	if(treap[idx].rc != EMPTY){
		treap[idx].siz += treap[treap[idx].rc].siz;
		treap[idx].maxv = max(treap[idx].maxv, treap[treap[idx].rc].maxv);
		treap[idx].minv = min(treap[idx].minv, treap[treap[idx].rc].minv);
		treap[idx].min_diff = min(treap[idx].min_diff, treap[treap[idx].rc].min_diff);
		treap[idx].min_diff = min(treap[idx].min_diff, treap[treap[idx].rc].minv - treap[idx].val);
	}
}

inline pii split(int idx, int key){
	pii parts = pii(EMPTY, EMPTY);
	if(idx == EMPTY) return parts;
	if(treap[idx].val <= key){
		parts = split(treap[idx].rc, key);
		treap[idx].rc = parts.first;
		parts.first = idx;
		refresh(idx);
	}
	else{
		parts = split(treap[idx].lc, key);
		treap[idx].lc = parts.second;
		parts.second = idx;
		refresh(idx);
	}
	return parts;
}

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

inline int insert(int val){
	int idx = create_node(val);
	pii parts = split(ROOT, val - 1);
	return merge(merge(parts.first, idx), parts.second);
}

inline int erase(int val){
	pii y = split(ROOT, val);
	pii x = split(y.first, val - 1);
	return merge(x.first, y.second);
}

inline int get_kth(int idx, int k){
	if(treap[treap[idx].lc].siz >= k)
		return get_kth(treap[idx].lc, k);
	else{
		k -= treap[treap[idx].lc].siz;
		if(k == 1) return idx;
		return get_kth(treap[idx].rc, k - 1);
	}
}

inline int query(int val_i, int val_j){
	pii y = split(ROOT, val_j);
	pii x = split(y.first, val_i - 1);
	int res = treap[x.second].min_diff;
	ROOT = merge(merge(x.first, x.second), y.second);
	return res;
}

int main(){
	scanf("%d", &Q);
	ROOT = EMPTY;
	while(Q--){
		scanf("%s", cmd);
		int k, i, j;
		if(cmd[0] == 'I'){
			scanf("%d", &k);
			if(!is_present[k]){
				ROOT = insert(k);
				is_present[k] = 1;
			}
		}
		else if(cmd[0] == 'D'){
			scanf("%d", &k);
			if(is_present[k]){
				ROOT = erase(k);
				is_present[k] = 0;
			}
		}
		else if(cmd[0] == 'N'){
			scanf("%d %d", &i, &j);
			i++, j++;
			i = get_kth(ROOT, i), j = get_kth(ROOT, j);
			if(i == j) printf("-1\n");
			else printf("%d\n", query(treap[i].val, treap[j].val));
		}
		else{
			scanf("%d %d", &i, &j);
			i++, j++;
			i = get_kth(ROOT, i), j = get_kth(ROOT, j);
			if(i == j) printf("-1\n");
			else printf("%d\n", treap[j].val - treap[i].val);
		}
	}
}
