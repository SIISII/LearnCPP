#include <cstdio>
#include <windows.h>
#include  <clocale>
#include <thread>

constexpr wchar_t Name[] = L"SingleMode";

void Second_Thread()
{
    printf_s("\nSecond thread is launched");

    Sleep(5000);

    printf_s("\n����� �����������...");
}

int main()
{
    std::setlocale(LC_ALL, "Russian");
    
    printf_s("������� �����...");

    std::thread T(Second_Thread);

    printf_s("\n������� �����.");

    T.join();
}