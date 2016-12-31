#include "bits/stdc++.h"
using namespace std;

/*
    Generates a random tree with n nodes : nodes indexed 1 to n.
    Prints number of nodes followed by edges of the tree.
    Ensures p_i < i is not the case.

    Depth of such tree is expected to be O(log n)
*/

inline void generateRandomTree(int n) {
    vector < int > a(n);
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
    }
    random_shuffle(a.begin(), a.end());
    vector < pair < int, int > > tree_edges;
    for (int i = 1; i < n; i++) {
        int parent_ai = a[rand() % i];
        tree_edges.push_back(make_pair(parent_ai, a[i]));
    }
    cout << n << "\n";
    for (auto it : tree_edges) {
        cout << (it.first) << " " << (it.second) << "\n";
    }
}


/*
    Generates a random tree with n nodes : nodes indexed 1 to n.
    Prints number of nodes followed by edges of the tree.
    Ensures p_i < i is not the case.

    This one ensures depth is O(n) (no matter where you root it).
*/

inline void generateRandomTreeWithHighDepth(int n) {
    int sz = (n + 1) / 2;
    vector < int > a(sz);
    for (int i = 0; i < sz; i++) {
        a[i] = sz + i + ((n & 1) ^ 1);
    }
    random_shuffle(a.begin(), a.end());
    vector < pair < int, int > > tree_edges;
    for (int i = 2; i <= n / 2; i++) {
        int parent_ai = i - 1;
        tree_edges.push_back(make_pair(parent_ai, i));
    }
    tree_edges.push_back(make_pair(n / 2, a[0]));
    for (int i = 1; i < sz; i++) {
        int parent_ai = a[rand() % i];
        tree_edges.push_back(make_pair(parent_ai, a[i]));
    }
    cout << n << "\n";
    for (auto it : tree_edges) {
        cout << (it.first) << " " << (it.second) << "\n";
    }
}

/*
    Generates the "broom tree" with n nodes. nodes indexed 1 to n.
    Prints number of nodes followed by edges of the tree.
    Ensures p_i < i is not the case.

    Useful to break degree based heuristics.
*/

inline void generateBroomTest(int n) {
    vector < int > a(n);
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
    }
    random_shuffle(a.begin(), a.end());
    vector < pair < int, int > > tree_edges;
    for (int i = 1; i < n / 2; i++) {
        int parent_ai = a[i - 1];
        tree_edges.push_back(make_pair(parent_ai, a[i]));
    }
    for (int i = n / 2; i < n; i++) {
        int parent_ai = a[(n / 2) - 1];
        tree_edges.push_back(make_pair(parent_ai, a[i]));
    }
    cout << n << "\n";
    for (auto it : tree_edges) {
        cout << (it.first) << " " << (it.second) << "\n";
    }
}


/*
    Generates the "linear tree" with n nodes. nodes indexed 1 to n.
    Prints number of nodes followed by edges of the tree.
    Ensures p_i < i is not the case.

    Useful to break many height/depth/subtreeSize based heuristics
*/

inline void generateChain(int n) {
    vector < int > a(n);
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
    }
    random_shuffle(a.begin(), a.end());
    vector < pair < int, int > > tree_edges;
    for (int i = 1; i < n; i++) {
        int parent_ai = a[i - 1];
        tree_edges.push_back(make_pair(parent_ai, a[i]));
    }
    cout << n << "\n";
    for (auto it : tree_edges) {
        cout << (it.first) << " " << (it.second) << "\n";
    }
}

/*
    Generates a tree with one chain of length sqrt(n), and sqrt(n) - 1 chains each of length
    sqrt(n) popping out of the last node in the chain. nodes indexed 1 to n.
    Prints number of nodes followed by edges of the tree.
    Ensures p_i < i is not the case.
    IMPORTANT : Assumes n is a perfect square!

    Useful especially for data structure problems.
*/

inline void generateFireworksTree(int n) {
    vector < int > a(n);
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
    }
    random_shuffle(a.begin(), a.end());
    int s = sqrt(n);
    if ((s * s) != n) {
        s += 1;
    }
    if ((s * s) != n) {
        cout << "You haven't entered a perfect square!\n";
        assert(false);
    }
    vector < pair < int, int > > tree_edges;
    for (int i = 1; i < s; i++) {
        int parent_ai = a[i - 1];
        tree_edges.push_back(make_pair(parent_ai, a[i]));
    }
    for (int i = 1; i < s; i++) {
        for (int j = i * s; j < (i + 1) * s; j++) {
            int parent_aj;
            if (j == (i * s)) {
                parent_aj = a[s - 1];
            } else {
                parent_aj = a[j - 1];
            }
            tree_edges.push_back(make_pair(parent_aj, a[j]));
        }
    }
    cout << n << "\n";
    for (auto it : tree_edges) {
        cout << (it.first) << " " << (it.second) << "\n";
    }
}


/*
    Generates the star tree with n nodes. nodes indexed 1 to n.
    Prints number of nodes followed by edges of the tree.
    Ensures p_i < i is not the case.
*/

inline void generateStarTree(int n) {
    vector < int > a(n);
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
    }
    random_shuffle(a.begin(), a.end());
    vector < pair < int, int > > tree_edges;
    for (int i = 1; i < n; i++) {
        int parent_ai = a[0];
        tree_edges.push_back(make_pair(parent_ai, a[i]));
    }
    cout << n << "\n";
    for (auto it : tree_edges) {
        cout << (it.first) << " " << (it.second) << "\n";
    }
}


/*
    Generates a BST (Segment Tree) with n nodes. nodes indexed 1 to n.
    Prints number of nodes followed by edges of the tree.
    Ensures p_i < i is not the case.

    IMPORTANT : n must be odd (n >= 3)
*/

int node_id = 0;

inline void makeBST(int id, int l, int r, vector < pair < int, int > > &edges) {
    if (l == r) {
        return;
    }
    int lc = ++node_id;
    int rc = ++node_id;
    int mid = (l + r) >> 1;
    edges.push_back(make_pair(id, lc));
    edges.push_back(make_pair(id, rc));
    makeBST(lc, l, mid, edges);
    makeBST(rc, mid + 1, r, edges);
}

inline void generateBST(int n) {
    if (n % 2 == 0) {
        cout << "n must be odd!\n";
        assert(false);
    }
    int sz = (n + 1) / 2;
    vector < int > a(n);
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
    }
    random_shuffle(a.begin(), a.end());
    vector < pair < int, int > > tree_edges;
    node_id = 0;
    makeBST(0, 0, sz - 1, tree_edges);
    cout << n << "\n";
    assert((int)(tree_edges.size()) == n - 1);
    for (auto it : tree_edges) {
        assert(0 <= it.first && it.first < n);
        assert(0 <= it.second && it.second < n);
        cout << a[(it.first)] << " " << a[(it.second)] << "\n";
    }
}


int main() {
    freopen("trees.out", "w", stdout);
    srand(time(NULL));
    int n = 123201;
    generateRandomTree(n);
    cout << "--------------\n";
    generateRandomTreeWithHighDepth(n);
    cout << "--------------\n";
    generateBroomTest(n);
    cout << "--------------\n";
    generateChain(n);
    cout << "--------------\n";
    generateFireworksTree(n); // n must be a perfect square
    cout << "--------------\n";
    generateStarTree(n);
    cout << "--------------\n";
    generateBST(n); // n must be odd
}