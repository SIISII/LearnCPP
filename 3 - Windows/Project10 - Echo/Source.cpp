#include <cstdio>
#include  <clocale>
#include <thread>
#include <atomic>

void Second_Thread();

volatile char C;
volatile bool B = false;

std::thread Thread(Second_Thread);

int main()
{
    std::setlocale(LC_ALL, "Russian");
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

