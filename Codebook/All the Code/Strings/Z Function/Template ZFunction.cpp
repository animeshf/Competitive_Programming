// POI - Template

/*
	Print smallest length string which can be "stamped" multiple times to get 
	Target String T

	Suppose T = ababbababbabababbabababbababbaba
	Ans = 8, Ans_String = ababbaba

	The answer string will always be a prefix of T. Hence, you can compute z[i] for
	each index (i). Now you can do an offline algorithm to solve the problem.
*/

#include <bits/stdc++.h>
using namespace std;

const int N = 5e5 + 50;
char str[N];
int n, z[N];
vector < pair < int, int > > values;

struct node{
	int mn, mx, ret;
	node(int _mn = -1, int _mx = -1, int _ret = -INT_MAX){
		mn  = _mn;
		mx  = _mx;
		ret = _ret;
	}
}tree[N * 4];

inline node merge(node x, node y){
	if(x.mn == -1 && x.mx == -1) return y;
	if(y.mn == -1 && y.mx == -1) return x;
	return node(x.mn, y.mx, max(x.ret, max(y.ret, y.mn - x.mx)));
}

inline void update(int i, int l, int r, int pos){
	if(l == r){
		tree[i].mn = tree[i].mx = l;
		return;
	}
	int mid = l + r >> 1;
	if(mid >= pos) update(i * 2, l, mid, pos);
	else update(i * 2 + 1, mid + 1, r, pos);
	tree[i] = merge(tree[i * 2], tree[i * 2 + 1]);
}

inline void z_function(){
	n = strlen(str);
	for(int i = 1, l = 0, r = 0; i < n; i++){
		if(i <= r) z[i] = min(z[i - l], r - i + 1);
		while(i < n && str[z[i]] == str[i + z[i]]) ++z[i];
		if(i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
	}
	z[0] = n;
}

int main(){
	scanf("%s", str);
	z_function();
	for(int i = 0; i < n; i++){
		if(z[i]) values.push_back(make_pair(z[i], i));
	}
	sort(values.begin(), values.end());
	reverse(values.begin(), values.end());
	for(int i = 0; i < n * 4; i++) tree[i] = node();
	int ans = n;
	update(1, 0, n + 1, 0), update(1, 0, n + 1, n + 1);
	for(int i = 0; i < (int) values.size(); i++){
		int cur = values[i].first, j = i + 1;
		while((j < (int) values.size()) && (values[j].first == cur)) j++;
		j--;
		for(int k = i; k <= j; k++){
			int idx = 1 + values[k].second;
			update(1, 0, n + 1, idx);
		}
		if(tree[1].ret <= cur) ans = cur;
		i = j;
	}
	printf("%d\n", ans);
}
