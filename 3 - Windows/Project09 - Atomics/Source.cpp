#include <cstdio>
#include <windows.h>
#include  <clocale>
#include <thread>

constexpr wchar_t Name[] = L"SingleMode";

std::atomic_int V = 0;

void Second_Thread()
{
    for (int i = 0; i <= 5000000; ++i)
    {
        ++V;
    }
}

int main()
{
    std::setlocale(LC_ALL, "Russian");
    
    printf_s("Создаем поток...");

    std::thread T(Second_Thread);

    printf_s("\nСоздали поток.");

    while (V < 5000000)
    {
        printf_s("%i\n", V.load());
        Sleep(1);
    }

    T.join();
}