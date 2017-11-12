
int log_table[N], mx[LN][N], mn[LN][N];
inline void preprocess(){
	log_table[1] = 0;
	for(int i = 2; i <= n; i++) log_table[i] = log_table[i >> 1] + 1;
	
	for(int i = 1; i <= n; i++) mx[0][i] = a[i];
	for(int i = 1; i < LN; i++)
		for(int j = 1; j + (1 << i) - 1 <= n; j++)
			mx[i][j] = max(mx[i - 1][j] , mx[i - 1][j + (1 << (i - 1))]);
	
	for(int i = 1; i <= n; i++) mn[0][i] = b[i];
	for(int i = 1; i < LN; i++)
		for(int j = 1; j + (1 << i) - 1 <= n; j++)
			mn[i][j] = min(mn[i - 1][j] , mn[i - 1][j + (1 << (i - 1))]);
}

inline int get_max(int l, int r){
	int k = log_table[r - l + 1];
	return max(mx[k][l] , mx[k][r - (1 << k) + 1]);
}

inline int get_min(int l, int r){
	int k = log_table[r - l + 1];
	return min(mn[k][l] , mn[k][r - (1 << k) + 1]);
}
