#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <cstring>
#include  <cctype>

using  int32  = std::int32_t;


constexpr int32  Qty = 50;


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    char  S[Qty];

    printf_s("Введите строку: ");
    scanf_s("%s", S, _countof(S));

    printf_s("Строка: %s\n", S);

    int32  Len = strlen(S);

    printf_s("Длина строки: %i\n", Len);

    int32  Count = 0;

    // Подсчитываем строки "ab" "AB" "aB" 'Ab" внутри S
    for ( int32  I = 1; I < Len; ++I )
    {
        if ( (S[I] == 'b' || S[I] == 'B')
             &&
             (S[I-1] == 'a' || S[I-1] == 'A') )
        {
            ++Count;
        }
    }

    printf_s("Число строк вида 'ab' = %i", Count);
}
