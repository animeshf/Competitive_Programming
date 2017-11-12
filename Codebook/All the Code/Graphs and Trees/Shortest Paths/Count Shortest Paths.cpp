// Undirected, Weighted Graph without self loops and multiple edges.
// dist[i][j] = Shortest Path from (i) to (j)
// num[i][j]  = Number of Shortest Paths from (i) to (j)

// NOI Social Network (WciPeg)

#include "bits/stdc++.h"
using namespace std;

const int N = 1e2 + 2;
const long long INF = 1e12;

int n, m, u, v, w;
long long dist[N][N], num[N][N];

int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= n; j++){
			dist[i][j] = INF;
		}
		dist[i][i] = 0;
	}
	for(int i = 1; i <= m; i++){
		scanf("%d %d %d", &u, &v, &w);
		dist[u][v] = dist[v][u] = w;
		num[u][v] = num[v][u] = 1;
	}
	for(int k = 1; k <= n; k++){
		for(int i = 1; i <= n; i++){
			for(int j = 1; j <= n; j++){
				if(dist[i][k] + dist[k][j] < dist[i][j]){
					dist[i][j] = dist[i][k] + dist[k][j];
					num[i][j] = num[i][k] * num[k][j];
				}
				else if(dist[i][k] + dist[k][j] == dist[i][j]){
					num[i][j] += num[i][k] * num[k][j];
				}
			}
		}
	}
	for(int i = 1; i <= n; i++){
		double ans = 0;
		for(int s = 1; s <= n; s++){
			for(int t = 1; t <= n; t++){
				if(s == i || t == i) continue;
				long long numerator = 0, denominator = 0;
				if(dist[s][t] == dist[s][i] + dist[i][t])
					numerator += (num[s][i] * num[i][t]);
				denominator += (num[s][t]);
				if(denominator != 0) ans += (numerator * 1.0 / denominator);
			}
		}
		printf("%.3f\n", ans);
	}
}