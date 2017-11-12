/**
	The Algorithm below is known as Heirholzer's Algorithm. It runs in O(V + E).

	The algorithm finds an Euler Tour or an Euler Circuit depending on whether the
	graph has 0 or 2 nodes with odd degree. If the graph is not Eulerian, it still
	returns some list, so make sure to first check that the graph is Eulerian.

	A connected graph has an Euler Path (A path that uses all edges but starts and ends at
	different nodes) if it has exactly 2 nodes with odd degree. While calling find_path()
	make sure you ensure that you call it with an ODD degree node since the path must
	start/end there.

	A connected graph has an Euler Circuit (A circuit that uses all edges and starts and ends
	at the same node) if all nodes have even degree. You can call find_path() with any
	node here.

	This algorithm handles self loops and multiple edges as well, but only works on undirected
	graphs.
**/

struct Edge;
typedef list<Edge>::iterator iter;

struct Edge
{
	int next_vertex;
	iter reverse_edge;

	Edge(int next_vertex)
		:next_vertex(next_vertex)
		{ }
};

const int max_vertices = ;
int num_vertices;
list<Edge> adj[max_vertices];		// adjacency list

vector<int> path;

void find_path(int v)
{
	while(adj[v].size() > 0)
	{
		int vn = adj[v].front().next_vertex;
		adj[vn].erase(adj[v].front().reverse_edge);
		adj[v].pop_front();
		find_path(vn);
	}
	path.push_back(v);
}

void add_edge(int a, int b)
{
	adj[a].push_front(Edge(b));
	iter ita = adj[a].begin();
	adj[b].push_front(Edge(a));
	iter itb = adj[b].begin();
	ita->reverse_edge = itb;
	itb->reverse_edge = ita;
}

// ************ End of Undirected Euler Path *************

/**
	A directed graph has an Eulerian cycle if and only if every node has an
	in-degree equal to its out-degree, and all of its nodes with nonzero degree
	belong to a single strongly connected component.

	A directed graph has an Eulerian path if and only if it is strongly connected and
	each vertex except 2 have the same in-degree as out-degree, and one of those 2
	vertices has out-degree with one greater than in-degree (this is the start vertex),
	and the other vertex has in-degree with one greater than out-degree (this is the end vertex).

	The code below works for both cases. For Euler Path, make sure that you call the function
	with the appropriate start vertex.
**/

std::vector<int> find_path_directed(std::vector<int> adj[], int u) {
  std::vector<int> stack, curr_edge(MAXN), res;
  stack.push_back(u);
  while (!stack.empty()) {
    u = stack.back();
    stack.pop_back();
    while (curr_edge[u] < (int)adj[u].size()) {
      stack.push_back(u);
      u = adj[u][curr_edge[u]++];
    }
    res.push_back(u);
  }
  std::reverse(res.begin(), res.end());
  return res;
}

// ************ End of Directed Euler Path *************

