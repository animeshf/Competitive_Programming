// Base Sums - SER 2016

#include "bits/stdc++.h"
using namespace std;

const int N = 100;

int num_digits[2];
int digits[2][N];
int sum_digits[2][N];
int increase[2][N];
long long val[2][N];

inline long long power(int base, int exp) {
    if (exp == 0) {
        return 1;
    }
    if (exp == 1) {
        return base;
    }
    long long z = power(base, exp >> 1);
    z = z * z;
    if (exp & 1) {
        z *= base;
    }
    return z;
}

inline void compute(int id, long long number, int base) {
    num_digits[id] = 0;
    while (number) {
        int digit = number % base;
        digits[id][++num_digits[id]] = digit;
        number /= base;
    }
    for (int i = 1; i <= num_digits[id]; i++) {
        sum_digits[id][i] = sum_digits[id][i - 1] + digits[id][i];
        increase[id][i] = increase[id][i - 1] + (base - 1) - (digits[id][i]);
        val[id][i] = val[id][i - 1] + (power(base, i - 1) * digits[id][i]);
    }
}


int main() {
    freopen ("inp.in", "r", stdin);
    freopen ("pattern.out", "w", stdout);
    long long n;
    int a, b;
    cin >> n >> a >> b;
    long long m = n + 1;
    while (true) {
        compute(0, m, a);
        compute(1, m, b);
        int sid = 0, lid = 1;
        int sum_digits_small = sum_digits[sid][num_digits[sid]];
        int sum_digits_large = sum_digits[lid][num_digits[lid]];
        if (sum_digits_small == sum_digits_large) {
            cout << m << endl;
            return 0;
        }
        if (sum_digits_small > sum_digits_large) {
            swap(sum_digits_small, sum_digits_large);
            swap(sid, lid);
        }
        long long increment = 1;
        // We'll make the first (i) digits of the smaller number the maximium it can be i.e. base - 1.
        for (int i = 1; i <= num_digits[sid]; i++) {
            // We'll make the first (j) digits of the larger number the minimum it can be i.e. 0.
            // and increment the j+1th digit by 1

            /**
                Let x be the smaller number, and y be the larger number.

                Suppose an increment of c is required to make the first (i) digits of x equal to base
                (base of smaller number) - 1. Suppose the sum of digits of (x + c) = S_x.

                Then for any for increment d <= c, sum_of_digits(x + d) <= S_x.

                Suppose an increment of f (f > 0) is required to make the first (j) digits of y equal to 0,
                and increase the (j + 1)^(th) digit of y by one. Suppose the sum of digits of (y + f) = S_y.

                Then for any increment e <= f, sum_of_digits(y + e) >= S_y.

                Thus, if S_x < S_y, we can definitely increment our candidate for the answer by min(c, f).

            **/

            for (int j = 1; j <= num_digits[lid]; j++) {
                int increase_sum_of_digits_small = increase[sid][i];
                int new_sum_of_digits_small = sum_digits_small + increase_sum_of_digits_small;
                int decrease_sum_of_digits_large = sum_digits[lid][j];
                int new_sum_of_digits_large = sum_digits_large - decrease_sum_of_digits_large + 1;
                // If after incrementing the smaller number, it still remains smaller, then
                // we have some lower bound on the increment.
                if (new_sum_of_digits_small < new_sum_of_digits_large) {
                    // How much do I need to increase smaller number by to get the first (i) digits = base - 1
                    long long compute_delta_small = power(sid == 1 ? b : a, i) - val[sid][i];
                    // How much do I need to increment larger number by to get the first (j) digits = 0
                    // and the (j + 1) th digit increases by one.
                    long long compute_delta_large = power(lid == 1 ? b : a, j) - val[lid][j];
                    // We can add the minimum of this safely.
                    increment = max(increment, min(compute_delta_small, compute_delta_large));
                }
            }
        }
        cout << increment << endl;
        m += increment;
    }
}