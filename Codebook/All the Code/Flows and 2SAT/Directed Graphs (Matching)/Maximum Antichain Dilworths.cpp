// LIGHTOJ
#include "bits/stdc++.h"
using namespace std;

const int N = 105;
const int M = 105 * 105;

int t, n;
int arr[N];
vector < int > values;

/*
	We will use Dilworths' Theorem and Min-Path-Cover on a DAG to solve this problem.
	Add an edge from number x to number y (x != y), if y % x == 0.
	Now we've built a dag, and we want to find the size of maximum antichain in this DAG.
	We need to find a subset of nodes such that no node in the subset can be reached from
	any other node in the subset.

	By Dilworth's Theorem, Size of maximum antichain = Min Path Cover in the DAG.
	We also know that Min Path Cover is just given by n - maxMatching(). Done!

	Computing the Lexicographically Smallest Anti-Chain:
	Let the size be S.
	To find the lexicographically smallest anti-chain, fix the smallest element and find
	the maximum antichain on the remaining graph. If you can get an antichain of size S - 1,
	then the smallest element can be taken. Repeat this process!
*/

/*
	Standard Matching Template
*/

int n1, n2, edges, last[N], previous[M], head[M];
int matching[N], dist[N], Q[N];
bool used[N], vis[N];

inline void init(int _n1, int _n2) {
	n1 = _n1;
	n2 = _n2;
	edges = 0;
	fill(last, last + n1, -1);
}

inline void addEdge(int u, int v) {
	head[edges] = v;
	previous[edges] = last[u];
	last[u] = edges++;
}

inline void bfs() {
	fill(dist, dist + n1, -1);
	int sizeQ = 0;
	for(int u = 0; u < n1; ++u){
		if(!used[u]){
			Q[sizeQ++] = u;
			dist[u] = 0;
		}
	}
	for(int i = 0; i < sizeQ; i++){
		int u1 = Q[i];
		for(int e = last[u1]; e >= 0; e = previous[e]){
			int u2 = matching[head[e]];
			if(u2 >= 0 && dist[u2] < 0){
				dist[u2] = dist[u1] + 1;
				Q[sizeQ++] = u2;
			}
		}
	}
}

inline bool dfs(int u1) {
	vis[u1] = true;
	for(int e = last[u1]; e >= 0; e = previous[e]){
		int v = head[e];
		int u2 = matching[v];
		if(u2 < 0 || !vis[u2] && dist[u2] == dist[u1] + 1 && dfs(u2)){
			matching[v] = u1;
			used[u1] = true;
			return true;
		}
	}
	return false;
}

inline int maxMatching() {
	fill(used, used + n1, false);
	fill(matching, matching + n2, -1);
	for(int res = 0; ;){
		bfs();
		fill(vis, vis + n1, false);
		int f = 0;
		for(int u = 0; u < n1; ++u)
			if(!used[u] && dfs(u))
				++f;
		if(!f) return res;
		res += f;
	}
}

// End of Hopcroft Karp Template


int main(){
	scanf("%d", &t);
	for(int qq = 1; qq <= t; qq++){
		scanf("%d", &n);
		values.clear();
		for(int i = 0; i < n; i++){
			scanf("%d", arr + i);
			values.push_back(arr[i]);
		}
		sort(values.begin(), values.end());
		values.resize(unique(values.begin(), values.end()) - values.begin());
		n = (int) values.size();
		init(n, n);
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				if(i == j) continue;
				if((values[j] % values[i]) == 0) addEdge(i, j);
			}
		}
		int max_antichain = n - maxMatching(), ans = max_antichain;
		vector < int > in_sol;
		set < int > result;
		for(int i = 0; i < n; i++) result.insert(values[i]);
		while(!result.empty()){
			set < int > :: iterator it = result.begin();
			int check_val = *it;
			vector < int > tmp; n = 0;
			while((++it) != result.end()){
				if(*it % check_val){
					++n;
					tmp.push_back(*it);
				}
			}
			init(n, n);
			for(int i = 0; i < n; i++){
				for(int j = 0; j < n; j++){
					if(i == j) continue;
					if(tmp[j] % tmp[i] == 0) addEdge(i, j);
				}
			}
			if(n - maxMatching() == max_antichain - 1){
				max_antichain = max_antichain - 1;
				in_sol.push_back(check_val);
				set < int > :: iterator it2 = result.begin();
				while((++it2) != result.end()){
					if(*it2 % check_val == 0){
						set < int > :: iterator it3 = it2;
						it3++;
						result.erase(it2);
						it2 = (--it3);
					}
				}
			}
			result.erase(result.begin());
		}
		printf("Case %d:", qq);
		for(int i = 0; i < ans; i++) printf(" %d", in_sol[i]);
		printf("\n");
	}
}
