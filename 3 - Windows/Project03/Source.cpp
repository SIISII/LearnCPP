#include  <cstdint>

#include  <Windows.h>
#include  <wingdi.h>
#include  <d2d1.h>
#include  <dwrite.h>

#include  <cstdio>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")



HANDLE  hApp;
HWND    hWnd;

ID2D1Factory           *pFactory        = nullptr;

ID2D1HwndRenderTarget  *pRender_Target  = nullptr;

ID2D1SolidColorBrush   *pBrush          = nullptr;


int  RW = 100; 
int  RH = 30;

int  Width;
int  Height;

int  DX = 0;
int  DY = 0;


template <class  T>
void  Safe_Release(T*  &pT)
{
    if ( pT != nullptr )
    {
        pT->Release();
        pT = nullptr;
    }
}


void  Discard_Graphics_Resources()
{
    Safe_Release(pBrush);
    Safe_Release(pRender_Target);
}


HRESULT  Create_Graphics_Resources()
{
    HRESULT  HR = S_OK;

    if ( pRender_Target == nullptr )
    {
        RECT  RC;

        // Получение размеров клиентской (т.е. основной) области окна в виде
        // координат левого верхнего и правого нижнего углов. Координаты левого
        // верхнего угла всегда равны нулю.
        GetClientRect(hWnd, &RC);

        // Получение размеров клиентской области; численно они равны координатам
        // её правого нижнего угла.
        D2D1_SIZE_U  Size = D2D1::SizeU(RC.right, RC.bottom);

        // Создание цели рендеринга (поверхности, на которой будет идти рисова-
        // ние). Используются стандартные параметры, формируемые вызовами мето-
        // дов RenderTargetProperties и HwndRenderTargetProperties.
        HR = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hWnd, Size),
            &pRender_Target);

        // Если цель рендеринга создана успешно, создаём остальные необходимые
        // для рисования вещи.
        if ( SUCCEEDED(HR) )
        {
            D2D1_COLOR_F  Color = D2D1::ColorF(0.8f, 0.8f, 0.8f);

            pRender_Target->CreateSolidColorBrush(Color, &pBrush);
        }
    }

    return HR;
}


void Paint()
{
    HRESULT  HR = Create_Graphics_Resources();

    if ( SUCCEEDED(HR) )
    {
        PAINTSTRUCT  PS;
        BeginPaint(hWnd, &PS);

        pRender_Target->BeginDraw();

        pRender_Target->Clear(D2D1::ColorF(D2D1::ColorF::White));

        pRender_Target->FillRectangle(
            D2D1::Rect(
                Width / 2 - RW / 2 + DX,
                Height / 2 - RH / 2 + DY,
                Width / 2 + RW / 2 + DX,
                Height / 2 + RH / 2 + DY),
            pBrush);

        HR = pRender_Target->EndDraw();

        if ( FAILED(HR) || HR == D2DERR_RECREATE_TARGET )
        {
            Discard_Graphics_Resources();
        }

        EndPaint(hWnd, &PS);
    }
}


LRESULT CALLBACK  Window_Proc(
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
    int  zDelta = 0;

    switch ( uMsg )
    {
        case WM_CREATE:
            if ( FAILED(D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED,
                &pFactory)) )
            {
                return -1;
            }

            return 0;


        case WM_PAINT:
            Paint();
            return 0;


        case WM_MOUSEWHEEL:
            zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            if ( zDelta < 0 )
            {
                if ( RW > 30 )
                {
                    RW = RW - 10;
                    RH = RH - 3;
                }
            }
            else
            {
                if ( RW < 300 )
                {
                    RW = RW + 10;
                    RH = RH + 3;
                }
            }
            Paint();
            return 0;


        case WM_CHAR:
            if ( wParam == 'W' || wParam == 'w' )
            {
                if ( DY > -300 )
                {
                    DY = DY - 10;
                }
            }

            if ( wParam == 'S' || wParam == 's' )
            {
                if ( DY < 300 )
                {
                    DY = DY + 10;
                }
            }

            if ( wParam == 'A' || wParam == 'a' )
            {
                if ( DX > -300 )
                {
                    DX = DX - 10;
                }
            }

            if ( wParam == 'D' || wParam == 'd' )
            {
                if ( DX < 300 )
                {
                    DX = DX + 10;
                }
            }
            Paint();
            break;


        case WM_SIZE:
            Discard_Graphics_Resources();
            Width  = static_cast<std::uint16_t>(lParam);
            Height = static_cast<std::uint16_t>(lParam >> 16);
            Paint();
            return 0;


        case  WM_DESTROY:
            Discard_Graphics_Resources();
            Safe_Release(pFactory);
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}


int WINAPI  wWinMain(
    HINSTANCE  hInstance,
    HINSTANCE  hPrevInstance,
    PWSTR      pCmdLine,
    int        nCmdShow)
{
    hApp = hInstance;

    // -----------------------
    // Регистрация класса окна
    // -----------------------

    WNDCLASSW  wc = { };

    const wchar_t  Class_Name[] = L"Circle Window Class";

    wc.lpfnWndProc = Window_Proc;
    wc.hInstance = hInstance;
    wc.lpszClassName = Class_Name;

    RegisterClassW(&wc);


    // ---------------------------
    // Создание и отображение окна
    // ---------------------------

    hWnd = CreateWindowExW(
        WS_EX_DLGMODALFRAME,            // Optional window styles
        Class_Name,                     // Window class
        L"Circle",                      // Window text
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,                  // Size and position
        CW_USEDEFAULT,
        300,
        200,
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        nullptr);                       // Additional application data

    if ( hWnd == NULL )
    {
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);


    // ------------------------
    // Цикл обработки сообщений
    // ------------------------

    // Появление сообщения WM_QUIT завершает цикл.
    MSG  Msg = { };
    while ( GetMessageW(&Msg, NULL, 0, 0) )
    {
        TranslateMessage(&Msg);
        DispatchMessageW(&Msg);
    }

    return Msg.wParam;
}
