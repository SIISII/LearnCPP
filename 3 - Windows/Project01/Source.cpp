#include  <Windows.h>


LRESULT CALLBACK  Window_Proc(
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam);


int WINAPI  wWinMain(
    HINSTANCE  hInstance,
    HINSTANCE  hPrevInstance,
    PWSTR      pCmdLine,
    int        nCmdShow)
{
    // Регистрация класса окна.
    const wchar_t  Class_Name[]  = L"Sample Window Class";

    WNDCLASSW  WC = { };

    WC.lpfnWndProc   = Window_Proc;
    WC.hInstance     = hInstance;
    WC.lpszClassName = Class_Name;

    RegisterClassW(&WC);


    // Создание и отображение окна.
    HWND  hWnd = CreateWindowExW(
        0,                              // Optional window styles
        Class_Name,                     // Window class
        L"Учимся создавать окна",       // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT,                  // Size and position
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        nullptr);                       // Additional application data

    if ( hWnd == NULL )
    {
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);

    // Цикл прокачки сообщений.
    MSG  Msg = { };

    while ( GetMessageW(&Msg, NULL, 0, 0) )
    {
        TranslateMessage(&Msg);
        DispatchMessageW(&Msg);
    }

    return 0;
}


LRESULT CALLBACK  Window_Proc(
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
    switch ( uMsg )
    {
        case  WM_CLOSE:
            if ( MessageBoxW(
                    hWnd,
                    L"Действительно закрыть?",
                    L"Наша программа",
                    MB_OKCANCEL)
                 == IDOK )
            {
                DestroyWindow(hWnd);
            }
            return 0;

        case  WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case  WM_PAINT:
        {
            PAINTSTRUCT  PS;
            HDC          HDC = BeginPaint(hWnd, &PS);

            FillRect(HDC, &PS.rcPaint, HBRUSH(COLOR_WINDOW+1) );

            EndPaint(hWnd, &PS);
            return 0;
        }
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
