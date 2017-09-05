// IOITC 2015 Problem
/*
	DP[i] is a vector of length M denoting the number of arrays
	of length (i) for each 1 <= j <= M

	DP[2] = A * DP[1]
	DP[3] = B * DP[2] = B * (A * DP[1]) = (B * A) * DP[1]
	DP[4] = A * DP[3] = (A * (B * A) ) * DP[1]

	Let C = (B * A)

	A is an MxM matrix defined as follows
	for each (i, j) , if(j < i) A[i][j] = 1, else A[i][j] = 0

	B is an MxM matrix defines as follows
	for each (i, j) , if(j > i)  B[i][j] = 1, else B[i][j] = 0

	C is computed by multiplying B with A

	Ans = power(C, (N - 1) / 2)
	if( N is even ) Ans = multiply(A, Ans);
	We need to print sum of elements in Ans

	The complexity of this algorithm is O(M^3 log N)

	We need to find A[m]^K * C[m]^L * DP[1], where K <= 1, L <= 1000000 and m <= 40
	Precompute all powers of two of C[] matrix for each 1 <= m <= 40

	Now perform the multiplications in reverse order i.e. Instead of multiplying
	log N (MxM) matrices and multiplying the final result with a (Mx1) vector, start
	with multiplying the last (MxM) matrix with the (Mx1) vector and procees backwards.
	Thus you will keep getting a (Mx1) vector each time and you will perform only M^2
	operations while multiplying. Total complexity would abound to O(M ^ 2 log N)

*/

#include <bits/stdc++.h>
using namespace std;

const int MAX = 41;
const int LN  = 21;
const int MOD = (int)(1e9 + 7);

/*-------- Matrix Data Type ---------*/
struct matrix{
	int mat[MAX][MAX];
	matrix(int m = MAX - 1){
		for(int i = 1; i <= m; i++)
			for(int j = 1; j <= m; j++)
				mat[i][j] = 0;
	}
};

matrix A[MAX], B[MAX], C[MAX][LN], unit, temp;
int Q, N, M;

/*---- Modulo Addition and Modulo Multiplication ----*/
inline int add(int x, int y){
	int res = x + y;
	if(res >= MOD) res -= MOD;
	return res;
}

inline int prod(int x, int y){
	long long res = x * 1LL * y;
	if(res >= MOD) res %= MOD;
	return res;
}

/*--------- Matrix Multiplication of 2 Square Matrices ---------*/
inline matrix mult_sq(matrix p, matrix q, int m){
	matrix res(m);
	for(int i = 1; i <= m; i++)
		for(int k = 1; k <= m; k++)
			for(int j = 1; j <= m; j++)
				res.mat[i][j] = add(res.mat[i][j], prod(p.mat[i][k], q.mat[k][j]) );
	return res;
}

/*--------Precomputing Powers of C Matrix ---------*/
void precompute(int m){

	for(int i = 1; i <= m; i++)
		for(int j = 1; j < i; j++)
			A[m].mat[i][j] = 1;

	for(int i = 1; i <= m; i++)
		for(int j = i + 1; j <= m; j++)
			B[m].mat[i][j] = 1;

	C[m][0] = mult_sq(B[m], A[m], m);
	for(int i = 1; i < LN; i++)
		C[m][i] = mult_sq(C[m][i - 1], C[m][i - 1], m);
}


/*----- Multiplying a (m x m) and a (m x 1) matrix -----*/
matrix mult_sp(matrix x, matrix y, int m){
	matrix res(m);
	for(int i = 1; i <= m ; i++)
		for(int k = 1; k <= m ; k++)
			res.mat[i][1] = add(res.mat[i][1], prod(x.mat[i][k], y.mat[k][1]));
	return res;
}


// Note that this code might be rekt on local machine because of recursion with matrices
// It should work on judges tho

int main(){

	for(int i = 1; i <= 40; i++){
		unit.mat[i][i] = 1;
		precompute(i);
	}

	scanf("%d\n", &Q);

	while(Q--){

		scanf("%d %d\n", &N, &M);
		for(int i = 1 ; i <= M ; i++) temp.mat[i][1] = 1;

		int cur = (N - 1) >> 1;
		for(int i = 0 ; i <= 20 ; i++)
			if(cur & (1 << i)) temp = mult_sp(C[M][i], temp, M);

		if(N % 2 == 0) temp = mult_sp(A[M], temp, M);

		int res = 0;
		for(int i = 1; i <= M; i++) res = add(res, temp.mat[i][1]);
		printf("%d\n", res);
 	}
}
