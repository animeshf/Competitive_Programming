// LIS after modifying one element of the array - ZIPLINE CF.

#include <bits/stdc++.h>
#define rf freopen("inp.in", "r", stdin)
using namespace std;

const int mx = 400005;

int n, q;
int arr[mx], f[mx], g[mx], pref[mx], suf[mx];
int bit1[mx], bit2[mx], cnt[mx], lis = 1;
vector < pair < int , pair < int, int > > > event;
pair < int, int > query[mx];
vector < int > elements;
int comp[mx];

void update1(int x, int v){
	for(int i = x; i < mx; i += i & (-i))
		bit1[i] = max(bit1[i], v);
}

int query1(int x){
	int res = 0;
	for(int i = x; i; i -= i & (-i))
		res = max(res, bit1[i]);
	return res;
}

void update2(int x, int v){
	for(int i = x; i; i -= i & (-i))
		bit2[i] = max(bit2[i], v);
}

int query2(int x){
	int res = 0;
	for(int i = x; i < mx; i += i & (-i))
		res = max(res, bit2[i]);
	return res;
}

int main(){

	scanf("%d %d\n", &n, &q);
	for(int i = 1; i <= n; i++){
		scanf("%d ", &arr[i]);
		elements.push_back(arr[i]);
		event.push_back(make_pair(arr[i], make_pair(0, i)));
	}

	sort(elements.begin(), elements.end());
	elements.resize(unique(elements.begin(), elements.end()) - elements.begin());

	for(int i = 1; i <= n; i++){
		comp[i] = lower_bound(elements.begin(), elements.end(), arr[i]) - elements.begin() + 1;
	}

	for(int i = 1; i <= q; i++){
		int pos, val;
		scanf("%d %d\n", &pos, &val);
		event.push_back(make_pair(val, make_pair(-i, pos)));
		query[i] = (make_pair(pos, val));
	}

	sort(event.begin(), event.end());

	for(int i = 1; i <= n; i++){
		f[i] = query1(comp[i] - 1) + 1;
		update1(comp[i], f[i]);
		lis = max(lis, f[i]);
	}

	for(int i = n; i >= 1; i--){
		g[i] = query2(comp[i] + 1) + 1;
		update2(comp[i], g[i]);
	}

	memset(bit1, 0, sizeof bit1);
	memset(bit2, 0, sizeof bit2);

	for(int i = 0 ; i < event.size(); i++){
		int type = event[i].second.first;
		int pos  = event[i].second.second;
		if(type == 0){ // Array element
			update1(pos, f[pos]);
		}
		else{ // Query element
			pref[-type] = query1(pos - 1) + 1;
		}
		event[i].second.first = (-type); 
	}

	sort(event.begin(), event.end());
	reverse(event.begin(), event.end());

	for(int i = 0 ; i < event.size(); i++){
		int type = event[i].second.first;
		int pos  = event[i].second.second;
		if(type == 0){ // Array element
			update2(pos, g[pos]);
		}
		else{ // Query element
			suf[type] = query2(pos + 1) + 1;
		}
	}
	
	for(int i = 1; i <= n; i++){
		if(f[i] + g[i] - 1 == lis){
			cnt[f[i]]++;
		}
	}
	
	for(int i = 1; i <= q; i++){
		int pos = query[i].first, res = 1;
		res = max(res, pref[i] + suf[i] - 1); // lis including (i)
		if(f[pos] + g[pos] - 1 == lis && cnt[f[pos]] == 1) 
			res = max(res, lis - 1); // lis decreases
		else 
			res = max(res, lis);  // lis constant
		printf("%d\n", res);
	}

}