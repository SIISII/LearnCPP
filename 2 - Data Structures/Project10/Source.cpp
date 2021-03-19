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

    std::cout << "Введите строку: ";
    std::cin >> Str;

    std::sort(Str.begin(), Str.end());

    std::cout << "Отсортированная строка: " << Str;

    int  L = Str.size();

    std::cout << "\nДлина строки = " << L;

    std::cout << '\n' << Str[2];

    Str.replace(1, 3, "xyz");

    std::cout << "Строка с заменой: " << Str;
}
