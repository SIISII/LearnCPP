#define  _CRT_SECURE_NO_WARNINGS

#include  <cstdio>


struct  Person
{
    char  First_Name[30];
    char  Last_Name[30];
    int   Age;
};


bool  Input(Person  &S)
{
    printf("\nEnter an age: ");
    scanf("%i", &S.Age);
    if ( S.Age < 0 )
    {
        return false;
    }

    printf("Enter the first name: ");
    scanf("%s", S.First_Name);

    printf("\nEnter the last name: ");
    scanf("%s", S.Last_Name);

    return true;
}


int  main()
{
    Person  P;

    FILE  *File = fopen("Data.txt", "w+");

    while (true)
    {
        if ( !Input(P) )
        {
            break;
        }

        fprintf(File, "%s", P.First_Name);
        fprintf(File, " %s", P.Last_Name);
        fprintf(File, " %i\n", P.Age);
    }

    fclose(File);
}
