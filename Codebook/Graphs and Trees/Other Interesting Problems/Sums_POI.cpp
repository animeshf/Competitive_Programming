#include <bits/stdc++.h>
#define ii pair < int, int >
using namespace std;

const int N = 50000 + 50;
const int INF = 2147483647;

int n, k;
int a[N], dist[N];

int main() {
	cin >> n;
	for(int i = 1; i <= n; i++){
		cin >> a[i];
	}
	int a1 = a[1];
	for(int i = 0; i < a1; i++){
		dist[i] = INF;
	}
	priority_queue < ii, vector < ii >, greater < ii > > min_heap;	
	dist[0] = 0;	
	min_heap.push(make_pair(0, 0));
	while(!min_heap.empty()){
		ii x = min_heap.top();
		min_heap.pop();
		if(dist[x.second] < x.first) continue;
		for(int i = 2; i <= n; i++){
			ii y = make_pair(x.first + a[i], (x.second + a[i]) % a1);
			if(y.first < dist[y.second]){
				dist[y.second] = y.first;
				min_heap.push(y);
			}
		}
	}
	cin >> k;
	while(k--){
		int b; cin >> b;
		if(dist[b % a1] <= b) cout << "TAK\n";
		else cout << "NIE\n";
	}
}
