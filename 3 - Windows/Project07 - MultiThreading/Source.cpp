#include <cstdio>
#include <windows.h>
#include  <clocale>

constexpr wchar_t Name[] = L"SingleMode";

DWORD WINAPI Second_Thread(LPVOID)
{
    printf_s("\nSecond thread is launched");

    Sleep(5000);

    printf_s("\nПоток завершается...");

    return 69;
}

int main()
{
    std::setlocale(LC_ALL, "Russian");
    
    printf_s("Создаем поток...");

    HANDLE UH = CreateThread(nullptr, 8192, Second_Thread, nullptr, 0, nullptr);

    printf_s("\nСоздали поток.");

    DWORD ExitCode;

    do
    {
        GetExitCodeThread(UH, &ExitCode); 
    } while (ExitCode == STILL_ACTIVE);

    printf_s("\nПоток был завершен.");

    CloseHandle(UH);
}