struct range_update_bit {
  vector<int> tree[2];
  int tree_size;
  inline void init(int n) {
    tree_size = n;
    for (int i = 0; i < 2; i++) {
      tree[i].resize(n + 1, 0);
    }
  }
  inline void update_tree(int id, int pos, int delta) {
    for (int i = pos; i <= tree_size; i += i & -i) {
      tree[id][i] = add(tree[id][i], delta);
    }
  }
  inline int query_tree(int id, int pos) {
    int res = 0;
    for (int i = pos; i > 0; i -= i & -i) {
      res = add(res, tree[id][i]);
    }
    return res;
  }
  inline void update(int l, int r, int x) {
    int value = prod(l - 1, x);
    update_tree(0, l, prod(-1, value));
    update_tree(0, r + 1, prod(r, x));
    update_tree(1, l, x);
    update_tree(1, r + 1, prod(-1, x));
  }
  inline int query(int l, int r) {
    int sumR = add(prod(r, query_tree(1, r)), query_tree(0, r));
    int sumL = add(prod(l - 1, query_tree(1, l - 1)), query_tree(0, l - 1));
    return add(sumR, prod(-1, sumL));
  }
}bit;
