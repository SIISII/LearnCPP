#include  <cstdio>
#include  <clocale>
#include  <Windows.h>


constexpr int  Num_Threads = 10;
constexpr int  Num_Numbers = 500;


void CALLBACK  Work_Callback_Proc(
    PTP_CALLBACK_INSTANCE  Instance,
    PVOID                  Context,
    PTP_WORK               Work);


TP_CALLBACK_ENVIRON  CB_Env {};

PTP_POOL             Pool          = nullptr;
PTP_CLEANUP_GROUP    Cleanup_Group = nullptr;


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    InitializeThreadpoolEnvironment(&CB_Env);

    Pool = CreateThreadpool(nullptr);

    SetThreadpoolThreadMaximum(Pool, 20);
    SetThreadpoolThreadMinimum(Pool, 5);

    SetThreadpoolCallbackPool(&CB_Env, Pool);

    Cleanup_Group = CreateThreadpoolCleanupGroup();

    SetThreadpoolCallbackCleanupGroup(&CB_Env, Cleanup_Group, nullptr);

    for ( int  I = 0; I < Num_Numbers; ++I )
    {
        PTP_WORK  Work = CreateThreadpoolWork(
            Work_Callback_Proc,
            reinterpret_cast<void*>(I),
            &CB_Env);
        SubmitThreadpoolWork(Work);
    }

    CloseThreadpoolCleanupGroupMembers(Cleanup_Group, false, nullptr);
}


void CALLBACK  Work_Callback_Proc(
    PTP_CALLBACK_INSTANCE  Instance,
    PVOID                  Context,
    PTP_WORK               Work)
{
    int  I = reinterpret_cast<int>(Context);

    DWORD  Id = GetCurrentThreadId();

    printf("Thread = %08X; Number = %i\n", Id, I);

    Sleep(1000);
}
