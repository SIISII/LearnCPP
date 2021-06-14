#include  <Windows.h>
#include  <clocale>
#include  <cstdlib>
#include  <cstdio>

constexpr int  Num_Numbers = 500;

HANDLE  F;
HANDLE  E;

int  Counter = 0;


int  main()
{
    std::setlocale(LC_ALL, "Russian");
    
    srand(478);

    E = CreateEventW(nullptr, true, false, nullptr);

    F = CreateFileW(
        L"File.txt",
        GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_WRITE_THROUGH,
        NULL);

    OVERLAPPED  Ov {};

    Ov.hEvent = E;

    int  Counter = 0;

    for ( int  I = 0; I < Num_Numbers; ++I )
    {
        char  S[16];
        sprintf_s(S, sizeof(S), "%i\n", rand());
        int  Len = strlen(S);

        WriteFile(
            F,
            S,
            Len,
            nullptr,
            &Ov);

        while ( WaitForSingleObject(E, 0) != WAIT_OBJECT_0 )
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
