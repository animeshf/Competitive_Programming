#include "bits/stdc++.h"
using namespace std;

const int N = 1e6 + 6; 

char a[N], b[N], str[N * 2];
int t, z[N * 2];

inline void z_function(int n){
	memset(z, 0, sizeof z);
	for(int i = 1, l = 0, r = 0; i < n; i++){
		if(i <= r) z[i] = min(z[i - l], r - i + 1);
		while(i < n && str[z[i]] == str[i + z[i]]) ++z[i];
		if(i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
	}
}

int main(){
	freopen("ioi.in", "r", stdin);
	scanf("%d", &t);
	for(int qq = 1; qq <= t; qq++){
		scanf("%s %s", a, b);
		int len1 = strlen(a), len2 = strlen(b);
		for(int i = 0; i < len2; i++) str[i] = b[i];
		str[len2] = '$';
		for(int i = 0; i < len1; i++) str[i + len2 + 1] = a[i];
		z_function(len1 + len2 + 1);
		int ans = 0;
		for(int i = len2 + 1; i < len2 + len1 + 1; i++) ans += (z[i] == len2);
		printf("Case %d: %d\n", qq, ans);
	}
}
