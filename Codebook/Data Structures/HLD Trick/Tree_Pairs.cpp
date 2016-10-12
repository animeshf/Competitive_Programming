// Find number of pairs (u, v) such that A[u] * A[v] = A[lca(u, v)]


#include "bits/stdc++.h"
using namespace std;

const int N = 1e5 + 50;

int n, arr[N], par[N];
vector < int > adj[N];
map < int, int > val[N];
long long ans = 0;

inline int root(int x){
    if(par[x] == x) return x;
    return par[x] = root(par[x]);
}

inline void unite(int u, int v, int target){
    u = root(u), v = root(v);
    if((int) val[u].size() < (int) val[v].size()){
        for(map < int, int > :: iterator it = val[u].begin(); it != val[u].end(); it++){
            int cur = (*it).first;
            if(target % cur == 0) ans += ((*it).second * 1LL * val[v][target / cur]);
        }
        for(map < int, int > :: iterator it = val[u].begin(); it != val[u].end(); it++){
            int cur = (*it).first;
            val[v][cur] += (*it).second;
        }
        val[u].clear();
        par[u] = v;
    }
    else{
        for(map < int, int > :: iterator it = val[v].begin(); it != val[v].end(); it++){
            int cur = (*it).first;
            if(target % cur == 0) ans += ((*it).second * 1LL * val[u][target / cur]);
        }
        for(map < int, int > :: iterator it = val[v].begin(); it != val[v].end(); it++){
            int cur = (*it).first;
            val[u][cur] += (*it).second;
        }
        val[v].clear();
        par[v] = u;
    }
}

inline void dfs(int u, int p){
    val[u][arr[u]]++;
    for(int i = 0; i < (int) adj[u].size(); i++){
        int v = adj[u][i];
        if(v == p) continue;
        dfs(v, u);
        unite(u, v, arr[u]);
    }
}

int main(){
    freopen("inp.in", "r", stdin);
    scanf("%d", &n);
    for(int i = 1; i < n; i++){
        int u, v;
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for(int i = 1; i <= n; i++){
        scanf("%d", arr + i);
        par[i] = i;
    }
    dfs(1, -1);
    printf("%lld\n", ans);
}