#include <bits/stdc++.h>
using namespace std;

int n, k, x;

/*
 	A subarray of a[] is beautiful if the bitwise xor of all the elements in the subarray 
 	is at least k. Print count of such subarrays.
*/

struct node{
	node *lc, *rc;
	int leaves;
	node(node *_lc = NULL, node *_rc = NULL, int _leaves = 0){
		lc = _lc;
		rc = _lc;
		leaves = _leaves;
	}
	inline int val(node *x){
		return x ? x -> leaves : 0;
	}
	inline void create(node* &x){
		if(!x) x = new node();
	}
	inline int query(int pos, int prefix){
		if(pos == -1) return 0;
		int k_bit = k & (1 << pos);
		int p_bit = prefix & (1 << pos);
		int res = 0;
		if(!k_bit){
			if(!p_bit){
				res += val(rc);
				create(lc);
				res += lc -> query(pos - 1, prefix);
			}
			else{
				res += val(lc);
				create(rc);
				res += rc -> query(pos - 1, prefix);
			}
		}
		else{
			if(p_bit){
				create(lc);
				res += lc -> query(pos - 1, prefix);
			}
			else{
				create(rc);
				res += rc -> query(pos - 1, prefix);
			}
		}
		return res;
	}
	node *insert(int pos, long long prefix){
		if(pos == -1){
			++leaves;
			return this;
		}
		++leaves;
		int p_bit = prefix & (1 << pos);
		if(!p_bit){
			create(lc);
			lc = lc -> insert(pos - 1, prefix);
		}
		else{
			create(rc);
			rc = rc -> insert(pos - 1, prefix);
		}
		return this;
	}
};

node *trie = new node();

int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d %d", &n, &k);
	k--;
	long long res  = 0;
	int prefix_xor = 0;
	for(int i = 0; i < n; i++){
		scanf("%d", &x);
		prefix_xor ^= x;
		res += trie ->  query(30, prefix_xor) + (prefix_xor > k);
		trie = trie -> insert(30, prefix_xor);
	}
	printf("%lld\n", res);
}