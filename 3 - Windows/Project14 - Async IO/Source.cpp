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
        L"File.txt",
        GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        NULL);

    for ( int  I = 0; I < Num_Numbers; ++I )
    {
        OVERLAPPED  Ov = {};
        Ov.hEvent = E;

        char  S[16];
        sprintf_s(S, sizeof(S), "%i\n", rand());
        int  Len = strlen(S);
        WriteFile(
            F,
            S,
            Len,
            nullptr,
            &Ov);
        WaitForSingleObject(E, INFINITE);
        ResetEvent(E);
    }


    CloseHandle(F);
}