#include <stdio.h>

long long ops = 0;

double theoretical(double x, int n)
{
    double cur = 1.0;
    double res = cur;
    ops += 2;

    ops += 2;
    for (int i = 1; i <= n; ++i)
    {
        ops += 2;

        double k = (double)(n - i + 1) / i * x;
        ops += 5;

        cur *= k;
        ops += 2;

        res += cur;
        ops += 2;
    }
    return res;
}

int main(void)
{
    const double x = 0.2;
    printf("X: %.7f;\n", x);

    int n;
    do {
        printf("\nEnter n: ");
        scanf("%d", &n);
    } while (n <= 0);

    double exact_val = 1.0;
    for (int i = 0; i < n; ++i) {
        exact_val *= (1.0 + x);
    }

    ops = 0;
    double theoretical_val = theoretical(x, n);
    double difference = exact_val - theoretical_val;

    printf("Exact value: %.7f\n", exact_val);
    printf("Theoretical value: %.7f\n", theoretical_val);
    printf("Difference: %g\n", difference);
    printf("Operations: %lld\n", ops);

    return 0;
}