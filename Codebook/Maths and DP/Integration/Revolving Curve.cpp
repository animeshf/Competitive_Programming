#include <bits/stdc++.h>
using namespace std;

const long double PI = acos(-1);
int N, A, B, slices, stacks;
int coef[100], sq_coef[100];
double ind_coef[100];

double power_(double x, int y) {
    double ans = 1;
    for (int i = 0; i < y; i++)
        ans *= x;
    return ans;
}

void square_pol() {
    memset(sq_coef, 0, sizeof(sq_coef));

    for (int i = 0; i <= N; i++) {
        for (int k = 0; k <= N; k++) {
            int c = i + k;
            sq_coef[c] += coef[i] * coef[k];
        }
    }
}

double evaluate(double x) {
    double ans = 0;
    for (int i = 0; i <= N; i++) {
        ans += coef[i] * power_(x, i);
    }
    return ans;
}

double indefinite_evaluate(double x) {
    double ans = 0;
    for (int i = 0; i <= 2*N + 1; i++) {
        ans += ind_coef[i] * power_(x, i);
    }
    return ans;
}

void indefinite_integral() {
    for (int i = 2*N + 1; i > 0; i--) {
        ind_coef[i] = sq_coef[i-1]*1.0 / (i);
    }
}

double definite_integral() {
    square_pol();
    indefinite_integral();
    return PI * (indefinite_evaluate(B) - indefinite_evaluate(A));
}

double approximate() {
    double theta = 2*PI / slices;
    double width = (B - A)* (1.0) / stacks * 1.0;

    double volume = 0;
    double start_ = A, end_ = A + width;
    for (int i = 0; i < stacks; i++) {

        double px1 = evaluate(start_);
        double px2 = evaluate(end_);
        double m = (px2 - px1) / (end_ - start_);
        double c = px2 - m * end_;

        volume += (end_ - start_) * c * c;
        volume += m * c * (end_ * end_ - start_ * start_);
        volume += m * m * (end_*end_*end_ - start_*start_*start_) / 3;

        start_ += width;
        end_ += width;
    }

    return volume * sin(theta) / 2;
}

int main() {

    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        cin >> N;
        for (int i = N; i >= 0; i--) {
            cin >> coef[i];
        }

        cin >> A >> B;
        cin >> slices >> stacks;

        double real_volume = definite_integral();
        double app_volume = approximate() * slices;

        cout << "Case " << t + 1 << ": ";
        cout << fixed << setprecision(4);
        cout << fabs((real_volume - app_volume) / real_volume) * 100.0 << "\n";
    }

    return 0;
}