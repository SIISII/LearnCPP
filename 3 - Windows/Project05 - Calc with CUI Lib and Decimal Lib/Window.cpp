#include  <cstring>
#include  <exception>
#include  <map>
#include  <utility>

#include  <Windows.h>

#include  "Application.h"
#include  "Control.h"
#include  "Text.h"
#include  "Window.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")


ATOM              Window::Wnd_Class     = NULL;
ID2D1Factory     *Window::pD2D_Factory  = nullptr;
IDWriteFactory   *Window::pDW_Factory   = nullptr;


LRESULT CALLBACK  Window::Window_Proc(
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
    if ( uMsg == WM_CREATE )
    {
        CREATESTRUCTW  *S = reinterpret_cast<CREATESTRUCTW*>(lParam);

        Window  *Wnd = reinterpret_cast<Window*>(S->lpCreateParams);

        SetWindowLongPtrW(
            hWnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(Wnd));

        if ( !Wnd->On_Create() )
        {
            return 0;
        }
    }
    else
    {
        Window  *Wnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));

        switch ( uMsg )
        {
            case  WM_PAINT:
            {
                Wnd->Repaint();

                break;
            }

            case  WM_SIZE:
            {
                Wnd->Free_Resources();
                Wnd->On_Resize();

                break;
            }

            // ќбработка нажатий мышки.
            case WM_LBUTTONDOWN:
                POINT  Point;

                Point.x = static_cast<std::uint16_t>(lParam);
                Point.y = static_cast<std::uint16_t>(lParam >> 16);
                for ( auto  Obj : Wnd->Controls )
                {
                    if ( Obj->In_Object(Point.x, Point.y) )
                    {
                        if ( Obj->On_Click != nullptr )
                        {
                            Obj->On_Click(Obj);
                        }
                    }
                }

                return 0;

            // ќбработка ввода символа с клавиатуры.
            case WM_CHAR:
                for ( auto  Obj : Wnd->Controls )
                {
                    if ( Obj->On_Key_Pressed != nullptr )
                    {
                        Obj->On_Key_Pressed(Obj, wParam);
                    }
                }
                return 0;

            case  WM_DESTROY:
                Wnd->On_Destroy();
                return 0;
        }
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}


void  Window::Create(
    const wchar_t  *Caption,
    int             X,
    int             Y,
    int             Width,
    int             Height)
{
    // -----------------------
    // –егистраци€ класса окна
    // -----------------------

    if ( Wnd_Class == NULL )
    {
        WNDCLASSW  wc = { };

        const wchar_t  Class_Name[] = L"Mega Window Class";

        wc.lpfnWndProc = Window_Proc;
        wc.hInstance = Application::Get_Instance();
        wc.lpszClassName = Class_Name;
        wc.cbWndExtra = sizeof(Window*);

        Wnd_Class = RegisterClassW(&wc);

        if ( Wnd_Class == NULL )
        {
            throw std::exception("Ќе удалось зарегистрировать класс окна");
        }
    }

    if ( pD2D_Factory == nullptr )
    {
        if ( FAILED( D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED,
                &pD2D_Factory) ) )
        {
            throw std::exception("Ќе удалось создать фабрику Direct2D");
        }
    }

    if ( pDW_Factory == nullptr )
    {
        if ( FAILED( DWriteCreateFactory(
                DWRITE_FACTORY_TYPE_SHARED,
                __uuidof(IDWriteFactory),
                reinterpret_cast<IUnknown**>(&pDW_Factory)) ) )
        {
            throw std::exception("Ќе удалось создать фабрику DirectWrite");
        }
    }


    // ---------------------------
    // —оздание и отображение окна
    // ---------------------------

    Window_Handle = CreateWindowExW(
        WS_EX_DLGMODALFRAME,                    // Optional window styles
        reinterpret_cast<LPCTSTR>(Wnd_Class),   // Window class
        L"ћегакалькул€тор",                     // Window text
        WS_BORDER | WS_CAPTION | WS_SYSMENU,
        X,                                      // Size and position
        Y,
        Width,
        Height,
        NULL,                                   // Parent window
        NULL,                                   // Menu
        Application::Get_Instance(),            // Instance handle
        this);                                  // Additional application data

    if ( Window_Handle == NULL )
    {
        throw std::exception("Ќе удалось создать окно");
    }
}


void  Window::Free_Resources()
{
    Safe_Release(pBrush);
    Safe_Release(pRender_Target);
}


void  Window::Show()
{
    ShowWindow(Window_Handle, SW_RESTORE);
}


