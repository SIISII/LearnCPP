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
    
    Buffer_Empty = CreateSemaphoreW(nullptr, 1, 1, nullptr);
    Buffer_Full = CreateSemaphoreW(nullptr, 0, 1, nullptr);
    Thread = new std::thread(Second_Thread);
    while (true)
    {
        WaitForSingleObject(Buffer_Empty, INFINITE);
        C = getc(stdin);
        ReleaseSemaphore(Buffer_Full, 1, nullptr);
    }
}

void Second_Thread()
{
    while (true)
    {
        WaitForSingleObject(Buffer_Full, INFINITE);
        putc(C, stdout);
        ReleaseSemaphore(Buffer_Empty, 1, nullptr);
    }
}

