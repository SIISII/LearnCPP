#include  <cstdio>
#include  <clocale>
#include  <thread>
#include  <Windows.h>
#include  <vector>
#include  <queue>
#include  <mutex>
#include  <cstdlib>
#include  <atomic>

constexpr int  Num_Threads = 10;
constexpr int  Num_Numbers = 500;


void  Second_Thread();


PTP_POOL  Pool;
TP_CALLBACK_ENVIRON  CB_Env {};

std::queue<int>             Queue;
std::mutex                  Mutex;
std::atomic_int             Counter = 0;
std::atomic_int             Print_Counter = 0;


int  main()
{
    std::setlocale(LC_ALL, "Russian");
    
    srand(478);

    Pool = CreateThreadpool(nullptr);
    SetThreadpoolThreadMaximum(Pool, 20);
    SetThreadpoolThreadMinimum(Pool, 5);

    InitializeThreadpoolEnvironment(&CB_Env);
    SetThreadpoolCallbackPool(&CB_Env, Pool);

    for ( int  I = 0; I < Num_Numbers; ++I )
    {
        Mutex.lock();
        Queue.push(rand());
        Mutex.unlock();
    }

    while ( Print_Counter == Num_Numbers );
}


void  Second_Thread()
{
    int  Thread_Num = Counter++;
    
    while ( true )
    {
        int   Cur_Num;
        bool  Present = false;

        Mutex.lock();

        if ( !Queue.empty() )
        {
            Present = true;
            Cur_Num = Queue.front();
            Queue.pop();
        }

        Mutex.unlock();

        if ( Present )
        {
            printf_s("%i %i \n", Cur_Num, Thread_Num);
            ++Print_Counter;
        }
    }
}
