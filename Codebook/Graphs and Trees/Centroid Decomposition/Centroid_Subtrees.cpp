/*
	Computing centroid for every subtree of a tree.
*/

#include "bits/stdc++.h"
using namespace std;

const int MAX = 3e5 + 5;

int par[MAX], centroid[MAX];
long long len[MAX];
long long sub[MAX], tot[MAX];
long long dist_to_centroid[MAX], ans[MAX];
long long res = 0;
vector < int > adj[MAX];

/*
	Let s(v) denote the subtree rooted at (v)
	The tree is a directed tree with its root at 0.
	centroid[u] = centroid of s(u)
	ans[u] = sum(dist(centroid[u], v)) for all v in s(u)
	len[u] = weight of edge from u to parent[u]
	sub[u] = subtree size of s(u)
	tot[u] = sum(dist(u, v)) for all v in s(u)
	dist_to_centroid[u] = distance from u to centroid of s(u)
*/

struct TreeSums{
	int find_upper(int root, int u){
		int upper_tree = sub[root] - sub[u];
		return upper_tree;
	}
	void dfs_find(int u){
		centroid[u] = u;
		int heavy_child = -1;
		for(int v : adj[u]){
			dfs_find(v);
			if((heavy_child == -1) || (sub[v] > sub[heavy_child]))
				heavy_child = v;
		}
		if(heavy_child == -1) return; // Leaf node
		if(sub[heavy_child] <= sub[u] >> 1){ // u is the centroid of s(u)
			ans[u] = tot[u];
			res ^= ans[u];
			return;
		}
		/*
			The centroid is in s(heavy_child)
			We have recursively computed the centroid of s(heavy_child)
			The centroid for s(u) is now some ancestor of the centroid
			of s(heavy_child), or simply the centroid of s(heavy_child).
			Hence, we try to raise or lift the centroid of s(heavy_child)
			until the size of the "upper_tree" becomes <= s(u) / 2
		*/
		centroid[u] = centroid[heavy_child];
		dist_to_centroid[u] = dist_to_centroid[heavy_child] + len[heavy_child];
		ans[u] = ans[heavy_child] + dist_to_centroid[u] * (sub[u] - sub[heavy_child])
			   + tot[u] - (tot[heavy_child] + len[heavy_child] * sub[heavy_child]);
		while(true){
			int upper_tree = find_upper(u, centroid[u]);
			if(upper_tree <= sub[u] >> 1) break;
			ans[u] -= (2 * upper_tree - sub[u]) * len[centroid[u]];
			dist_to_centroid[u] -= len[centroid[u]];
			centroid[u] = par[centroid[u]];
		}
		res ^= ans[u];
	}
	void dfs_prep(int u){
		sub[u] = 1;
		for(int v : adj[u]){
			dfs_prep(v);
			sub[u] += sub[v];
			tot[u] += (tot[v] + len[v] * sub[v]);
		}
	}
	long long findSums(int N, int seed, int C, int D){
		for(int i = 0; i <= N; i++){
			adj[i].clear();
			sub[i] = tot[i] = ans[i] = dist_to_centroid[i] = 0;
			res = 0;
		}
		long long cur = seed;
		for(int i = 0; i <= N - 2; i++){
			cur = (C * 1LL * cur + D) % 1000000000;
			par[i + 1] = cur % (i + 1);
			adj[par[i + 1]].push_back(i + 1);
			cur = (C * 1LL * cur + D) % 1000000000;
			len[i + 1] = cur % 1000000;
		}
		dfs_prep(0);
		dfs_find(0);
		return res;
	}
};

int main(){
	TreeSums obj;
	cout << obj.findSums(6, 8, 3, 13) << '\n';
}