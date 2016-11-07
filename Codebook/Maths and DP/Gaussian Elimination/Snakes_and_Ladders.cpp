#include "bits/stdc++.h"
using namespace std;

/*
	dp[i]  = Expected number of steps to reach the goal (99) from the i'th cell.
	dp[99] = 0.0;
	
	to[i] -> the cell one reaches if one lands on cell i.
	
	dp[i]  = (1 / 6) * (dp[to[i + 1]] + dp[to[i + 2]] ... + dp[to[i + 6]]) + 1
	Notice that this relation can be cyclic because of the snakes and ladders.
	Hence you cannot do simple bottom up dp!
	
	We can set up n linear equations of the form :-
	dp[i] - p1(dp[to[i + 1]]) - p2(dp[to[i + 2]]) ... - p6(dp[to[i + 6]]) = 1
	
	Coefficients are 1, -p1, -p2...-p6 ------> 1
	So you have a system of 100 equations with 100 variables each (albeit most
	of the coefficients are zero). You have to solve this set of equations to
	compute the value of dp[0]

	It is guaranteed that a solution will exist since the game will end in some
	finite number of steps.

	Implementation notes :-
	1) to[i + x] = i if (i + x) > 99 (since you stay at the same place)
	2) Handle base case separately -> result[99] = 0 since dp[99] = 0 (special case!)
*/

const int N = 100;

int to[N];
double coefficients[N][N], result[N];
int t, n, u, v;
		
struct gaussian_elimination{

	static const double EPS = 0.000001;

    inline double abs_val(double &d){
        return (d < 0.0F) ? -d : d;
    }

    inline bool is_zero(double d){
        return abs_val(d) < EPS;
    }

    static const int VAR = N;

    int n;
    double co[VAR][VAR]; // coefficients
    double ans[VAR]; // value of each variable
    double res[VAR]; // constant part of each equation
    bool rekt; // is true when unique solution does not exist
    bool u[VAR]; // equation already used to eliminate some variable
    int used[VAR]; // equation used to eliminate the i'th var
    
    inline void read(int _n, double _res[VAR], double _co[VAR][VAR]) {
    	rekt = false;
    	n = _n;
        for(int i = 0; i < n; i++){
        	u[i] = false;
        	used[i] = 0;
        	ans[i] = 0.0;
        }
        for(int i = 0; i < n; i++){
            res[i] = _res[i];
            for(int j = 0; j < n; j++)
                co[i][j] = _co[i][j];
        }
    }

    inline void run(){
        for(int i = 0; i < n; i++){	
            used[i] = -1;
            for(int j = 0; j < n; j++){
                if(u[j]) continue;
                if(is_zero(co[j][i])) continue;
                used[i] = j;
                break;
            }
            if(used[i] < 0){ //variable already eliminated from all the equations
                cout << "Linearly dependent equations found!" << endl;
                rekt = true;
                return;
            }
            u[used[i]] = 1;
            for(int j = 0; j < n; j++){
                if(u[j]) continue;
                if(is_zero(co[j][i])) continue;
                //eliminating variable i from equation j
                double C = co[j][i] / co[used[i]][i];
                for(int k = 0; k < n; k++){
                	co[j][k] -= C * co[used[i]][k];
                }
                res[j] -= C * res[used[i]];
            }
        }

        for(int i = n - 1; i >= 0; i--){
            for(int j = i + 1; j < n; j++){
                res[used[i]] -= co[used[i]][j] * ans[j];
                co[used[i]][j] = 0;
            }
            res[used[i]] /= co[used[i]][i];
            co[used[i]][i] = 1.0F;
            ans[i] = res[used[i]];
        }
    }

    inline void print(int case_no){
        if(rekt){
        	cout << "Bug in code!\n";
        	return;
        };
        cout << "Case " << case_no << ": " << fixed << setprecision(10) << ans[0];
        cout << '\n'; 
    }
};


int main(){
	freopen("ioi.in", "r", stdin);
	cin >> t;
	for(int qq = 1; qq <= t; qq++){

		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				coefficients[i][j] = 0.0;
			}
			to[i] = i;
			result[i] = 1.0;
		}
		result[N - 1] = 0.0; // For last cell, equation is different.

		cin >> n;
		for(int i = 1; i <= n; i++){
			cin >> u >> v;
			u--, v--;
			to[u] = v;
		}

		double p = (1.0 / 6.0);
		for(int i = 0; i < N - 1; i++){
			coefficients[i][i] = 1.0;
			for(int j = 1; j <= 6; j++){
				if(i + j < N)
					coefficients[i][to[i + j]] -= p; // I go to (to[i + j])
				else
					coefficients[i][i] -= p; // I stay at the same place!
			}
		}
		coefficients[N - 1][N - 1] = 1.0;

		gaussian_elimination ge;
		ge.read(N, result, coefficients);
		ge.run();
		ge.print(qq);
	}
}