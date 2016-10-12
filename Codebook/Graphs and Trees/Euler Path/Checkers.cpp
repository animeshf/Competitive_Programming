// You can check for existence of an euler path by counting number of odd-degree vertices
// Checkers - ICPC 2015 SER
#include "bits/stdc++.h"
using namespace std;

const int N = 50;

string str[N];
int n, cur_time;
int vis[N][N], deg[N][N];
int dx[] = {-1, -1, 1, 1};
int dy[] = {-1, 1, -1, 1};
map < pair < pair < int, int >, pair < int, int > >, int > join;

inline bool isValid(int x, int y){
	return (x >= 0 and x < n and y >= 0 and y < n);
}

inline void dfs(int x, int y, int px, int py){
	vis[x][y] = ++cur_time;
	for(int i = 0; i < 4; i++){
		int nx = x + dx[i], ny = y + dy[i];
		int cx = nx + dx[i], cy = ny + dy[i];
		if(isValid(nx, ny) && str[nx][ny] == 'W' && isValid(cx, cy) && str[cx][cy] == '_'){
			if(px == cx and py == cy) continue;
			if(join.find(make_pair(make_pair(x, y), make_pair(cx, cy))) != join.end()) continue;
			if(join.find(make_pair(make_pair(cx, cy), make_pair(x, y))) != join.end()) continue;
			join[make_pair(make_pair(x, y), make_pair(cx, cy))] = true;
			join[make_pair(make_pair(cx, cy), make_pair(x, y))] = true;
			++deg[x][y], ++deg[cx][cy];
			if(!vis[cx][cy]){
				dfs(cx, cy, x, y);	
			}
		}
	}
}

inline bool can(int x, int y){
	memset(vis, 0, sizeof vis);
	memset(deg, 0, sizeof deg);
	join.clear();
	cur_time = 0;
	dfs(x, y, x, y);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(str[i][j] == 'W'){
				if(i == 0 || i == n - 1 || j == 0 || j == n - 1) return false;
				bool ok = (vis[i - 1][j - 1] && vis[i + 1][j + 1]);
				ok |= (vis[i - 1][j + 1] && vis[i + 1][j - 1]);
				if(!ok) return false;
			}
		}
	}
	int count_odd = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(vis[i][j])
				count_odd += (deg[i][j] & 1);
		}
	}
	assert(count_odd % 2 == 0);
	if(count_odd > 2) return false;
	if(count_odd == 2){
		if(deg[x][y] & 1) return true;
		return false;
	}
	return true;
}

int main(){
	freopen("ioi.in", "r", stdin);
	cin >> n;
	for(int i = 0; i < n; i++){
		cin >> str[i];
	}
	vector < string > pieces;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(str[i][j] == 'B'){
				if(can(i, j))
					pieces.push_back(char('a' + j) + to_string(n - i));
			}
		}
	}
	if((int) pieces.size() == 1) cout << pieces[0] << '\n';
	else if((int) pieces.size() > 1) cout << "Multiple\n";
	else cout << "None\n";
}