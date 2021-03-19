#define  _CRT_SECURE_NO_WARNINGS

#include  <cstdio>


int  main()
{
    int  Sum = 0;

    FILE  *File = fopen("Data.txt", "r");

    while ( !feof(File) )
    {
        int  N;

        fscanf(File, "%i", &N);
        Sum += N;
    }

    fclose(File);

    printf("Sum = %i\n", Sum);
}
