Copy
// http://codeforces.com/contest/845/problem/G: Compute XOR-Shortest Walk from 1 to N.
// Author: rekt_n00b

// Solution Sketch
// Let P be any path from 1 to N.
// The optimal solution can be written as P xor (C_1, C_2, ..., C_K) where each C_i is some simple cycle
// in the graph. Since there can be exponentially many cycles, we find a cycle basis first. We can further
// reduce the cycle basis to 32 elements since we only care about the costs of the cycles, and not being
// able to represent all cycles in the graph. Basically, we want to be able to represent all possible XOR
// costs but not all possible cycles themselves, allowing us to reduce the basis to log(max_value) ~ 32 elements.
// After this we use a greedy algorithm to compute the minimum cost. More details are provided below.

#include "bits/stdc++.h"
using namespace std;

const int MAXN = 100005;

int n, m;
int cost[MAXN];
int parent_cost[MAXN]; // This is to deal with multiedges
int visited[MAXN];
vector<pair<int, int>> adj[MAXN];
vector<int> cycle_basis;


/**
    We want to be able to represent all possible XOR costs.

    Idea: There always exists a basis in which every element has a distinct most-significiant-bit.
    So now we incrementally build a basis. Note this also implies that the basis will have atmost
    log(max_value) ~ 32 elements. For clarity, we keep the basis sorted at all times.

    C_1 can be assumed to be in the basis.
    Now, consider C_2. If C_2 xor C_1 = 0, then ignore C_2 else add C_2 to the basis.

    Suppose at some point the basis is B = {C_1, C_2, ...., C_k} where C_i >= C_{i + 1}..
    If we're adding x to B, we need to make sure x is not representable using elements in B.
    Note since elements of B have distinct most-siginificant-bits, this can be easily checked
    since we can deterministically see if some C_i must be used to make x. So this can be done
    in O(|B|) time.

    Iff x is not a linear combination, then add x to B.
**/
void add_cycle(int x) {
    int s = (int) cycle_basis.size();
    if (s == 0) {
        cycle_basis.push_back(x);
        return;
    }
    for (int z: cycle_basis) {
        x = min(x, x ^ z);
    }
    if (x > 0) {
        cycle_basis.push_back(x);
        s = s + 1;
        for (int i = s - 2; i >= 0; i--) {
            if (cycle_basis[i] < cycle_basis[i + 1])
                swap(cycle_basis[i], cycle_basis[i + 1]);
        }
    }
}

/**
    There can be exponentially many simple cycles in a graph, but fun fact is that we
    can make a basis for these cycles comprising only |E|-|V|+1 elements. Any simple cycle
    in the graph can then be represented as the XOR-Sum (Ring Sum) of elements in this basis.

    The following algorithm works for finding the cycle-basis B:
    - Take any spanning tree T.
    - All back edges e not in T create one simple cycle in [T \union {e}], called C_{e}.
    - Add each C_e to B.
**/
void dfs(int u, int p) {
    visited[u] = true;
    for (auto e: adj[u]) {
        int v = e.first;
        int w = e.second;
        if ((v != p) || (v == p && parent_cost[v] != w)) {
            if (!visited[v]) {
                cost[v] = cost[u] ^ w;
                parent_cost[v] = w;
                dfs(v, u);
            } else {
                add_cycle(cost[u] ^ cost[v] ^ w);
            }
        }
    }
}


/**
    So we have some path with cost p, and we have a basis B of cycles.
    We want to compute the optimal subset S of B such that p xor S is
    minimal. Again we use the crucial insight that each element of B
    has a distinct most-signficiant-bit, so we can greedily minimize.
**/
inline int solve(int p) {
    for (int x: cycle_basis) {
        p = min(p, p ^ x);
    }
    return p;
}

int main() {
    ios :: sync_with_stdio(false);
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        int x, y, w; cin >> x >> y >> w;
        adj[x].push_back(make_pair(y, w));
        adj[y].push_back(make_pair(x, w));
    }
    dfs(1, 0);
    int path = cost[1] ^ cost[n];
    cout << solve(path) << endl;
}
