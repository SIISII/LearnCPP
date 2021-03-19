#include  <cstdio>
#include  <clocale>
#include  <cstring>
#include  <cstdlib>


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    while (true)
    {
        int    A;
        int    B;
        char   S[50];
        char   S2[50];
        char   Op;
        char  *P;
        char  *T;

        scanf_s("%s", S, sizeof(S));

        if ( S[0] == 'Q' || S[0] == 'q' )
        {
            return 0;
        }

        P = &S[0];
        T = P;

        while ( *T != '\0' && *T >= '0' && *T <= '9' )
        {
            ++T;
        }

        if ( T == P )
        {
            printf_s("Ошибка: нет первого операнда\n");
            continue;
        }

        if ( *T == '\0' )
        {
            printf_s("Ошибка: нет операции\n");
            continue;
        }

        strncpy_s(S2, sizeof(S2), P, T - P);

        A = atoi(S2);

        if ( *T != '+' && *T != '-' && *T != '*' && *T != '/' && *T != '%' )
        {
            printf_s("Ошибка: неверная операция\n");
            continue;
        }

        Op = *T;

        P = T + 1;
        T = P;

        while ( *T != '\0' && *T >= '0' && *T <= '9' )
        {
            ++T;
        }

        if ( T == P )
        {
            printf_s("Ошибка: нет второго операнда\n");
            continue;
        }

        if ( *T != '\0' )
        {
            printf_s("Ошибка: лишние символы после второго операнда\n");
            continue;
        }

        strncpy_s(S2, sizeof(S2), P, T - P);

        B = atoi(S2);

        switch ( Op )
        {
            case '+':
                printf_s("%i\n", A + B);
                break;

            case '-':
                printf_s("%i\n", A - B);
                break;

            case '*':
                printf_s("%i\n", A * B);
                break;

            case '/':
                printf_s("%i\n", A / B);
                break;

            case '%':
                printf_s("%i\n", A % B);
                break;
        }
    }
}