void  Window::Repaint()
{
    Begin_Paint();
    On_Paint();

    for ( auto  &C : Controls )
    {
        C->Paint();
    }

    End_Paint();
}


void  Window::Begin_Paint()
{
    HRESULT  HR = S_OK;

    if ( pRender_Target == nullptr )
    {
        RECT  RC;

        // ѕолучение размеров клиентской (т.е. основной) области окна в виде
        // координат левого верхнего и правого нижнего углов.  оординаты левого
        // верхнего угла всегда равны нулю.
        GetClientRect(Window_Handle, &RC);

        // ѕолучение размеров клиентской области; численно они равны координатам
        // еЄ правого нижнего угла.
        D2D1_SIZE_U  Size = D2D1::SizeU(RC.right, RC.bottom);

        // —оздание цели рендеринга (поверхности, на которой будет идти рисова-
        // ние). »спользуютс€ стандартные параметры, формируемые вызовами мето-
        // дов RenderTargetProperties и HwndRenderTargetProperties.
        HR = pD2D_Factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(Window_Handle, Size),
            &pRender_Target);

        if ( FAILED(HR) )
        {
            throw std::exception("Ќе удалось создать цель рендеринга");
        }

        D2D1_COLOR_F  Color = D2D1::ColorF(0.8f, 0.8f, 0.8f);

        HR = pRender_Target->CreateSolidColorBrush(Color, &pBrush);

        if ( FAILED(HR) )
        {
            throw std::exception("Ќе удалось создать кисть");
        }
    }

    BeginPaint(Window_Handle, &PS);

    pRender_Target->BeginDraw();

    pRender_Target->Clear(D2D1::ColorF(D2D1::ColorF::White));
}


void  Window::End_Paint()
{
    HRESULT  HR = pRender_Target->EndDraw();

    if ( FAILED(HR) || HR == D2DERR_RECREATE_TARGET )
    {
        Free_Resources();
    }

    EndPaint(Window_Handle, &PS);
}


void  Window::Rectangle(
    int            X,
    int            Y,
    std::uint32_t  Width,
    std::uint32_t  Height,
    std::uint32_t  Color)
{
    std::uint8_t  B = static_cast<std::uint8_t>(Color);
    std::uint8_t  G = static_cast<std::uint8_t>(Color >> 8);
    std::uint8_t  R = static_cast<std::uint8_t>(Color >> 16);
    std::uint8_t  A = static_cast<std::uint8_t>(Color >> 24);

    D2D1_COLOR_F  C;
    C.r = static_cast<float>(R) / 255.0f;
    C.g = static_cast<float>(G) / 255.0f;
    C.b = static_cast<float>(B) / 255.0f;
    C.a = static_cast<float>(A) / 255.0f;

    pBrush->SetColor(C);

    D2D1_RECT_F  Coords;
    Coords.left   = static_cast<FLOAT>(X);
    Coords.top    = static_cast<FLOAT>(Y);
    Coords.right  = static_cast<FLOAT>(X + Width);
    Coords.bottom = static_cast<FLOAT>(Y + Height);

    pRender_Target->FillRectangle(Coords, pBrush);
}


void  Window::Write_Text(
    const wchar_t  *Str,
    int             X,
    int             Y,
    std::uint32_t   Width,
    std::uint32_t   Height,
    Text_Format    *Format,
    std::uint32_t   Color)
{
    std::uint8_t  B = static_cast<std::uint8_t>(Color);
    std::uint8_t  G = static_cast<std::uint8_t>(Color >> 8);
    std::uint8_t  R = static_cast<std::uint8_t>(Color >> 16);
    std::uint8_t  A = static_cast<std::uint8_t>(Color >> 24);

    D2D1_COLOR_F  C;
    C.r = static_cast<float>(R) / 255.0f;
    C.g = static_cast<float>(G) / 255.0f;
    C.b = static_cast<float>(B) / 255.0f;
    C.a = static_cast<float>(A) / 255.0f;

    pBrush->SetColor(C);

    D2D1_RECT_F  Coords;
    Coords.left   = static_cast<FLOAT>(X);
    Coords.top    = static_cast<FLOAT>(Y);
    Coords.right  = static_cast<FLOAT>(X + Width);
    Coords.bottom = static_cast<FLOAT>(Y + Height);

    pRender_Target->DrawTextW(
        Str,
        static_cast<UINT32>(wcslen(Str)),
        Format->DW_Format,
        Coords,
        pBrush);
}


void  Window::Add_Control(Control  *C)
{
    Controls.push_back(C);
}
