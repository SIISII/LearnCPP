#define  _CRT_SECURE_NO_WARNINGS

#include  <cstdio>
#include  "My_Lib.h"


constexpr int  Qty = 10;

int  main()
{
    int  N[Qty];

    FILE  *File = fopen("Data.txt", "r");

    for ( int  I = 0; I < Qty; ++I )
    {
        fscanf(File, "%i", &N[I]);
    }

    fclose(File);

    printf("Min = %i\n", Min(N, Qty));
    printf("Max = %i\n", Max(N, Qty));
}
