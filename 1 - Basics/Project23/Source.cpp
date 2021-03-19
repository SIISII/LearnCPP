#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <cstring>

using  int32  = std::int32_t;


constexpr int32  Qty = 50;


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    char  S1[Qty];
    char  S2[Qty];

    printf_s("Введите строку 1: ");
    scanf_s("%s", S1, _countof(S1));

    printf_s("Введите строку 2: ");
    scanf_s("%s", S2, _countof(S2));

    int32  Len1 = strlen(S1);
    int32  Len2 = strlen(S2);

    int32  Count = 0;

    for ( int32  I = Len1 - Len2; I >= 0; --I )
    {
        bool  EQ = true;

        for ( int32  J = 0; J < Len2; ++J )
        {
            if ( S1[I + J] != S2[J] )
            {
                EQ = false;
                break;
            }
        }

        if ( EQ )
        {
            ++Count;
            printf_s("%i\n", I);

            for ( int32  J = 0; J < Len2; ++J )
            {
                S1[I + J] = S2[Len2 - 1 - J];
            }
        }
    }

    printf_s("Число подстрок внутри строки = %i\n", Count);

    printf_s("Результирующая строка = %s", S1);
}
