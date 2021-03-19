#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <forward_list>
#include  <iostream>
#include  <string>
#include  <algorithm>


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    std::forward_list<int>  List, List2, List3;

    FILE  *F;

    fopen_s(&F, "Text.txt", "r");

    while ( !feof(F) )
    {
        int  Q;

        fscanf_s(F, "%i", &Q);

        List.push_front(Q);
    }

    fclose(F);


    fopen_s(&F, "Text1.txt", "r");

    while ( !feof(F) )
    {
        int  Q;

        fscanf_s(F, "%i", &Q);

        List2.push_front(Q);
    }

    fclose(F);


    List.sort();

    List2.sort();

    List3 = List;

    List3.merge(List2);

    List3.unique();

    List3.pop_front();

    List3.reverse();

    List3.remove(2);

    for ( auto P : List3 )
    {
        std::cout << P << '\n';
    }
}
