#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>

#include  <iostream>
#include  <string>
#include  <algorithm>
#include  <vector>


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

    std::vector<int>  N;

    FILE  *F;

    N.reserve(10);

    fopen_s(&F, "File1.txt", "r");

    while ( !feof(F) )
    {
        int  Q;

        fscanf_s(F, "%i", &Q);

        N.push_back(Q);
    }

    fclose(F);

    std::sort(
        N.begin(),
        N.end(),
        Comparator);

    for ( auto  Q : N )
    {
        printf("%i\n", Q);
    }
}
