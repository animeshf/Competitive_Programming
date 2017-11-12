#include <bits/stdc++.h>
using namespace std;


/*
	gradient of (function we want to minimize/maximize) = lambda * (gradient of (constraint function))
*/


/*
	Example problem:

	Suppose you are in a 2D world. Your are in a system
	of N parallel zones in which you are allowed
	to travel at different maximum speeds in any direction
	Width of each zone is 100 along the Y. You are currently at
	the origin and want to reach (100 * N, D)

	----------------------------*goal
			max speed v3
	----------------------------
					  v2
	----------------------------
					  v1
start* ----------------------------
*/

/*
	Solution:
	Say that you travel di meters in the x direction within zone i.
	Then, we have the restriction that d1 + d2 + ... + dn = D
	We want to minimize the sum of sqrt(di^2 + 100^2) / vi for i = 1 to n

	We will need to binary search for the value of lambda
*/

const int MAX = 110;
int N, D;
double v[MAX];

double evaluate(double l) {
	double total = 0;
	for (int i = 0; i < N; i++) {
		double cur = sqrt((l*l *v[i]*v[i]* 100*100) / (1 - l*l*v[i]*v[i]));
		total += cur;
	}
	return total;
}

double get_time(double l) {
	double ans = 0;
	for (int i = 0; i < N; i++) {
		double dd = (l*l * v[i]*v[i]*100*100) / (1 - l*l*v[i]*v[i]);
		ans += sqrt(dd + 100*100) / v[i];
	}
	return ans;
}

int main() {

	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		cin >> N >> D;

		double mm = 0;
		for (int i = 0; i < N; i++) {
			cin >> v[i];
			mm = max(mm, v[i]);
		}

		double minim = 0, maxim = 1 / mm; // set correct bounds. Cant' have square root of a negative
		for (int aa = 0; aa < 400; aa++) {
			double lambda = (minim + maxim) / 2;
			double cur = evaluate(lambda);
			if (cur < D) {
				minim = lambda;
			} else {
				maxim = lambda;
			}
		}

		double lambda = minim;
		cout << fixed << setprecision(9);
		cout << "Case " << t << ": " << get_time(lambda) << "\n";
	}

	return 0;
}