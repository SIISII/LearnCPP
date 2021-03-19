#define  _CRT_SECURE_NO_WARNINGS

#include  <cstdio>

constexpr int  Qty = 10;


int  main()
{
    int   N[Qty];
    bool  B[Qty];

    FILE  *File = fopen("Data.txt", "r");

    for ( int  I = 0; I < Qty; ++I )
    {
        fscanf(File, "%i", &N[I]);
        B[I] = false;
    }

    fclose(File);

    for ( int  I = 0; I < Qty-2; ++I )
    {
        bool  Found = false;
        int   S = N[I];

        if ( B[I] )
        {
            continue;
        }

        for ( int  J = I+1; J < Qty; ++J )
        {
            if ( N[J] == S )
            {
                B[J] = true;
                Found = true;
            }
        }

        if ( Found )
        {
            printf("Found duplicates for %i\n", S);
            B[I] = true;
        }
    }
}
