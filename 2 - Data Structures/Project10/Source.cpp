#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>

#include  <iostream>
#include  <string>
#include  <algorithm>


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    std::string  Str;

    std::cout << "������� ������: ";
    std::cin >> Str;

    std::sort(Str.begin(), Str.end());

    std::cout << "��������������� ������: " << Str;

    int  L = Str.size();

    std::cout << "\n����� ������ = " << L;

    std::cout << '\n' << Str[2];

    Str.replace(1, 3, "xyz");

    std::cout << "������ � �������: " << Str;
}
