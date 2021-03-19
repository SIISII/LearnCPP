#define  _CRT_SECURE_NO_WARNINGS

#include  <cstdio>

#include  "My_Lib.h"


int  main()
{
    int  Sum = 0;
    int  Count = 0;
    int  Minimal;
    int  Maximal;

    FILE  *File = fopen("Data.txt", "r");

    while ( !feof(File) )
    {
        int  N;

        if ( fscanf(File, "%i", &N) == 1 )
        {
            if ( Count == 0 )
            {
                Minimal = N;
                Maximal = N;
            }
            else
            {
                Minimal = Min(Minimal, N);
                Maximal = Max(Maximal, N);
            }

            Sum += N;
            ++Count;
        }
    }

    fclose(File);

    printf("Count = %i\n", Count);
    printf("Sum = %i\n", Sum);
    printf("Min = %i\n", Minimal);
    printf("Max = %i\n", Maximal);
}
