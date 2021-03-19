#include  <Windows.h>
#include  <wingdi.h>
#include  <d2d1.h>


#pragma comment(lib, "d2d1")


// ����� ("�����") ���������� (���������).
HINSTANCE               hApp;

// ����� ����.
HWND                    hWnd;

// ����� ������� ��� �������� �������� Direct2D.
ID2D1Factory           *pFactory       = nullptr;

// ����� ���� ���������� (�.�. ��� ����� ������������� �����������).
ID2D1HwndRenderTarget  *pRender_Target = nullptr;

// �������� ��� ��������� �������� � �����������.
ID2D1SolidColorBrush   *pEllipse_Brush = nullptr;

// �������� ��� ��������� ���������������.
ID2D1SolidColorBrush   *pRect_Brush    = nullptr;

// ��������� ��������.
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

        // ��������� �������� ���������� (�.�. ��������) ������� ���� � ����
        // ��������� ������ �������� � ������� ������� �����. ���������� ������
        // �������� ���� ������ ����� ����.
        GetClientRect(hWnd, &RC);

        // ��������� �������� ���������� �������; �������� ��� ����� �����������
        // � ������� ������� ����.
        D2D1_SIZE_U  Size = D2D1::SizeU(RC.right, RC.bottom);

        // �������� ���� ���������� (�����������, �� ������� ����� ���� ������-
        // ���). ������������ ����������� ���������, ����������� �������� ����-
        // ��� RenderTargetProperties � HwndRenderTargetProperties.
        HR = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hWnd, Size),
            &pRender_Target);

        // ���� ���� ���������� ������� �������, ������ ����� ��� ���������
        // ���������� �������� ������, � ����� ���������� ������� � ���������
        // ����������, ������ �� �������� ����.
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
        // ���������� �������� ����. ��� ��������� ������� � �������������� �
        // �������� ���������� CreateWindowExW (�� ������ �������� ����� ����-
        // �����). ���� ������������ �������� -1, �������� ���� ������������, �
        // CreateWindowExW ����� ������� �����.
        //
        // ����� �� ������ ������� ���� ID2D1Factory, ������� ������������
        // ������������ ��� �������� �������� Direct2D. ��� ������� ������������
        // ������������ �� ������ ���� ���������� pFactory, � ������� �����
        // ������� ��������� �� ��������� �������.
        case WM_CREATE:
            if ( FAILED( D2D1CreateFactory(
                    D2D1_FACTORY_TYPE_SINGLE_THREADED,
                    &pFactory) ) )
            {
                return -1;
            }

            return 0;

        // ��������� ����. ����� ������� ����������� ������� � ������������
        // ���������� ����������� ����������� ����.
        case WM_PAINT:
            Paint();
            return 0;

        // ��������� �������� ����. ����� ���������� ��������� ��������, �����
        // ��������� "��������" �������� � ����������� ����.
        case WM_SIZE:
            Resize();
            return 0;

        // ����������� ���� �������� ���������� ���������, ��� ���� � �������
        // ��������� ����������� ��������� WM_QUIT. ����� ����, ������, ������-
        // ������ ��� ��������� �������, � ����� �������.
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
    // ����������� ������ ����
    // -----------------------

    WNDCLASSW  WC = { };

    const wchar_t  Class_Name[]  = L"Circle Window Class";

    WC.lpfnWndProc   = Window_Proc;
    WC.hInstance     = hInstance;
    WC.lpszClassName = Class_Name;

    RegisterClassW(&WC);


    // ---------------------------
    // �������� � ����������� ����
    // ---------------------------

    hWnd = CreateWindowExW(
        0,                              // Optional window styles
        Class_Name,                     // Window class
        L"������� � ����",              // Window text
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
    // ���� ��������� ���������
    // ------------------------

    // ��������� ��������� WM_QUIT ��������� ����.
    MSG  Msg = { };
    while ( GetMessageW(&Msg, NULL, 0, 0) )
    {
        TranslateMessage(&Msg);
        DispatchMessageW(&Msg);
    }

    return 0;
}
