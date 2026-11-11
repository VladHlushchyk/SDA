#include <stdio.h>

int main(void)
{
    double x, y = 0;

    printf("x: ");
    scanf("%lf", &x);

    if (x >= 2)
    {
        if (x <= 7)
            y = x*x*x + 14;
        else if(x > 14)
            y = -4*x*x*x + 3*x - 7;
        else
        {
            printf("y() for your x doesn't exist.\n");
            return -1;
        }
    }
    else if (x > -13)
    {
        if (x <= -3)
            y = -4*x*x*x + 3*x - 7;
        else
        {
            printf("y() for your x doesn't exist.\n");
            return -1;
        }
    }
    else
    {
        printf("y() for your x doesn't exist.\n");
        return -1;
    }
        
    printf("y = %lf\n", y);
    return 0;
}