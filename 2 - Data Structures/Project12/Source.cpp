#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <list>
#include  <iostream>
#include  <string>
#include  <algorithm>
#include  <utility>



int  main()
{
    std::setlocale(LC_ALL, "Russian");

    std::list<int>  List, List2, List3;

    FILE  *F;

    fopen_s(&F, "Text.txt", "r");

    while ( !feof(F) )
    {
        int  Q;

        fscanf_s(F, "%i", &Q);

        List.push_back(Q);
    }

    fclose(F);


    fopen_s(&F, "Text1.txt", "r");

    while ( !feof(F) )
    {
        int  Q;

        fscanf_s(F, "%i", &Q);

        List2.push_back(Q);
    }

    fclose(F);



    List.sort();

    List2.sort();

    List3 = std::move(List);

    List3.merge(List2);

    List3.unique();

    List3.pop_front();

    List3.reverse();

    List3.remove(2);

    std::cout << "длина 1 = " << List.size() << '\n';

    std::cout << "длина 2 = " << List2.size() << '\n';

    std::cout << "длина 3 = " << List3.size() << '\n';

    for ( auto P : List3 )
    {
        std::cout << P << '\n';
    }
    
    int  Sum = 0;

    for ( std::list<int>::iterator  P = List3.begin(); P != List3.end(); ++P )
    {
        Sum = Sum + *P;
    }

    std::cout << "Чумма равна " << Sum << '\n';


    for ( std::list<int>::iterator P = List3.begin(); P != List3.end(); ++P )
    {
        if ( *P % 2 != 0 )
        {
            List3.insert(P, *P + 1);  
        }
    }


    for ( auto P : List3 )
    {
        std::cout << P << '\n';
    }
}