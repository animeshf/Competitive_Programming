// Ans[i] = Shortest Path from S to D when you remove edge (i)
#include <bits/stdc++.h>
using namespace std;

const int MAXE = 50005;
const int MAXN = 7005;
const int INF = (int)(1e9);

// Information about Edges
int U[MAXE], V[MAXE], W[MAXE], VAL[MAXE], E[MAXE];

// Information about Nodes
int DIST[2][MAXN], LEV[MAXN], DSU[MAXN], PAR[MAXN], F[MAXN];

// ANS[i] = Shortest Path when you remove edge number (i)
int ANS[MAXE];

map < int , int > ID[MAXN]; // MAP[u][v] = Edge number of the edge (u->v)
vector < int > edgeList[MAXN]; // List of Edges
vector < int > tree[MAXN]; // Stores any Shortest Path Tree

int N, M, Q, S, D;

bool coolEdge[MAXE];
bool coolNode[MAXN];

inline void dijkstra(int src, int idx){

    for(int i = 0 ; i < N ; i++) DIST[idx][i] = INF;

    set < pair < int , pair < int , int > > > nodes;
    nodes.insert( make_pair(0 , make_pair(src, 0) ) );

    while(!nodes.empty()){

        int u  = (*nodes.begin()).second.first;
        int edgeNo = (*nodes.begin()).second.second;
        int v  = (U[edgeNo] + V[edgeNo]) - (u);
        int c  = (*nodes.begin()).first;

        nodes.erase(nodes.begin());

        if( DIST[idx][u] == INF ){

            // Add to Shortest Path Tree
            if( (idx == 0) and (edgeNo > 0) ){
                coolEdge[edgeNo] = true;
                tree[u].push_back(edgeNo);
                tree[v].push_back(edgeNo);
            }

            DIST[idx][u] = c;
            for(int i = 0 ; i < edgeList[u].size() ; i++){
                edgeNo = edgeList[u][i];
                v = (U[edgeNo] + V[edgeNo]) - (u);
                nodes.insert(make_pair(W[edgeNo] + c, make_pair(v, edgeNo)));
            }
        }
    }

}

bool dfs(int u, int p){

    bool inPath = (u == D);

    for(int i = 0 ; i < tree[u].size() ; i++){

        int edgeNo = tree[u][i];
        int v = (U[edgeNo] + V[edgeNo]) - (u);
        if(edgeNo == p) continue;

        DSU[v] = PAR[v] = u;
        LEV[v] = LEV[u] + 1;

        inPath |= dfs(v, edgeNo);
    }

    if(p != -1) coolEdge[p] = inPath;

    coolNode[u] = inPath;
    return inPath;
}

bool cmp(int x , int y){
    return (VAL[x]) < (VAL[y]);
}

int find(int x){
    if(coolNode[x]) return x;
    return DSU[x] = find(DSU[x]);
}

int goUP(int u, int lca, int val){

    if(LEV[u] <= LEV[lca]) return u;
    if(!coolNode[u]) return DSU[u] = goUP(DSU[u], lca, val); // Visit each edge once

    coolNode[u] = false;
    // Mark this node on Shortest Path as processed

    int p = PAR[u];
    int edgeNo = ID[min(u,p)][max(u,p)];
    ANS[edgeNo] = val;

    return DSU[u] = goUP(DSU[u], lca, val); // Compress Tree
}

int main() {

    cin.tie(0), ios::sync_with_stdio(false);

    cin >> N >> M >> Q;
    for(int i = 1 ; i <= M ; i++){

        cin >> U[i] >> V[i] >> W[i];
        if(U[i] > V[i]) swap(U[i], V[i]);

        edgeList[U[i]].push_back(i);
        edgeList[V[i]].push_back(i);
        ID[U[i]][V[i]] = i;

        ANS[i] = INF;
    }

    S = 0, D = N - 1;

    dijkstra(S, 0); // Dijkstra from Source
    dijkstra(D, 1); // Dijkstra from Destination

    int elen = 0;

    // Cool edges are those that are in the Shortest Path Tree
    for(int i = 1 ; i <= M ; i++){
        if(coolEdge[i]) continue;
        // If edge is in the Shortest Path Tree, Ignore!
        E[ elen++ ] = i;
        VAL[i] = min(DIST[0][U[i]] + DIST[1][V[i]], DIST[0][V[i]] + DIST[1][U[i]]) + W[i];
    }


    sort(E, E + elen, cmp);
    dfs(S, -1);

    // Now Cool edges are those which lie on the Shortest Path from S -> D
    // Cool nodes are those which lie on the Shortest Path from S -> D

    for(int i = 1 ; i <= M ; i++){
        if(!coolEdge[i])  // Ans for all these edges is = Shortest Path from S -> D
            ANS[i] = DIST[0][D];
    }

    DSU[S] = PAR[S] = S;

    for(int i = 0 ; i < N ; i++){
        if(!coolNode[i]){
             // If (i) isn't in Shortest Path
            // Find the first ancestor of (i) which is in Shortest Path
            DSU[i] = find(DSU[i]);
            F[i] = DSU[i];
        }
        else F[i] = i;
    }

    for(int i = 0 ; i < elen ; i++){

        int edgeNo = E[i];
        int u = U[edgeNo], v = V[edgeNo], w = VAL[edgeNo];

        u = F[u], v = F[v];
        int lca = (LEV[u] < LEV[v]) ? u : v;

        goUP(u, lca, w);
        goUP(v, lca, w);
    }

    while(Q--){
        int edge;
        cin >> edge;
        edge++;
        if(ANS[edge] == INF) cout << "-1" << '\n';
        else cout << ANS[edge] << '\n';
    }

    return 0;
}
