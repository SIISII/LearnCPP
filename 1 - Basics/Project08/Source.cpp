#define  _CRT_SECURE_NO_WARNINGS

#include  <cstdio>

constexpr int  Qty = 10;


int  main()
{
    int  N[Qty];     // N[0], N[1], ... N[Qty-1]
    int  Sum = 0;
    int  Min;
    int  Max;

    for ( int  I = 0; I < Qty; ++I )
    {
        printf("\nEnter %i number = ", I);
        scanf("%i", &N[I]);
    }

    for ( int  I = 0; I < Qty; ++I )
    {
        Sum = Sum + N[I];
    }

    Min = N[0];
    for ( int  I = 1; I < Qty; ++I )
    {
        if ( Min > N[I] )
        {
            Min = N[I];
        }
    }

    Max = N[0];
    for ( int  I = 1; I < Qty; ++I )
    {
        if ( Max < N[I] )
        {
            Max = N[I];
        }
    }

    printf("Sum = %i\n", Sum);
    printf("Min = %i\n", Min);
    printf("Max = %i\n", Max);
    printf("Average = %f\n", float(Sum) / float(Qty));
}
