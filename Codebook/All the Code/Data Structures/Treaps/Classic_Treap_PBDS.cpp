// Number of subarrays with average >= K.
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <bits/stdc++.h>
 
using namespace __gnu_pbds; 
using namespace std;
 
const int N = 200005;
 
// This stores a set of objects.
// For multiset, I've used pair<..., int> instead
typedef tree <
  pair<long long, int>,
  null_type,
  less<pair<long long, int>>,
  rb_tree_tag,
  tree_order_statistics_node_update
> ordered_set;
 
// treap.order_of_key(x): Number of elements <= x in the set
// *treap.find_by_order(k): kth smallest element in set (k = 0 is smallest)
// end(treap) iterator is returned when k >= size of set

int n, k;
int arr[N];
ordered_set treap;
 
int main() {
  ios :: sync_with_stdio(false);
  cin >> n >> k;
  for (int i = 1; i <= n; i++) {
    cin >> arr[i];
    arr[i] -= k;
  }
  long long sum = 0, ans = 0;
  treap.insert({0, 0});
  for (int i = 1; i <= n; i++) {
    sum += arr[i];
    ans += treap.order_of_key({sum, n + 1});
    treap.insert({sum, i});
  }
  cout << ans << endl;
} 
