// Some Codeforces Educational Round

#include "bits/stdc++.h"
using namespace std;


/*------ Matrix Exponentiation Template ------*/

const int ORD = 2; // Order of Square Matrix
const int MOD = 1000000007; // Modulo

inline int prod(int x, int y){
	long long res = x * 1LL * y;
	if(res >= MOD) res %= MOD;
	return res;
}

inline int add(int x, int y){
	int res = x + y;
	if(res >= MOD) res -= MOD;
	return res;
}

struct matrix{
	int mat[ORD][ORD];
	matrix(){
		for(int i = 0; i < ORD; i++)
			for(int j = 0; j < ORD; j++)
				mat[i][j] = 0;
	}
	friend matrix operator * (matrix x, matrix y){
		matrix res;
		for(int i = 0; i < ORD; i++)
			for(int j = 0; j < ORD; j++)
				for(int k = 0; k < ORD; k++)
					res.mat[i][j] = add(res.mat[i][j], prod(x.mat[i][k], y.mat[k][j]));
		return res; 
	}
};

matrix base;

// power(n) returns base ^ {n}
matrix power(matrix cur, long long p){
	if(p == 1) return base;
	matrix res = power(cur, p >> 1);
	res = res * res;
	if(p & 1) res = res * base;
	return res;
}

/*------------ End of Template -------------*/


int a, b, x;
long long n;

int main(){
	freopen("ioi.in", "r", stdin);
	cin >> a >> b >> n >> x;
	base.mat[0][0] = a, base.mat[0][1] = 1;
	base.mat[1][0] = 0, base.mat[1][1] = 1;
	matrix result = power(base, n);
	cout << add(prod(result.mat[0][0], x), prod(result.mat[0][1], b)) << '\n';
}
