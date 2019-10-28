#include "bits/stdc++.h"
using namespace std;

const int MAXN = 200005;
const int EMPTY = MAXN - 1;

int N, n, k, arr[MAXN];

struct treap_node {
  long long val;
  int pri, siz, lc, rc;
}treap[MAXN];

inline int create_node(long long val){
  N = N + 1;
  treap[N].val = val;
  treap[N].pri = rand();
  treap[N].siz = 1;
  treap[N].lc = treap[N].rc = EMPTY;
  return N;
}

inline void refresh(int root) {
  int l = treap[root].lc;
  int r = treap[root].rc;
  treap[root].siz = treap[l].siz + 1 + treap[r].siz;
}

inline pair<int, int> split(int root, long long key) {
  pair<int, int> parts = pair<int, int>(EMPTY, EMPTY);
  if (root == EMPTY) return parts;
  if (treap[root].val <= key) {
    parts = split(treap[root].rc, key);
    treap[root].rc = parts.first;
    refresh(root);
    parts.first = root;
    return parts;
  } else {
    parts = split(treap[root].lc, key);
    treap[root].lc = parts.second;
    refresh(root);
    parts.second = root;
    return parts;
  }
}

inline int merge(int l, int r) {
  if (l == EMPTY) return r;
  if (r == EMPTY) return l;
  if (treap[l].pri > treap[r].pri) {
    treap[l].rc = merge(treap[l].rc, r);
    refresh(l);
    return l;
  } else {
    treap[r].lc = merge(l, treap[r].lc);
    refresh(r);
    return r;
  }
}

inline int insert(int root, long long value) { 
  pair<int, int> parts = split(root, value - 1);
  int x = create_node(value);
  return merge(merge(parts.first, x), parts.second);
}

inline int query_k(int root, long long k) {
  if (root == EMPTY) return 0;
  if (treap[root].val <= k) {
    return treap[treap[root].lc].siz + 1 + query_k(treap[root].rc, k);
  } else {
    return query_k(treap[root].lc, k);
  }
}

int main() {
  ios :: sync_with_stdio(false);
  cin >> n >> k;
  for (int i = 1; i <= n; i++) {
    cin >> arr[i];
    arr[i] -= k;
  }
  long long sum = 0, ans = 0;
  int root = EMPTY;
  root = insert(root, 0);
  for (int i = 1; i <= n; i++) {
    sum += arr[i];
    ans += query_k(root, sum);
    root = insert(root, sum);
  }
  cout << ans << endl;
}