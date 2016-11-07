
/*
	l r k : Let S denote the sorted (in increasing order) set of elements of array A with 
	its indices between l and r. Note that set S contains distinct elements.
	You need to find kth number in it. 
	If such a number does not exist, i.e. the S has less than k elements, output -1.
*/

#include <bits/stdc++.h>
using namespace std;
  
const int N = 100005;
const int L = 1;
const int R = 100000;
 
int n, q, arr[N], orig[N], latest[N];
vector < int > values;

struct node_y{ // Contains latest occurence positions of values.
	node_y *lc, *rc;
	int val;
	node_y(node_y *l = NULL, node_y *r = NULL, int v = 0){
		lc = l, rc = r, val = v;
	}
	inline void create(node_y* &x){
		if(!x) x = new node_y();
	}
	inline int sum(node_y *x){
		return (x) ? (x -> val) : (0);
	}
	inline node_y *update_y(int l, int r, int pos, int add){
		node_y *nw = new node_y();
		if(l == r){
			nw -> val = (val + add);
			return nw;
		}
		int mid = (l + r) >> 1;
		if(mid >= pos){
			nw -> rc = rc;
			create(lc);
			nw -> lc = lc -> update_y(l, mid, pos, add);
		}
		else{
			nw -> lc = lc;
			create(rc);
			nw -> rc = rc -> update_y(mid + 1, r, pos, add);
		}
		nw -> val = sum(nw -> lc) + sum(nw -> rc);
		return nw;
	}
	inline int query_y(int l, int r, int qs, int qe){
		if(l > qe || r < qs) return 0;
		if(l >= qs && r <= qe) return val;
		int mid = (l + r) >> 1, res = 0;
		if(lc) res += lc -> query_y(l, mid, qs, qe);
		if(rc) res += rc -> query_y(mid + 1, r, qs, qe);
		return res;
	}
};

/*
	It's a persistent segment tree in which every node is also another
	persistent segment tree. The base tree is indexed by values of nodes.
	Thus, in a node corresponding to range [min, max], we can query 
	"how many values in [min, max] have their latest occurence >= x" by
	querying the 2nd-dimenion-persistent-segment-tree. This allows us
	to binary search the answer.
*/
 
struct node_x{ // Contains the numbers (We will binary search to find kth element)
	node_x *lc, *rc;
	node_y *outer;
	int val;
	node_x(node_x *l = NULL, node_x *r = NULL, int v = 0){
		lc = l, rc = r, val = v;
		outer = new node_y();
	}
	inline void create(node_x* &x){
		if(!x) x = new node_x();
	}
	inline node_x *update_x(int l, int r, int value, int prev_occ, int curr_occ){
		node_x *nw = new node_x();
		if(!outer) outer = new node_y();
		if(prev_occ){
			nw -> outer = outer -> update_y(1, n, prev_occ, -1);
			nw -> outer = nw -> outer -> update_y(1, n, curr_occ, 1);
		}
		else{
			nw -> outer = outer -> update_y(1, n, curr_occ, 1);
		}
		if(l == r) return nw;
		int mid = (l + r) >> 1;
		if(mid >= value){
			nw -> rc = rc;
			create(lc);
			nw -> lc = lc -> update_x(l, mid, value, prev_occ, curr_occ);
		}
		else{
			nw -> lc = lc;
			create(rc);
			nw -> rc = rc -> update_x(mid + 1, r, value, prev_occ, curr_occ);
		}
		return nw;
	}	
	inline int query_x(int l, int r, int qs, int qe, int k){
		if(l == r) return l;
		if(l == L && r == R){
			if(!outer) return -1;
			if(outer -> query_y(1, n, qs, qe) < k) return -1;
		}
		int mid = (l + r) >> 1, goLeft = 0;
		if(lc && lc -> outer) goLeft = lc -> outer -> query_y(1, n, qs, qe);
		if(goLeft >= k) return lc -> query_x(l, mid, qs, qe, k);
		else return rc -> query_x(mid + 1, r, qs, qe, k - goLeft);
	}
};
 
node_x *root[N];
 
int main(){
	scanf("%d %d", &n, &q);
	for(int i = 1; i <= n; i++){
		scanf("%d", arr + i);
		values.push_back(arr[i]);
	}
	
	sort(values.begin(), values.end());
	values.resize(unique(values.begin(), values.end()) - values.begin());
 
	for(int i = 1; i <= n; i++){
		int copy = arr[i];
		arr[i] = lower_bound(values.begin(), values.end(), arr[i]) - values.begin() + 1;
		orig[arr[i]] = copy;
	}	
 
	root[0] = new node_x();
 
	for(int i = 1; i <= n; i++){
		int prev_occ = latest[arr[i]];
		latest[arr[i]] = i;
		root[i] = root[i - 1] -> update_x(L, R, arr[i], prev_occ, i);
	}
 
	int ans = 0;
	while(q--){
		int a, b, c, d, k;
		scanf("%d %d %d %d %d", &a, &b, &c, &d, &k);
		
		int l = ((a * 1LL * max(0, ans) + b) % n) + 1;
		int r = ((c * 1LL * max(0, ans) + d) % n) + 1;
		if(l > r) swap(l, r);
 
		ans = root[r] -> query_x(L, R, l, r, k);
		if(ans != -1) ans = orig[ans];
		printf("%d\n", ans);
	}
} 
