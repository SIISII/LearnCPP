#define  _CRT_SECURE_NO_WARNINGS

#include  <cstdio>
#include  <cmath>


int  main()
{
    float  A, B, C, D, X1, X2;

    printf("A = ");
    scanf("%f", &A);
    printf("B = ");
    scanf("%f", &B);
    printf("C = ");
    scanf("%f", &C);

    if ( A == 0 )
    {
        if ( B == 0 )
        {
            printf("Either A or B must not be 0\n");
        }
        else
        {
            printf("X = %f\n", -C / B);
        }
    }
    else
    {
        D = B*B - 4*A*C;

        if ( D < 0 )
        {
            printf("D must be greater than or equal to 0\n");
        }
        else
        {
            X1 = (-B + sqrt(D)) / (2*A);
            X2 = (-B - sqrt(D)) / (2*A);
            printf("X1 = %f\n", X1);
            printf("X2 = %f\n", X2);
        }
    }
}
