#include <cstdio>
#include  <clocale>
#include <thread>
#include <Windows.h>

void Second_Thread();

volatile char C;

HANDLE Buffer_Empty;
HANDLE Buffer_Full;

std::thread Thread(Second_Thread);

int main()
{
    std::setlocale(LC_ALL, "Russian");
    Buffer_Empty = CreateEventW(nullptr, true, true, nullptr);
    Buffer_Full = CreateEventW(nullptr, true, false, nullptr);
    while (true)
    {
        while ( B );
        C = getc(stdin);
        B = true;
    }
}

void Second_Thread()
{
    while (true)
    {
        while ( !B );
        putc(C, stdout);
        B = false;
    }
}

