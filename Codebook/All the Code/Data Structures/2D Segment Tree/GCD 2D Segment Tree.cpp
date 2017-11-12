#include <bits/stdc++.h>
using namespace std;

const int MAX = 275000;
int r, c, t;
vector < int > all_x, all_y;
int type[MAX], a1[MAX], b1[MAX], a2[MAX], b2[MAX];
long long val[MAX];

inline long long gcd(long long X, long long Y) {
    long long tmp;
    while (X != Y and Y != 0) {
        tmp = X;
        X = Y;
        Y = tmp % Y;
    }
    return X;
}

struct node{
	node *left, *right, *outer;
	long long val;
	node(){
		left = right = outer = NULL;
		val = 0LL;
	}
	inline void create(node* &x){
		if(!x) x = new node();
	}
	inline node* update_y(node* &left_x, node* &right_x, int l, int r, int y, long long v, bool isLeaf){
		if(l == r){
			if(isLeaf) val = v;
			else val = gcd((left_x) ? (left_x -> val) : (0), (right_x) ? (right_x -> val) : (0));
			return this;
		}
		int mid = (l + r) >> 1;
		if(mid >= y){
			create(left);
			left = left -> update_y((left_x) ? (left_x -> left) : (left_x), 
			(right_x) ? (right_x -> left) : (right_x), l, mid, y, v, isLeaf);
		}
		else{
			create(right);
			right = right -> update_y((left_x) ? (left_x -> right) : (left_x), 
			(right_x) ? (right_x -> right) : (right_x), mid + 1, r, y, v, isLeaf);
		}
		val = gcd((left) ? (left -> val) : (0), (right) ? (right -> val) : (0));
		return this;
	}
	inline node* update_x(int l, int r, int x, int y, long long v){
		create(outer), create(left), create(right);
		create(left -> outer), create(right -> outer);
		if(l == r){
			outer = outer -> update_y(left -> outer, right -> outer, 1, c, y, v, 1);
			return this;
		}
		int mid = (l + r) >> 1;
		if(mid >= x) left = left -> update_x(l, mid, x, y, v);
		else right = right -> update_x(mid + 1, r, x, y, v);
		outer = outer -> update_y(left -> outer, right -> outer, 1, c, y, v, 0);
		return this;
	}
	inline long long query_y(int l, int r, int b1, int b2){
		if(l > b2 or r < b1) return 0;
		if(l >= b1 and r <= b2) return val;
		int mid = (l + r) >> 1;
		return gcd((left) ? (left -> query_y(l, mid, b1, b2)) : (0), 
			      (right) ? (right -> query_y(mid + 1, r, b1, b2)) : (0));
	}
	inline long long query_x(int l, int r, int a1, int b1, int a2, int b2){
		if(l > a2 or r < a1) return 0;
		if(l >= a1 and r <= a2) return (outer) ? (outer -> query_y(1, c, b1, b2)) : (0);
		int mid = (l + r) >> 1;
		return gcd((left) ? (left -> query_x(l, mid, a1, b1, a2, b2)) : (0), 
				  (right) ? (right -> query_x(mid + 1, r, a1, b1, a2, b2)) : (0));
	}
};

inline int compressX(int x){
	return lower_bound(all_x.begin(), all_x.end(), x) - all_x.begin() + 1;
}

inline int compressY(int y){
	return lower_bound(all_y.begin(), all_y.end(), y) - all_y.begin() + 1;
}

node* root = new node();
int main(){

	scanf("%d %d %d\n", &r, &c, &t);
	for(int i = 1; i <= t; i++){
		scanf("%d ", &type[i]);
		if(type[i] == 1){
			scanf("%d %d %lld\n", &a1[i], &b1[i], &val[i]);
			a1[i]++, b1[i]++;
			all_x.push_back(a1[i]);
			all_y.push_back(b1[i]);
		}
		else{
			scanf("%d %d %d %d\n", &a1[i], &b1[i], &a2[i], &b2[i]);
			a1[i]++, b1[i]++, a2[i]++, b2[i]++;
			all_x.push_back(a1[i]), all_x.push_back(a2[i]);
			all_y.push_back(b1[i]), all_y.push_back(b2[i]);
		}
	}
	
	sort(all_x.begin(), all_x.end());
	sort(all_y.begin(), all_y.end());
	all_x.resize(unique(all_x.begin(), all_x.end()) - all_x.begin());
	all_y.resize(unique(all_y.begin(), all_y.end()) - all_y.begin());
	r = all_x.size(), c = all_y.size();

	for(int i = 1; i <= t; i++){
		if(type[i] == 1){
			a1[i] = compressX(a1[i]), b1[i] = compressY(b1[i]);
			root  = root -> update_x(1, r, a1[i], b1[i], val[i]);
		}
		else{
			a1[i] = compressX(a1[i]), a2[i] = compressX(a2[i]);
			b1[i] = compressY(b1[i]), b2[i] = compressY(b2[i]);
			printf("%lld\n", root -> query_x(1, r, a1[i], b1[i], a2[i], b2[i]));
		}
	}
}
