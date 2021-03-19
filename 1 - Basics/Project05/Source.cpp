#define  _CRT_SECURE_NO_WARNINGS

#include  <cstdio>


int  main()
{
    float  A, B;

    printf("A = ");
    scanf("%f", &A);
    printf("B = ");
    scanf("%f", &B);

    if ( A == 0 )
    {
        printf("Error\n");
    }
    else
    {
        printf("\nX = %f\n", -B / A);
    }
}
