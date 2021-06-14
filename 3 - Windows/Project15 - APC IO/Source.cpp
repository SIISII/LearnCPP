#include  <Windows.h>
#include  <clocale>
#include  <cstdlib>
#include  <cstdio>

constexpr int  Num_Numbers = 500;

HANDLE  F;

int  Counter = 0;
volatile bool  Flag;


void  IO_Completed(
    DWORD,
    DWORD,
    LPOVERLAPPED)
{
    Flag = true;
}


int  main()
{
    std::setlocale(LC_ALL, "Russian");
    
    srand(478);

    F = CreateFileW(
        L"File.txt",
        GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_WRITE_THROUGH,
        NULL);

    OVERLAPPED  Ov {};

    int  Counter = 0;

    for ( int  I = 0; I < Num_Numbers; ++I )
    {
        char  S[16];
        sprintf_s(S, sizeof(S), "%i\n", rand());
        int  Len = strlen(S);
        
        Flag = false;

        WriteFileEx(
            F,
            S,
            Len,
            &Ov,
            IO_Completed);

        while ( !Flag )
        {
            ++Counter;
            SleepEx(0, true);
        }

        Ov.Offset = Ov.Offset + Len;
    }

    printf("%i", Counter);

    CloseHandle(F);
}
