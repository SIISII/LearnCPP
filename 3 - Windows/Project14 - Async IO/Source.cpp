#include  <Windows.h>
#include  <clocale>
#include  <cstdlib>
#include  <cstdio>

HANDLE  F;
HANDLE  E;
constexpr int  Num_Numbers = 500;
int  Counter = 0;

int  main()
{
    std::setlocale(LC_ALL, "Russian");
    
    srand(478);

    E = CreateEvent(nullptr, true, false, nullptr);

    F = CreateFileW(
        L"D:\\File.txt",
        GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        NULL);

    OVERLAPPED  Ov = {};
    Ov.hEvent = E;

    int Counter = 0;

    for ( int  I = 0; I < Num_Numbers; ++I )
    {
        char  S[1000000];
        sprintf_s(S, sizeof(S), "%i\n", rand());
        //int  Len = strlen(S);
        int  Len = 1000000;
        WriteFile(
            F,
            S,
            Len,
            nullptr,
            &Ov);
        while ( WaitForSingleObject(E, INFINITE) != WAIT_OBJECT_0 )
        {
            ++Counter;
        }
        Ov.Offset = Ov.Offset + Len;
        ResetEvent(E);
    }
    printf("%i", Counter);

    CloseHandle(F);
    CloseHandle(E);
}