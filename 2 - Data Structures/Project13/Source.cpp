#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <set>
#include  <iostream>
#include  <string>
#include  <algorithm>
#include  <utility>


constexpr  int  Lower = 10;
constexpr  int  Upper = 10000;


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    std::multiset<int>  Set;

    FILE  *F;

    fopen_s(&F, "Text.txt", "r");

    while ( !feof(F) )
    {
        int  Q;

        fscanf_s(F, "%i", &Q);

        Set.insert(Q);
    }

    fclose(F);

    for ( const auto  P : Set )
    {
        std::cout << P << '\n';
    }

    int  Count = 0;

    for ( auto  P = Set.lower_bound(Lower); P != Set.upper_bound(Upper); ++P )
    {
        ++Count;
    }

    // Можно записать так: auto  Iterators = Set.equal_range(45);
    // А можно - полностью описав тип вручную:
    std::pair< std::multiset<int>::const_iterator, std::multiset<int>::const_iterator >  Iterators = Set.equal_range(45);

    for ( auto  P = Iterators.first; P != Iterators.second; ++P )
    {
        ++Count;
    }
 
    std::cout << "Найдено " << Count << " элементов\n";
}
