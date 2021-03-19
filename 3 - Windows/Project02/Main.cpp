#include  <Windows.h>
#include  <wingdi.h>
#include  <d2d1.h>


#pragma comment(lib, "d2d1")


// Хэндл ("номер") приложения (программы).
HINSTANCE               hApp;

// Хэндл окна.
HWND                    hWnd;

// Адрес фабрики для создания ресурсов Direct2D.
ID2D1Factory           *pFactory       = nullptr;

// Адрес цели рендеринга (т.е. где будет формироваться изображение).
ID2D1HwndRenderTarget  *pRender_Target = nullptr;

// Кисточка для рисования эллипсов и окружностей.
ID2D1SolidColorBrush   *pEllipse_Brush = nullptr;

// Кисточка для рисования прямоугольников.
ID2D1SolidColorBrush   *pRect_Brush    = nullptr;

// Параметры эллиспса.
D2D1_ELLIPSE            Ellipse_Parms;


// =============================================================================
//
// Safe_Release
//
// =============================================================================

template <class  T>
void  Safe_Release(T*  &pT)
{
    if ( pT != nullptr )
    {
        pT->Release();
        pT = nullptr;
    }
}


// =============================================================================
//
// Calculate_Layout
//
// =============================================================================

void  Calculate_Layout()
{
    if ( pRender_Target != nullptr )
    {
        D2D1_SIZE_F  Size = pRender_Target->GetSize();

        const float  X = Size.width / 2;
        const float  Y = Size.height / 2;

        const float  Radius = min(X, Y) / 2;

        Ellipse_Parms = D2D1::Ellipse(D2D1::Point2F(X, Y), X, Y);
    }
}


// =============================================================================
//
// Create_Graphics_Resources
//
// =============================================================================

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

        // Если цель рендеринга создана успешно, создаём кисть для рисования
        // окружности заданным цветом, а затем определяем размеры и положение
        // окружности, исходя из размеров окна.
        if ( SUCCEEDED(HR) )
        {
            D2D1_COLOR_F  Color = D2D1::ColorF(0.0f, 0.0f, 1.0f);   // R, G, B

            HR = pRender_Target->CreateSolidColorBrush(Color, &pEllipse_Brush);

            if ( SUCCEEDED(HR) )
            {
                Calculate_Layout();
            }

            Color = D2D1::ColorF(0.5f, 0.6f, 0.3f);   // R, G, B

            HR = pRender_Target->CreateSolidColorBrush(Color, &pRect_Brush);
        }
    }

    return HR;
}


// =============================================================================
//
// Discard_Graphics_Resources
//
// =============================================================================

void  Discard_Graphics_Resources()
{
    Safe_Release(pRect_Brush);
    Safe_Release(pEllipse_Brush);
    Safe_Release(pRender_Target);
}


// =============================================================================
//
// Paint
//
// =============================================================================

void  Paint()
{
    HRESULT  HR = Create_Graphics_Resources();

    if ( SUCCEEDED(HR) )
    {
        PAINTSTRUCT  PS;
        BeginPaint(hWnd, &PS);

        pRender_Target->BeginDraw();

        pRender_Target->Clear(D2D1::ColorF(D2D1::ColorF::Moccasin));

        pRender_Target->DrawEllipse(Ellipse_Parms, pEllipse_Brush);

        pRender_Target->DrawRectangle(
            D2D1::Rect(10, 10, 300, 150),
            pRect_Brush);

        D2D1_ROUNDED_RECT  RR;

        RR.rect.top    = 100;
        RR.rect.left   = 100;
        RR.rect.bottom = 250;
        RR.rect.right  = 1000;
        RR.radiusX = 50;
        RR.radiusY = 25;

        pRender_Target->FillRoundedRectangle(
            RR,
            pRect_Brush);

        HR = pRender_Target->EndDraw();

        if ( FAILED(HR) || HR == D2DERR_RECREATE_TARGET )
        {
            Discard_Graphics_Resources();
        }

        EndPaint(hWnd, &PS);
    }
}


// =============================================================================
//
// Resize
//
// =============================================================================

void  Resize()
{
    if ( pRender_Target != nullptr )
    {
        RECT  RC;

        GetClientRect(hWnd, &RC);

        D2D1_SIZE_U  Size = D2D1::SizeU(RC.right, RC.bottom);

        pRender_Target->Resize(Size);

        Calculate_Layout();

        InvalidateRect(hWnd, nullptr, false);
    }
}


// =============================================================================
//
// Window_Proc
//
// =============================================================================

LRESULT CALLBACK  Window_Proc(
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
    switch ( uMsg )
    {
        // Собственно создание окна. Это сообщение выдаётся и обрабатывается в
        // процессе выполнения CreateWindowExW (до начала обычного цикла сооб-
        // щений). Если возвращается значение -1, создание окна прекращается, и
        // CreateWindowExW вернёл нулевой хэндл.
        //
        // Здесь мы создаём фабрику типа ID2D1Factory, которая впоследствии
        // используется для создания ресурсов Direct2D. Тип фабрики определяется
        // компилятором на основе типа переменной pFactory, в которой будет
        // сохранён указатель на созданную фабрику.
        case WM_CREATE:
            if ( FAILED( D2D1CreateFactory(
                    D2D1_FACTORY_TYPE_SINGLE_THREADED,
                    &pFactory) ) )
            {
                return -1;
            }

            return 0;

        // Отрисовка окна. Будут созданы необходимые ресурсы и осуществлено
        // собственно отображение содержимого окна.
        case WM_PAINT:
            Paint();
            return 0;

        // Изменение размеров окна. Кроме собственно изменения размеров, будет
        // выполнена "подгонка" ресурсов и перерисовка окна.
        case WM_SIZE:
            Resize();
            return 0;

        // Уничтожение окна вызывает завершение программы, для чего в очередь
        // сообщений добавляется сообщение WM_QUIT. Перед этим, однако, уничто-
        // жаются все созданные ресурсы, а также фабрика.
        case  WM_DESTROY:
            Discard_Graphics_Resources();
            Safe_Release(pFactory);
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}


// =============================================================================
//
// wWinMain
//
// =============================================================================

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

    WNDCLASSW  WC = { };

    const wchar_t  Class_Name[]  = L"Circle Window Class";

    WC.lpfnWndProc   = Window_Proc;
    WC.hInstance     = hInstance;
    WC.lpszClassName = Class_Name;

    RegisterClassW(&WC);


    // ---------------------------
    // Создание и отображение окна
    // ---------------------------

    hWnd = CreateWindowExW(
        0,                              // Optional window styles
        Class_Name,                     // Window class
        L"Графика в окне",              // Window text
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

    return 0;
}
