#include "bits/stdc++.h"
using namespace std;

const int N = 300000 + 30;
const int BIT = 30;

struct trie_node {
    trie_node *lc, *rc;
    trie_node(trie_node *l = NULL, trie_node *r = NULL) {
        lc = l;
        rc = r;
    }
    inline trie_node* get_left(trie_node* x) {
        if (x) {
            return x -> lc;
        } else {
            return NULL;
        }
    }
    inline trie_node* get_right(trie_node* x) {
        if (x) {
            return x -> rc;
        } else {
            return NULL;
        }
    }
    inline trie_node* update(trie_node* old_root, int i, int value) {
        if (i == -1) {
            return new trie_node();
        }
        trie_node * nw = new trie_node();
        if (value & (1 << i)) {
            nw -> lc = get_left(old_root);
            nw -> rc = update(get_right(old_root), i - 1, value);
        } else {
            nw -> rc = get_right(old_root);
            nw -> lc = update(get_left(old_root), i - 1, value);
        }
        return nw;
    }
    inline int query_min(trie_node* root, int i, int value) {
        if (i == -1) {
            return 0;
        }
        if (value & (1 << i)) {
            if (get_right(root)) {
                return query_min(get_right(root), i - 1, value);
            } else {
                return query_min(get_left(root), i - 1, value) | (1 << i);
            }
        } else {
            if (get_left(root)) {
                return query_min(get_left(root), i - 1, value);
            } else {
                return query_min(get_right(root), i - 1, value) | (1 << i);
            }
        }
    }
    inline int query_max(trie_node* root, int i, int value) {
        if (i == -1) {
            return 0;
        }
        if (value & (1 << i)) {
            if (get_left(root)) {
                return query_max(get_left(root), i - 1, value) | (1 << i);
            } else {
                return query_max(get_right(root), i - 1, value);
            }
        } else {
            if (get_right(root)) {
                return query_max(get_right(root), i - 1, value) | (1 << i);
            } else {
                return query_max(get_left(root), i - 1, value);
            }
        }
    }
};

int parent[N];
unordered_map < int, int > compress;
trie_node* trie[N];

int main() {
    int n, q, r, root_key;
    scanf("%d %d %d %d", &n, &q, &r, &root_key);
    int timer = 0;
    trie[0] = new trie_node();
    compress[r] = ++timer;
    parent[compress[r]] = 0;
    trie[compress[r]] = trie[0] -> update(trie[parent[compress[r]]], BIT, root_key);
    for (int i = 1; i < n; i++) {
        int u, v, k;
        scanf("%d %d %d", &u, &v, &k);
        compress[u] = ++timer;
        parent[compress[u]] = compress[v];
        trie[compress[u]] = trie[0] -> update(trie[parent[compress[u]]], BIT, k);
    }
    int last_answer = 0;
    while (q--) {
        int t; scanf("%d", &t);
        t ^= last_answer;
        if (t == 0) {
            int v, u, k; scanf("%d %d %d", &v, &u, &k);
            v ^= last_answer; u ^= last_answer; k ^= last_answer;
            compress[u] = ++timer;
            parent[compress[u]] = compress[v];
            trie[compress[u]] = trie[0] -> update(trie[parent[compress[u]]], BIT, k);
        } else {
            int v, k; scanf("%d %d", &v, &k);
            v ^= last_answer; k ^= last_answer;
            int min_answer = trie[0] -> query_min(trie[compress[v]], BIT, k);
            int max_answer = trie[0] -> query_max(trie[compress[v]], BIT, k);
            printf("%d %d\n", min_answer, max_answer);
            last_answer = min_answer ^ max_answer;
        }
    }
}