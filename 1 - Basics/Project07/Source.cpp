#define  _CRT_SECURE_NO_WARNINGS

#include  <cstdio>

constexpr int  Qty = 10;


int  main()
{
    int  N[Qty];     // N[0], N[1], ... N[Qty-1]
    int  Sum = 0;

    for ( int  I = 0; I < Qty; ++I )
    {
        printf("\nEnter %i number = ", I);
        scanf("%i", &N[I]);
    }

    for ( int  I = 0; I < Qty; ++I )
    {
        Sum = Sum + N[I];
    }

    printf("Sum = %i\n", Sum);
    printf("Average = %f\n", static_cast<float>(Sum) / static_cast<float>(Qty) );
}
