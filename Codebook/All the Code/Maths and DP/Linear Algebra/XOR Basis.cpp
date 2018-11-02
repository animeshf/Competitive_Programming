// The following code finds the XOR basis of a given set of integers
// The basis is stored in b[] in increasing order, and is normalized
// to be as similar as possible to the identity basis. Formally, every
// number in b[] has exactly one set bit.

// get(k): returns the kth smallest distinct subset XOR that can be
// made with the elements that we considered while making the basis.
// So for k = 1, answer is the empty set with xor zero.
// And for k = 2, and k = 2 --> b[0].
// Note that the number of distinct subset XORs is exactly 2^{sz}.

// This implementation works for 32 bit numbers, but can be made to
// work for 64 bit numbers by replacing int with long long everywhere.

#include <bits/stdc++.h>
using namespace std;

int b[32];
int sz = 0;

void add(int x) {
    for(int i = 0; i < sz; i++)
        if((x ^ b[i]) < x)
            x ^= b[i];
    for(int i = 0; i < sz; i++)
        if((x ^ b[i]) < b[i])
            b[i] ^= x;
    if(x) {
        b[sz++] = x;
        for(int i = sz - 1; i; i--)
            if(b[i] < b[i - 1])
                swap(b[i], b[i - 1]);
    }
}

int get(int k) {
    k--;
    int ans = 0;
    for(int i = 0; i < sz; i++)
        if((k >> i) & 1)
            ans ^= b[i];
    return ans;
}

int main() {
    ios :: sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    while(n--) {
        int t, x;
        cin >> t >> x;
        if(t == 1)
            add(x);
        else
            cout << get(x) << "\n";
    }
}
