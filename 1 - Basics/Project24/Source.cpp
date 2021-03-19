#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <cstring>


using   int32  = std::int32_t;
using  uint32  = std::uint32_t;

constexpr int32  Qty = 50;

int  main()
{
    std::setlocale(LC_ALL, "Russian");

    char  *S;

//    S = reinterpret_cast<char *>( malloc(Qty) );

    S = new char [Qty];

    printf_s("¬ведите строку: ");
    scanf_s("%s", S, Qty);

    int32  Len = strlen(S);

//    free(S);

    delete [] S;

    printf_s("ƒлина строки = %i\n", Len);
}
