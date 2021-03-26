#include "Common_Defs.h"

THIRD_PARTY_START

#include  <Windows.h>
#include  <exception>
#include  <DXGI1_6.h>
#include  <D3D10_1.h>

THIRD_PARTY_END

#pragma comment(lib, "DXGI")
#pragma comment(lib, "D3D10_1")

constexpr uint32  Render_Width  = 640;
constexpr uint32  Render_Height = 480;

HWND                     g_hWnd;

IDXGIFactory7           *g_pIDXGI_Factory       = nullptr;
IDXGIAdapter4           *g_pDXGI_Adapter        = nullptr;

// =============================================================================
//
// Safe_Release
//
// =============================================================================

template< typename  T >
void  Safe_Release(T*  &Ptr)
{
    if ( Ptr != nullptr )
    {
        Ptr->Release();
        Ptr = nullptr;
    }
}


// =============================================================================
//
// CHECK_HR
//
// =============================================================================

static inline void  CHECK_HR(HRESULT  HR)
{
    if ( FAILED(HR) )
    {
        throw  std::exception();
    }
}


// =============================================================================
//
// Wnd_Proc
//
// =============================================================================

LRESULT CALLBACK  Wnd_Proc(
    HWND    hWnd,
    UINT    Message,
    WPARAM  wParam,
    LPARAM  lParam)
{
    switch ( Message )
    {
        case  WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            break;
    }

    return DefWindowProcW(hWnd, Message, wParam, lParam);
}


// =============================================================================
//
// Init_Window
//
// =============================================================================

void  Init_Window(
    HINSTANCE  hInstance,
    int        nCmdShow)
{
    constexpr wchar_t  Class_Name[] = L"DX10_Test_Window_Class";

    WNDCLASSEXW  WC;

    WC.cbSize           = sizeof(WNDCLASSEXW);
    WC.style            = CS_HREDRAW | CS_VREDRAW;
    WC.lpfnWndProc      = Wnd_Proc;
    WC.cbClsExtra       = 0;
    WC.cbWndExtra       = 0;
    WC.hInstance        = hInstance;
    WC.hIcon            = NULL;
    WC.hCursor          = LoadCursorW(NULL, IDC_ARROW);
    WC.hbrBackground    = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    WC.lpszMenuName     = nullptr;
    WC.lpszClassName    = Class_Name;
    WC.hIconSm          = LoadIconW(NULL, IDI_APPLICATION);

    if ( !RegisterClassEx(&WC) )
    {
        throw std::exception();
    }

    RECT  Rect = { 0, 0, Render_Width, Render_Height };

    AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, false);

    g_hWnd = CreateWindowW(
        Class_Name,
        L"DirectX 10: Простейшая программа",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        Rect.right,
        Rect.bottom,
        NULL,
        NULL,
        hInstance,
        NULL);

    if ( g_hWnd == NULL )
    {
        throw std::exception();
    }

    ShowWindow(g_hWnd, nCmdShow);
}


// =============================================================================
//
// Init_Direct3D10_1
//
// =============================================================================

void  Init_Direct3D10_1()
{
    // Создание фабрики DXGI, с помощью которой создаются различные объекты для
    // работы с DirectX.

    CHECK_HR( CreateDXGIFactory2(
#ifdef  _DEBUG
        DXGI_CREATE_FACTORY_DEBUG   |
#endif
        0,
        IID_PPV_ARGS(&g_pIDXGI_Factory) ) );

    // Получение теоретически самого высокопроизводительного адаптера в системе.
    // Адаптер - это графический процессор (ГП) и всё, что обеспечивает его ра-
    // боту, будь то отдельная видеокарта или же ГП, объединённый в одной микро-
    // схеме с центральным процессором (ЦП).

    CHECK_HR( g_pIDXGI_Factory->EnumAdapterByGpuPreference(
        0,                                      // Порядковый номер адаптера
        DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,   // Критерий выбора адаптера
        IID_PPV_ARGS(&g_pDXGI_Adapter) ) );     // Объект адаптера

    // Получение свойств выбранного адаптера. Реально это не требуется и здесь
    // сделано, чтобы можно было визуально проконтролировать, тот ли адаптер
    // был выбран.

    DXGI_ADAPTER_DESC3  Adapter_Desc;

    CHECK_HR( g_pDXGI_Adapter->GetDesc3(&Adapter_Desc) );
}


// =============================================================================
//
// Cleanup
//
// =============================================================================

void  Cleanup()
{
    Safe_Release(g_pDXGI_Adapter);
    Safe_Release(g_pIDXGI_Factory);
}


// =============================================================================
//
// wWinMain
//
// =============================================================================

int WINAPI  wWinMain(
    HINSTANCE  hInstance,
    HINSTANCE,
    PWSTR,
    int        nCmdShow)
{
    Init_Window(hInstance, nCmdShow);

    Init_Direct3D10_1();

    MSG  Message = {};

    while ( Message.message != WM_QUIT )
    {
        if ( PeekMessageW(&Message, NULL, 0, 0, PM_REMOVE) )
        {
            TranslateMessage(&Message);
            DispatchMessageW(&Message);
        }
        else
        {
//            Render_Scene();
        }
    }

    Cleanup();

    return static_cast<int>(Message.wParam);
}