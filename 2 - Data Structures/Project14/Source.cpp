#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <map>
#include  <iostream>
#include  <string>
#include  <algorithm>
#include  <utility>


int  Hash(const std::string  &S)
{
    int  Result = 0;

    for ( int  I = 0; I < S.size(); I++ )
    {
        Result = Result + S[I];
    }
    return Result;
}


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    std::multimap<int, std::string>  Set;

    FILE  *F;

    fopen_s(&F, "Text.txt", "r");

    while ( !feof(F) )
    {
        char  Str[50];

        fscanf_s(F, "%s", Str, 50);

        std::string S = Str;

        int H = Hash(S);

        Set.insert({H,S});
    }

    fclose(F);

    for ( auto  P : Set )
    {
        std::cout << P.first << ' ' << P.second << '\n';
    }

    std::string S;
    std::cin >> S;
    int  H = Hash(S);
    auto  R = Set.equal_range(H);

    int  Count = 0;

    for ( auto  P = R.first; P != R.second; P++ )
    {
        auto  V = *P;

        if ( S == V.second )
        {
            Count++;
        }
    }

    std::cout << "Найдено " << Count << " строк\n";
}