#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>

#include  <iostream>
#include  <string>
#include  <algorithm>
#include  <array>


using  uint32  = std::uint32_t;


bool  Comparator(
    int  A,
    int  B)
{
    return (A & 0b111u) > (B & 0b111u);
}


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    std::array<int, 10>  N;

    for ( int  I = 0; I < N.size(); ++I )
    {
        scanf_s("%i", &N[I]);
    }

    std::sort(
        N.begin(),
        N.end(),
        [] (int a, int b) { return (a & 0b111u) > (b & 0b111u); }
    );

    for ( auto  Q : N )
    {
        printf("%i\n", Q);
    }
}
