#include <bits/stdc++.h>
using namespace std;

typedef vector < int > :: iterator iter;


/* 
    ------------ Wavelet Tree Template ----------
    
    quantile(k, a, b) : k'th smallest element in [a, b)
    range(x, y, a, b) : # of elements with value in range [x, y] in subarray [a, b) 
    rank(x, k) : # of occurrences of x in [0, k)
    push_back(x) : Append another value x to the existing array.
                   Note : x should be in [0, sigma)
    pop_back() :   Pop the last element from the existing array.
    swap_adj(i) :  Swap arr[i] and arr[i + 1]. Assumes i is in [0, n - 1) 

    WaveTree obj(arr, sigma) : Creates a Wavelet Tree on the vector 'arr', alphabet size [0, sigma)

    All indices are Zero-Based.

    -----------------------------------------------
*/


class WaveTree {
    vector < vector < int > > tree; 
    vector < int > arr_copy;
    // tree[u][i] = uptil index (i) in node (u), how many values are <= (mid)
    int n, s;
    // O(n * log (sigma)) construction
    inline void build(iter b, iter e, int l, int r, int u) {
        if (l == r) return;
        int m = (l + r) / 2;
        tree[u].reserve(e - b + 1); 
        tree[u].push_back(0);
        for (iter it = b; it != e; ++it)
            tree[u].push_back(tree[u].back() + (*it <= m));
        iter p = stable_partition(b, e, [=](int i){ return i <= m;});
        // arr[b, p) have elements <= m and arr[p, e) have > m
        build(b, p, l, m, u * 2);
        build(p, e, m + 1, r, u * 2 + 1);
    }
 
    int qq, w;
    inline int range(int a, int b, int l, int r, int u) {
        if (r < qq or w < l) return 0;
        if (qq <= l and r <= w) return b - a;
        int m = (l + r) / 2, za = tree[u][a], zb = tree[u][b];
        return range(za, zb, l, m, u * 2) +
               range(a - za, b - zb, m + 1, r, u * 2 + 1);
    }
 
public:

    //arr[i] in [0, sigma)
    WaveTree(vector < int > arr, int sigma) {
        n = arr.size(); 
        s = sigma;
        tree.resize(s * 2);
        arr_copy = arr;
        build(arr.begin(), arr.end(), 0, s - 1, 1);
    }
 
    //k in [1, n], [a, b) is 0-indexed, -1 if error
    inline int quantile(int k, int a, int b) {
        if (a < 0 or b > n or k < 1 or k > b - a) return -1;
        int l = 0, r = s - 1, u = 1, m, za, zb;
        while (l != r) {
            m = (l + r) / 2;
            za = tree[u][a]; 
            zb = tree[u][b]; 
            u *= 2;
            if (k <= zb - za)
                a = za, b = zb, r = m;
            else
                k -= zb - za, a -= za, b -= zb,
                l = m + 1, ++u;
        }
        return r;
    }
 
    //Counts numbers in [x, y] in positions [a, b)
    inline int range(int x, int y, int a, int b) {
        if (y < x or b <= a) return 0;
        qq = x; w = y;
        return range(a, b, 0, s - 1, 1);
    }
 
    //Count occurrences of x in positions [0, k)
    inline int rank(int x, int k) {
        int l = 0, r = s - 1, u = 1, m, z;
        while (l != r) {
            m = (l + r) / 2;
            z = tree[u][k]; 
            u *= 2;
            if(x <= m) k = z, r = m;
            else k -= z, l = m + 1, ++u;
        }
        return k;
    }
     
    //x in [0, sigma)
    inline void push_back(int x) {
        int l = 0, r = s - 1, u = 1, m, p; 
        ++n;
        while (l != r) {
            m = (l + r)/2;
            p = (x <= m);
            tree[u].push_back(tree[u].back() + p);
            u *= 2;
            if(p) r = m; 
            else l = m + 1, ++u;
        }
    }
     
    //Assumes that array is non-empty
    inline void pop_back() {
        int l = 0, r = s - 1, u = 1, m, p, k; 
        --n;
        while (l != r) {
            m = (l + r) / 2; 
            k = tree[u].size();
            p = tree[u][k - 1] - tree[u][k - 2];
            tree[u].pop_back();
            u *= 2;
            if(p) r = m; 
            else l = m + 1, ++u;
        }
    }

    //swap arr[i] with arr[i + 1], i in [0, n - 1)
    inline void swap_adj(int i){
    	int &x = arr_copy[i], &y = arr_copy[i + 1];
		int l = 0, r = s - 1, u = 1;
		while(l != r){
            int m = (l + r) / 2, p = (x <= m), q = (y <= m);
            if (p != q){
                tree[u][i + 1] ^= tree[u][i] ^ tree[u][i + 2];
                break;
            }
            int z = tree[u][i]; 
            u *= 2;
            if(p) i = z, r = m;
            else i -= z, l = m + 1, ++u;
        }
        swap(x, y);
    }
};
 
int main() {

    int n, q;
    scanf("%d %d", &n, &q);
    vector < int > arr(n);
    for(int i = 0; i < n; i++) scanf("%d", &arr[i]);
 
    //Co-ordinate Compression
    vector < int > values;
    for(int i = 0; i < n; i++){
        values.push_back(arr[i]);
    }
    
    sort(values.begin(), values.end());
    values.resize(unique(values.begin(), values.end()) - values.begin());
    
    int sigma = 0;
    vector < int > orig(n);
    for(int i = 0; i < n; i++){
        int init = arr[i];
        arr[i] = lower_bound(values.begin(), values.end(), arr[i]) - values.begin();
        orig[arr[i]] = init;
        sigma = max(sigma, arr[i]);
    }

    /*
        1) Vector 'arr' represents the array 
        2) 'sigma' represents the alphabet size i.e [0, sigma + 1) in this case.
    */

    WaveTree wt(arr, sigma + 1);  

    for(int qq = 0; qq < q; qq++){
        int cmd, i, k;
        scanf("%d", &cmd);
        if(cmd){
        	scanf("%d", &i);
        	wt.swap_adj(i);
        }
        else{
	        scanf("%d %d", &i, &k);
	        // val = 'k'th smallest element in [0, i + 1)
	        int val = orig[wt.quantile(k, 0, i + 1)]; 
	        printf("%d\n", val);
	    }
    }
}