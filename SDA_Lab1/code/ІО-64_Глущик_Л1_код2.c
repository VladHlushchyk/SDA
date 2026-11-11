#include <stdio.h>

int main(void)
{
    double x, y;
    printf("x: ");
    scanf("%lf", &x);

    if ( x >= 2 && x <= 7 )
        y = x*x*x + 14;
    else if ( (x > -13 && x <= -3) || x > 14 )
        y = -4*x*x*x + 3*x - 7;
    else
    {
        printf("y() for your x doesn't exist\n");
        return -1;
    }

    printf("y = %lf\n", y);
    return 0;
}
