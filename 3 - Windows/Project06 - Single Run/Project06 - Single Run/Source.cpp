#include <cstdio>
#include <windows.h>
#include  <clocale>

constexpr wchar_t Name[] = L"SingleMode";

int main()
{
    std::setlocale(LC_ALL, "Russian");
    
    HANDLE UH = OpenMutexW(MUTEX_ALL_ACCESS, false, Name);

    if (UH == NULL)
    {
        UH = CreateMutexW(nullptr, true, Name);

        if (UH == NULL)
        {
            printf_s("������� Mutex �� �������");
        }
        else
        {
            printf_s("��������� ������� ��������, ������� ���-������");

            char c;
            scanf_s("%c", &c);

            CloseHandle(UH);
        }
    }
    else
    {
        printf_s("��������� ��� ��������.");

        CloseHandle(UH);
    }
}