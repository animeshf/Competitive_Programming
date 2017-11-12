#include <bits/stdc++.h>
using namespace std;
 

// ------ KMP Template -------

const int MAX_LEN = 1e5 + 5; 
int lps[MAX_LEN];

// lps[] table is 1 based, strings are 0 based.
inline void compute_table(string &pattern) {
	lps[0] = -1, lps[1] = 0;
	int pref = 0;
	for (int i = 2; i <= pattern.size(); i++) {
		while (pref != -1 && pattern[i - 1] != pattern[pref]) {
			pref = lps[pref];
		}
		pref++;
		lps[i] = pref;
	}
}

// Function returns frequency of 'pattern' in 'text'
inline int kmp(string &text, string &pattern){
	compute_table(pattern);
	int pref = 0, count = 0;
	for (int i = 0; i < text.size(); i++) {
		while (pref != -1 && text[i] != pattern[pref]) {
			pref = lps[pref];
		}
		pref++;
		if (pref == pattern.size()) {
			pref = lps[pref];
			count++;
		}
	}
	return count;
}

// ---- End of KMP Template ----
 
int main() {
	string text, pattern;
	while (cin >> text >> pattern) {
		cout << kmp(text, pattern) << '\n';
	}
}