#define  _CRT_SECURE_NO_WARNINGS

#include  <cstdio>


int  Min(
    int  N[],
    int  Q)
{
    int  M = N[0];

    for ( int  I = 1; I < Q; ++I )
    {
        if ( N[I] < M )
        {
            M = N[I];
        }
    }

    return M;
}


int  Max(
    int  N[],
    int  Q)
{
    int  M = N[0];

    for ( int  I = 1; I < Q; ++I )
    {
        if ( N[I] > M )
        {
            M = N[I];
        }
    }

    return M;
}


constexpr int  Qty = 10;

int  main()
{
    int  P[Qty];

    FILE  *File = fopen("Data.txt", "r");

    for ( int  I = 0; I < Qty; ++I )
    {
        fscanf(File, "%i", &P[I]);
    }

    fclose(File);

    printf("Min = %i\n", Min(P, Qty));
    printf("Max = %i\n", Max(P, Qty));
}
