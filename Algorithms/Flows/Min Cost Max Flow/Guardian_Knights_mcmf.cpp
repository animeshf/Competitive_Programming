// LightOj
#include "bits/stdc++.h"
using namespace std;


// Min cost Max flow template

struct MinimumCostMaximumFlow {

    typedef long long Flow;
    typedef long long Cost;
    static const Cost infiniteDistance = 1e18;
    static const Cost EPS = 1e-7;
    static const Flow infiniteFlow = 1e18;
    
    struct Edge{
        int u, v;
        Flow mx, c;
        Cost w;
        Edge(int u, int v, Flow mx, Flow c, Cost w) : u(u), v(v), mx(mx), c(c), w(w) {}
    };  
    
    vector < Edge > e;
    vector < vector < int > > g;  
    int n, source, sink, *prev;
    Cost *dist;

    MinimumCostMaximumFlow(int n) : n(n){
        dist = (Cost*)malloc(sizeof(Cost)*n);
        prev = (int*) malloc(sizeof(int)*n);
        g.resize(n);
    }

    ~MinimumCostMaximumFlow(){
        free(dist);
        free(prev);
        g.clear();
    }

    inline void add(int u, int v, Flow c, Cost w){
        g[u].push_back(e.size());
        e.push_back(Edge(u, v, 0, c, w));
        // For residual graph
        g[v].push_back(e.size());
        e.push_back(Edge(v, u, 0, 0, -w));
    }   

    inline pair < Cost, Flow > getMaxFlow(int source, int sink){
        this -> source = source;
        this -> sink = sink;
        for(int i = 0; i < (int) e.size(); i++) e[i].mx = 0;
        Flow flow = 0;
        Cost cost = 0;
        while(bellmanFord()){
            int u = sink;
            Flow pushed = infiniteFlow;
            Cost pushCost = 0;
            while(u != source){   
                int id = prev[u];
                pushed = min(pushed, e[id].c - e[id].mx);
                pushCost += e[id].w;
                u = e[id].u;
            }
            u = sink;
            while(u != source){
                int id = prev[u];
                e[id].mx += pushed;
                e[id ^ 1].mx -= pushed;   
                u = e[id].u;
            }
            flow += pushed;
            cost += pushCost * pushed;
        }
        return make_pair(cost, flow);
    }

    inline bool bellmanFord(){
        for(int i = 0; i < n; ++i) dist[i] = infiniteDistance;
        dist[source] = 0;
        for(int k = 0; k < n; ++k){
            bool update = false;
            for(int id = 0; id < (int) e.size(); ++id){
                int u = e[id].u;
                int v = e[id].v;
                if(dist[u] + EPS >= infiniteDistance) continue;
                Cost w = e[id].w;
                if(e[id].mx < e[id].c && dist[v] > dist[u] + w + EPS){
                    dist[v] = dist[u] + w;
                    prev[v] = id;
                    update = true;
                }   
            }
            if(!update) break;
        }
        return (dist[sink] + EPS) < (infiniteDistance);
    }

    // After running mcmf_max, e[id].mx has the flow which has passed through that edge in the optimal soln
    inline void displayEdges(){
        cout << "******" << '\n';
        for(int i = 0; i < (int) e.size(); ++i)
            cout << e[i].u << " " << e[i].v << " " << e[i].mx << " " << e[i].c << " " << e[i].w <<"\n";
        cout << "******" << '\n';
    }
};
 



const int N = 200;
const int INF = N * N;

int t, n, k, m;
char a[N][N];
int id[N][N], lim[N], dist[N][N], seen[N][N];

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, -1, 0, 1};

inline bool isValid(int x, int y){
	return (x >= 1 && x <= n && y >= 1 && y <= n);
}

inline void bfs(int src_x, int src_y){
	int node = id[src_x][src_y];
	memset(seen, 0, sizeof seen);
	seen[src_x][src_y] = true;
	queue < pair < int, int > > bfs;
	queue < int > dis;
	bfs.push(make_pair(src_x, src_y));
	dis.push(0);
	while(!bfs.empty()){
		int x = bfs.front().first;
		int y = bfs.front().second;
		int d = dis.front();
		bfs.pop(), dis.pop();
		if(a[x][y] == 'm') dist[node][id[x][y]] = d;
		for(int i = 0; i < 4; i++){
			int nx = x + dx[i];
			int ny = y + dy[i];
			if(isValid(nx, ny) && a[nx][ny] != '#' && !seen[nx][ny]){
				seen[nx][ny] = true;
				bfs.push(make_pair(nx, ny));
				dis.push(d + 1);
			}
		}
	}
}

int main(){
	freopen("ioi.in", "r", stdin);
	cin >> t;
	for(int qq = 1; qq <= t; qq++){
		cin >> n >> k >> m;
		for(int i = 1; i <= k; i++)
			for(int j = 1; j <= m; j++)
				dist[i][j] = INF;
		for(int i = 1; i <= n; i++)
			for(int j = 1; j <= n; j++)
				cin >> a[i][j];
		for(int i = 1; i <= k; i++) cin >> lim[i];
		int cur_time = 0;
		for(int i = 1; i <= n; i++){
			for(int j = 1; j <= n; j++){
				if(a[i][j] == 'm') id[i][j] = ++cur_time;
				if(a[i][j] >= 'A' && a[i][j] <= 'Z') id[i][j] = a[i][j] - 'A' + 1;
			}
		}
		for(int i = 1; i <= n; i++){
			for(int j = 1; j <= n; j++){
				if(a[i][j] >= 'A' && a[i][j] <= 'Z') bfs(i, j);
			}
		}
		MinimumCostMaximumFlow mcmf(k + m + 2);
		int source = 0, sink = k + m + 1;
		for(int i = 1; i <= k; i++) mcmf.add(source, i, lim[i], 0);
		for(int i = 1; i <= k; i++){
			for(int j = 1; j <= m; j++){
				if(dist[i][j] != INF)
					mcmf.add(i, j + k, 1, dist[i][j]);
			}
		}
		for(int i = k + 1; i <= k + m; i++) mcmf.add(i, sink, 1, 0);
		cout << "Case " << qq  << ": " << mcmf.getMaxFlow(source, sink).first << "\n";
	}
}
