#include "bits/stdc++.h"
using namespace std;

// Don't use this template, you coded this and you suck.

/*

	So basically you have N linear equations, each containing the same N variables.
	Each equation is of the form C = c1 * x1 + c2 * x2 + .. + cN * xN, where C is a
	constant(res[]), and c1...cN are the coefficients (co[][]), and x1...xN are the
	variables.

	We will go in turns, our goal in the i'th turn is to eliminate xi from all equations.
	For that we find one equation in which ci != 0, and since any linear combination of
	two equations is still true, we can add/subtract this equation to/from the other
	equations the correct times, to make ci == 0 in all of the other equations.

	Now we will continue with the remaining equations (N - 1 equations with N - 1 variables)
	Recursively eliminate until we have just one equation for 1 var, which we can solve.
	Then go backwards and substitute the already known variables into the equations
	so we will find out the value of each variable one by one.

	So if a unique solution exists, we will have N reduced equation of the following form :-
	x_N = constant_1
	x_Nc_N + x_N-1c_N-1 = constant_2
	x_Nd_N + x_N-1d_N-1 + x_N-2d_N-2 = constant_3
	....
	We can process these equations from top to bottom to uncover the values of all variables!

	Note, that in the equations that are used to eliminate a variable, I don't eliminate
	any further variables. You use one equation to eliminate x_i from all the other
	equations *that has not been used before*.

	(i.e if you use equation j to eliminate i from all equations,
	x_k will not be eliminated from j, if k >= i)

*/

struct gaussian_elimination{

	static const double EPS = 0.000001;

    inline double abs_val(double &d){
        return (d < 0.0F) ? -d : d;
    }

    inline bool is_zero(double d){
        return abs_val(d) < EPS;
    }

    static const int VAR = 100;

    int n;
    double co[VAR][VAR]; // coefficients
    double ans[VAR]; // value of each variable
    double res[VAR]; // constant part of each equation
    bool rekt; // is true when unique solution does not exist
    bool u[VAR]; // equation already used to eliminate some variable
    int used[VAR]; // equation used to eliminate the i'th var

    inline void read() {
        for(int i = 0; i < VAR; i++){
        	u[i] = false;
        	used[i] = 0;
        	res[i] = ans[i] = 0.0;
        }
        cin >> n;
        for(int i = 0; i < n; i++){
            cin >> res[i];
            for(int j = 0; j < n; j++)
                cin >> co[i][j];
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

    inline void print(){
        if(rekt) return;
        for(int i = 0; i < n; i++)
            cout << ans[i] << '\n';
        return;
    }
};

gaussian_elimination obj;

int main() {
    obj.read();
    obj.run();
    obj.print();
}