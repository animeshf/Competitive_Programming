// SWERC 2016 - Passwords

#include "bits/stdc++.h"
using namespace std;

/*
    ------------ Aho-Corasick Template ----------

    This is an implementation of Aho-Corasick Algorithm to perform multiple pattern
    matching efficiently. The function aho_corasick() accepts a dictionary of strings
    and creates an automaton on those strings. Each node in the automaton represents
    a prefix of one of the strings in the dictionary. The root, indexed as 0, denotes
    the empty string.

    The following arrays are useful:

    - to[node][c]: Which node in the automaton would we be at if we append the character
      'c' to the end of the string represented by 'node'. This is the longest suffix
      of the new string that appears as a prefix in the automaton.

    - link[node]: Link to the longest proper suffix of the string represented by 'node'

    - isTerminal[node]: 'True' if any suffix (not necessarily proper) of 'node' exists
                        in the dictionary provided to us, 'False' otherwise.

    Implementation Details:

    - M denotes the maximum number of nodes possible in the automaton, which is given by
      summation of lengths of all strings in our dictionary.

    - SIGMA denotes the size of our alphabet, which is usually 26.

    - INVALID denotes a transition character that does not appear in our alphabet. Use the
      variable "INVALID" in your code to denote such a character.

    Time Complexity:

    - O(length(s)) for add_str(s)
    - O(M * SIGMA) = O(M) for push_links()

    -----------------------------------------------
*/

const int M = 2000;
const int SIGMA = 26;
const int INVALID = SIGMA;

bool isTerminal[M];
int timer;
int to[M][SIGMA + 1], link[M];

inline void add_str(string s) {
    int cur = 0;
    for (char c: s) {
        if (!to[cur][c - 'a']) {
            to[cur][c - 'a'] = ++timer;
        }
        cur = to[cur][c - 'a'];
    }
    isTerminal[cur] = true;
}

void push_links() {
    queue < int > q;
    q.push(0);
    while(!q.empty()) {
        int V = q.front();
        q.pop();
        int U = link[V];
        if (!isTerminal[V]) {
            isTerminal[V] = isTerminal[U];
        }
        for (int c = 0; c < SIGMA; c++) {
            if (to[V][c]) {
                link[to[V][c]] = V ? to[U][c] : 0;
                q.push(to[V][c]);
            } else {
                to[V][c] = to[U][c];
            }
        }
    }
}

inline void aho_corasick(vector < string > patterns) {
    // Cleanup.
    timer = 0;
    memset(isTerminal, false, sizeof(isTerminal));
    memset(to, 0, sizeof(to));
    memset(link, 0, sizeof(link));
    // Add strings to automata.
    for (string s: patterns) {
        add_str(s);
    }
    for (int node = 0; node <= timer; node++) {
        // Appending an INVALID character from any node
        // should lead us back to the root node, giving us no match.
        to[node][INVALID] = 0;
    }
    // Compute Suffix Links.
    push_links();
}

// End of Aho Corasick Template.

const int N = 25;
const int MOD = 1000003;

inline int add(int x, int y) {
    int res = x + y;
    if (res >= MOD) {
        res -= MOD;
    }
    return res;
}

int dp[N][2][2][2][M];
map < char, int > leet;

int compute(int len, bool u, bool l, bool d, int node) {
    if (isTerminal[node]) {
        return 0;
    }
    int val = dp[len][u][l][d][node];
    if (val != -1) {
        return val;
    }
    if (len == 0) {
        return (u & l & d);
    }
    // Upper Case Letter
    val = 0;
    for (char c = 'A'; c <= 'Z'; c++) {
        int transition_node = to[node][c - 'A'];
        val = add(val, compute(len - 1, 1, l, d, transition_node));
    }
    // Lower Case Letter
    for (char c = 'a'; c <= 'z'; c++) {
        int transition_node = to[node][c - 'a'];
        val = add(val, compute(len - 1, u, 1, d, transition_node));
    }
    // Digit
    for (char c = '0'; c <= '9'; c++) {
        int transition_node = to[node][leet[c]];
        val = add(val, compute(len - 1, u, l, 1, transition_node));
    }
    return dp[len][u][l][d][node] = val;
}

inline int solve(int len) {
    int root = 0;
    return compute(len, 0, 0, 0, root);
}

int main() {
    ios :: sync_with_stdio(false);
    int l, r; cin >> l >> r;
    int n; cin >> n;
    vector < string > patterns;
    for (int i = 0; i < n; i++) {
        string x; cin >> x;
        patterns.push_back(x);
    }
    aho_corasick(patterns);
    for (char d = '0'; d <= '9'; d++) {
        leet[d] = INVALID;
    }
    leet['0'] = 'o' - 'a';
    leet['1'] = 'i' - 'a';
    leet['3'] = 'e' - 'a';
    leet['5'] = 's' - 'a';
    leet['7'] = 't' - 'a';
    memset(dp, -1, sizeof dp);
    int ans = 0;
    for (int i = l; i <= r; i++) {
        ans = add(ans, solve(i));
    }
    cout << ans << endl;
}