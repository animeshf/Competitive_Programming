// Mushroom Scientists -> Codeforces

#include <bits/stdc++.h>
using namespace std;

int s, a, b, c;

int main(){
	cin >> s >> a >> b >> c;
	cout << fixed << setprecision(15);
	double x = 0.0, y = 0.0, z = 0.0;
	if(a + b + c == 0) cout << x << ' ' << y << ' ' << z << '\n';
	else{
		x = (a * s * 1.0) / (a + b + c);
		y = (b * s * 1.0) / (a + b + c);
		z = (c * s * 1.0) / (a + b + c);
		cout << x << ' ' << y << ' ' << z << '\n';
	}
}
