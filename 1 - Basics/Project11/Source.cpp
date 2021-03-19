#define  _CRT_SECURE_NO_WARNINGS

#include  <cstdio>


constexpr int  Qty = 10;

int  main()
{
    int  N[Qty];
    int  Min;
    int  Max;

    FILE  *File = fopen("Data.txt", "r");

    for ( int  I = 0; I < Qty; ++I )
    {
        fscanf(File, "%i", &N[I]);
    }

    fclose(File);

    Min = N[0];
    Max = N[0];

    for ( int  I = 1; I < Qty; ++I )
    {
        if ( N[I] < Min )
        {
            Min = N[I];
        }
        if ( N[I] > Max )
        {
            Max = N[I];
        }
    }

    printf("Min = %i\n", Min);
    printf("Max = %i\n", Max);
}
