#define  _CRT_SECURE_NO_WARNINGS

#include  <cstdio>

constexpr int  Qty = 10;


int  main()
{
    int  N[Qty];
    int  Sum = 0;

    FILE  *File = fopen("Data.txt", "r");

    for ( int  I = 0; I < Qty; ++I )
    {
        fscanf(File, "%i", &N[I]);
    }

    fclose(File);

    for ( int  I = 0; I < Qty; ++I )
    {
        Sum = Sum + N[I];
    }

    printf("Sum = %i\n", Sum);
}
