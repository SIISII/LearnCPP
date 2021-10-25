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

    printf_s("������� ������: ");
    scanf_s("%s", S, _countof(S));

    printf_s("������: %s\n", S);

    int32  Len = strlen(S);

    printf_s("����� ������: %i\n", Len);

    int32  Count = 0;

    // ������������ ������ "ab" "AB" "aB" 'Ab" ������ S
    for ( int32  I = 1; I < Len; ++I )
    {
        if ( (S[I] == 'b' || S[I] == 'B')
             &&
             (S[I-1] == 'a' || S[I-1] == 'A') )
        {
            ++Count;
        }
    }

    printf_s("����� ����� ���� 'ab' = %i", Count);
}
