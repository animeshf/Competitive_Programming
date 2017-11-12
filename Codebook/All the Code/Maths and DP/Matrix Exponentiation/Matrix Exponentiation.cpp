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

matrix exponentiate(matrix cur, long long p){
    matrix result;
    for (int i = 0; i < ORD; i++) {
        result.mat[i][i] = 1;
    }
    if (p == 0) {
        return result;
    }
    if (p == 1) {
        return base;
    }
    while (p > 0) {
        if (p & 1) {
            result = result * base;
        }
        base = base * base;
        p >>= 1;
    }
    return result;
}

// power(n) returns cur^{n}
matrix power(matrix cur, long long p) {
    base = cur;
    return exponentiate(cur, p);
}

/*------------ End of Template -------------*/

int main(){
	matrix fib;
	fib.mat[0][0] = 1, fib.mat[0][1] = 1;
	fib.mat[1][0] = 1, fib.mat[1][1] = 0;
	matrix result = power(fib, 10);
	cout << result.mat[0][0] << ' '  << result.mat[0][1] << endl;
	cout << result.mat[1][0] << ' '  << result.mat[1][1] << endl;
}
