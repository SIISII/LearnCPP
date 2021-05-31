#include <cstdio>
#include  <clocale>
#include <thread>
#include <Windows.h>

void Second_Thread();

volatile char C;

HANDLE Buffer_Empty;
HANDLE Buffer_Full;

std::thread *Thread;

int main()
{
    std::setlocale(LC_ALL, "Russian");
    Buffer_Empty = CreateEventW(nullptr, true, true, nullptr);
    Buffer_Full = CreateEventW(nullptr, true, false, nullptr);
    Thread = new std::thread(Second_Thread);
    while (true)
    {
        WaitForSingleObject(Buffer_Empty, INFINITE);
        ResetEvent(Buffer_Empty);
        C = getc(stdin);
        SetEvent(Buffer_Full);
    }
}

void Second_Thread()
{
    while (true)
    {
        WaitForSingleObject(Buffer_Full, INFINITE);
        ResetEvent(Buffer_Full);
        putc(C, stdout);
        SetEvent(Buffer_Empty);
    }
}

