#include "Common_Defs.h"

THIRD_PARTY_START

#include  <exception>

#include  <Windows.h>

#include  <DXGI1_6.h>
#include  <D3D10_1.h>
#include  <D3DX10.h>

THIRD_PARTY_END


constexpr uint32  Render_Width  = 640;
constexpr uint32  Render_Height = 480;

static const wchar_t  File_Name[] = L"Picture.bmp";


HWND                        g_hWnd;

IDXGIFactory7              *g_pDXGI_Factory     = nullptr;
IDXGIAdapter4              *g_pDXGI_Adapter     = nullptr;
ID3D10Device1              *g_pD3D_Device       = nullptr;
IDXGISwapChain1            *g_pSwap_Chain       = nullptr;
ID3D10ShaderResourceView   *g_pShader_Resource  = nullptr;
ID3DX10Sprite              *g_pSprite           = nullptr;


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
        L"DirectX 10: ����� �������",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        Rect.right - Rect.left,
        Rect.bottom - Rect.top,
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
    // �������� ������� DXGI, � ������� ������� ��������� ��������� ������� ���
    // ������ � DirectX.
    CHECK_HR( CreateDXGIFactory2(
#ifdef  _DEBUG
        DXGI_CREATE_FACTORY_DEBUG   |
#endif
        0,
        IID_PPV_ARGS(&g_pDXGI_Factory) ) );

    // ��������� ������������ ������ ����������������������� �������� � �������.
    // ������� - ��� ����������� ��������� (��) � ��, ��� ������������ ��� ��-
    // ����, ���� �� ��������� ���������� ��� �� ��, ����������� � ����� �����-
    // ����� � ����������� ����������� (��).
    CHECK_HR( g_pDXGI_Factory->EnumAdapterByGpuPreference(
        0,                                      // ���������� ����� ��������
        DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,   // �������� ������ ��������
        IID_PPV_ARGS(&g_pDXGI_Adapter) ) );     // ������ ��������

    // ��������� ������� ���������� ��������. ������� ��� �� ��������� � �����
    // �������, ����� ����� ���� ��������� �����������������, ��� �� �������
    // ��� ������.
    DXGI_ADAPTER_DESC3  Adapter_Desc;

    CHECK_HR( g_pDXGI_Adapter->GetDesc3(&Adapter_Desc) );

    // �������� ���������� Direct3D10.1 - �.�. ������� ��� �������������� � ���-
    // �������� ����������� (��), �� ������� ����� ����������� ���������.
    CHECK_HR( D3D10CreateDevice1(
        g_pDXGI_Adapter,                    // pAdapter - ����� �� ������������
        D3D10_DRIVER_TYPE_HARDWARE,         // ��� �������� - �������� ������
        NULL,                               // Software - ������ NULL
#ifdef  _DEBUG
        D3D10_CREATE_DEVICE_DEBUG   |
#endif
        D3D10_CREATE_DEVICE_SINGLETHREADED, // Flags
        D3D10_FEATURE_LEVEL_10_1,           // HardwareLevel
        D3D10_1_SDK_VERSION,                // SDKVersion
        &g_pD3D_Device) );                  // ppDevice

    // �������� ������� ������������ �������, � ������� ����� ��������������
    // ��������� � ������� ����� ������������ �� ������. 
    //
    // � ������������� ������ ���������� ����� ������� ��� ������ - ��������� �
    // ��������� (���������� ���������� ������ ������������ �� ��������, � ��
    // ����� ��� �� ��������� ������������ ���������, �.�. ����������� ��������-
    // ��� ���������� �����; ����� ��������� ��������, ���������� ������������
    // �������: ��������� ����� ���������� ��������� � ��� ����� ������������ ��
    // ������, � ��������� ����� ���������� ���������, � ������� ����� �������-
    // ������ ��������� ����).
    //
    // � ������� ������ ������������ ���������� ������ ������, ��� ��� ���������
    // ������� ������� ���� �� (��������� ������, �� ����� ������� ����), � ���-
    // ����� �� ���������� ������ �����-���� ������� ����������� � ������ �����
    // ���������� ������. ������ � DirectX12 �������� ������ ��� �� ����������-
    // ����, � � �� ����� ����� ������� ��� ������.
    DXGI_SWAP_CHAIN_DESC1  SCD;

    SCD.Width               = Render_Width;
    SCD.Height              = Render_Height;
    SCD.Format              = DXGI_FORMAT_R8G8B8A8_UNORM;
    SCD.Stereo              = false;
    SCD.SampleDesc.Count    = 1;
    SCD.SampleDesc.Quality  = 0;
    SCD.BufferUsage         = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SCD.BufferCount         = 2;
    SCD.Scaling             = DXGI_SCALING_NONE;
    SCD.SwapEffect          = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    SCD.AlphaMode           = DXGI_ALPHA_MODE_IGNORE;
    SCD.Flags               = 0;

    CHECK_HR( g_pDXGI_Factory->CreateSwapChainForHwnd(
        g_pD3D_Device,                          // pDevice
        g_hWnd,                                 // hWnd
        &SCD,                                   // pDesc
        nullptr,                                // pFullscreenDesc
        nullptr,                                // pRestrictToOutput
        &g_pSwap_Chain) );                      // ppSwapChain

    // �������� ������� (��������) �� �����. ������� ���������� �������� �����-
    // ����� �� ���� ��������, ���������� � ����� �����. ������ � �������� ��-
    // ������� ������� D3DX10GetImageInfoFromFileW ������������, ���� ��������
    // ���������� � ����� ����������� ���������� (�.�. ���� ����� ���� �������
    // ��������� ������� �������� � ������ ������, �� �� ���������� ��� �����-
    // �����).
    D3DX10_IMAGE_INFO  Image_Info;

    CHECK_HR( D3DX10GetImageInfoFromFileW(
        File_Name,                              // pSrcFile
        nullptr,                                // pPump
        &Image_Info,                            // pSrcInfo
        nullptr) );                             // pSrcInfo

    // ������� ���������� � �������, ����� ��������� ��� ��������. ���� ��� ��-
    // ������������ ���������� ��� ��� ���������� ��������� �����, ����������
    // � �����, �� ����� �� �������� ���������� �� ����, � ��������� ���������
    // ���� D3DX10_IMAGE_LOAD_INFO ���������� D3DX10_DEFAULT - ����� �������-
    // �������� ������ ����� �������� ����� �� �����. ���� ����� ������������
    // ����� ���������� �� �����, � ����� ������� ��������������, ����� ������-
    // ������ �������� D3DX10_DEFAULT ������ ��� ��� �����, � ������� ����������
    // ����� ���������� ����� �� �����. ����� ����, ����� �������� � �������� �
    // ����� ����������� � ��� ��������� (� ������ ������ ������� ������ ��� - �
    // ����� ����������� ��������� �������, � �� ��-�� �������� �������������).
    //
    // ������� D3DX10CreateShaderResourceViewFromFileW ������ �������������
    // (view) ������� (� ������ ������ ��������, �.�. ��������) - �.�. ��������
    // ����, ��� ������ (���������, ����������� ����������� ����������� �� �����
    // ����������) ����� �������� � ���� ��������.
    D3DX10_IMAGE_LOAD_INFO  Load_Image_Info;

    Load_Image_Info.Width           = Image_Info.Width;
    Load_Image_Info.Height          = Image_Info.Height;
    Load_Image_Info.Depth           = Image_Info.Depth;
    Load_Image_Info.FirstMipLevel   = 1;
    Load_Image_Info.MipLevels       = Image_Info.MipLevels;
    Load_Image_Info.Usage           = D3D10_USAGE_DEFAULT;
    Load_Image_Info.BindFlags       = D3D10_BIND_SHADER_RESOURCE;
    Load_Image_Info.CpuAccessFlags  = 0;
    Load_Image_Info.MiscFlags       = 0;
    Load_Image_Info.Format          = Image_Info.Format;
    Load_Image_Info.Filter          = D3DX10_FILTER_NONE;
    Load_Image_Info.MipFilter       = D3DX10_FILTER_NONE;
    Load_Image_Info.pSrcInfo        = &Image_Info;

    CHECK_HR( D3DX10CreateShaderResourceViewFromFileW(
        g_pD3D_Device,                          // pDevice
        File_Name,                              // pSrcFile
        &Load_Image_Info,                       // pLoadInfo
        nullptr,                                // pPump
        &g_pShader_Resource,                    // ppShaderResourceView
        nullptr) );                             // pHResult

    // �������� �������, �.�. ��������� ��� ��������� ������� ������� �������
    // ���������� ������ DirectX ��� ������ ������������� �������� ������������.
    CHECK_HR( D3DX10CreateSprite(g_pD3D_Device, 1, &g_pSprite) );
}


// =============================================================================
//
// Cleanup
//
// =============================================================================

void  Cleanup()
{
    Safe_Release(g_pSprite);
    Safe_Release(g_pShader_Resource);
    Safe_Release(g_pSwap_Chain);
    Safe_Release(g_pD3D_Device);
    Safe_Release(g_pDXGI_Adapter);
    Safe_Release(g_pDXGI_Factory);
}


// =============================================================================
//
// Render_Scene
//
// =============================================================================

void  Render_Scene()
{
    // ����������� ����� �������� ����.
    const float  Clear_Color[4] = { 1.0f, 0.0f, 0.5f, 1.0f };

    // ��������� ���� ������ (viewport), �.�. ����������� ����� ���� ����������
    // (������ �����), � ������� ����� �������������� ����� �����������. ����
    // ������ ������������ �� ������ ������������, �.�. ����������� ���������
    // ���������� (�������������, ����� � �����) � �������.
    D3D10_VIEWPORT  VP;

    VP.TopLeftX = 0;
    VP.TopLeftY = 0;
    VP.Width    = Render_Width;
    VP.Height   = Render_Height;
    VP.MinDepth = 0.0f;
    VP.MaxDepth = 1.0f;

    g_pD3D_Device->RSSetViewports(1, &VP);

    // �������� ������������� ���� ���������� (render target view). ����������-
    // ��� - ��� �������� ������� � ������, ���������� ��������� �������� (�����
    // ���������� � ������ ������); ��� ����� ��������� � ��������� ������ ���-
    // ��������� ���������, ����� �� ����, ��� � ���� ���������� �������������
    // ����������.
    //
    // ����� ������� �������������, ������� ���� �������� ���� ����������, �.�.
    // ��������� �����. ������� ������������ ��������� ����� ������ ����� �����
    // 0, ��������� Direct3D ��� ������������ ������� ������ �� �������.
    ID3D10Resource          *pBack_Buffer;
    ID3D10RenderTargetView  *pRender_Target_View;

    CHECK_HR( g_pSwap_Chain->GetBuffer(0, IID_PPV_ARGS(&pBack_Buffer) ) );

    CHECK_HR( g_pD3D_Device->CreateRenderTargetView(
        pBack_Buffer,
        nullptr,
        &pRender_Target_View) );

    Safe_Release(pBack_Buffer);

    // �������� ������������� � ���������.
    g_pD3D_Device->OMSetRenderTargets(1, &pRender_Target_View, nullptr);

    // ������ ����������: ������� ������ (�������� ��� ������� ������).
    g_pD3D_Device->ClearRenderTargetView(pRender_Target_View, Clear_Color);

    // ������� D3DXMatrixTranslation ��������� ��� ���������� ������� ��������-
    // ���, �����������, � ����� ����� ���������� ������������ ���� �����������
    // ����� ������. � ����� ������ ������ ����� ������������� � ������ ������-
    // ���, ������� ��� ��� ��� ���������� ����� ����. ���������� ������� ����-
    // ������� ����� ������ � ��� ���� ������� �������, �.�. �������, ��������
    // ��� ������� �� ������ ��������� � ������������, �� � ����������, � ���-
    // ����: ��� ���� �� ��������� ������ ��� ���������.
    D3DXMATRIX  M_World;

    D3DXMatrixTranslation(&M_World, 0.0f, 0.0f, 0.0f);

    // ��������� ������� ��������. ��� ������� �������� �� �������������� ���-
    // ������ ����� � ������� �����������. ��� ���������� ������� ������� ���-
    // ��: ���� ������ �� ���������, ����������� ������ (�� �������� ����� ��-
    // ����� ���� ������ �� �����������) � ������� ������� � ������� ����������
    // ��������� (� ������� ��������� �������� ���� �������, ����������� �����
    // ����� �����������).
    D3DXMATRIX  M_Projection;

    D3DXMatrixPerspectiveFovLH(
        &M_Projection,
        static_cast<float>(D3DX_PI) / 4.0f,
        1.0f,
        0.0f,
        1.0f);

    // ��������� ������� �������� ��� ���� �������� �����.
    g_pSprite->SetProjectionTransform(&M_Projection);

    // ��������� ������� ���� - ��� ����������, � ����� ����� ��������� �����-
    // ������ ("������"), ���� �� ������� � ��� ��� ���� ��������� ����. �����
    // �� ��������� �, ����� ��� �������: ������ �������� ���������� �����, ���
    // ��������� ������, ������ - �����, �� ������� ������� ������, ������ ����-
    // ������ ����������� �����.
    D3DXMATRIX  M_View;

    D3DXVECTOR3  V_Eye_Point(0.0f, 0.0f, -3.0f);

    D3DXVECTOR3  V_Lookat_Point(0.0f, 0.0f, 0.0f);

    D3DXVECTOR3  V_Up_Vec(0.0f, 1.0f, 0.0f);

    D3DXMatrixLookAtLH(&M_View, &V_Eye_Point, &V_Lookat_Point, &V_Up_Vec);

    // ���������� ������� ���� �� ���� ��������.
    g_pSprite->SetViewTransform(&M_View);

    // ��������� �������� ������. ���� D3DX10_SPRITE_SORT_TEXTURE ���������
    // ������� Begin, ��� ���� ������ �������� �������, ������������ ���� �
    // �� �� ���������, ��� �������� ������������������. � ����� ������ ���
    // ���� �� ������, ��� ��� ������ � ��� ����� ����.
    g_pSprite->Begin(D3DX10_SPRITE_SORT_TEXTURE);

    // ��������� ���� D3DX10_SPRITE ��������� �������������� �������, �������
    // ����� ����������. � ��� �������� ���������, ���������� � ������� �������
    // � ������������ (������� �������), ������������ �������� (��� ����������
    // ��������, ��� � ���������� ������ �������� � ������� ������� ����� ���
    // ����� ��������, ������� ����� ������������ �� ������) � ����, �������
    // ������������ ���� �������� �������� ��� �������� ������� (�������������
    // � ������ ������ - ��� ��������� �������� ������; �� ������� �������� 1,
    // ��� ��������, ��� �������� ����� �������� �������� ���������� �� �����).
    D3DX10_SPRITE  Sprite_To_Draw;

    Sprite_To_Draw.matWorld      = M_World;

    Sprite_To_Draw.TexCoord.x    = 0.0f;
    Sprite_To_Draw.TexCoord.y    = 0.0f;

    Sprite_To_Draw.TexSize.x     = 1.0f;
    Sprite_To_Draw.TexSize.y     = 1.0f;

    Sprite_To_Draw.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    Sprite_To_Draw.pTexture      = g_pShader_Resource;

    Sprite_To_Draw.TextureIndex  = 0;

    // ���������� ������� � ������� �� ���������.
    CHECK_HR( g_pSprite->DrawSpritesBuffered(&Sprite_To_Draw, 1) );

    // �������� ��������������� ������� �� ���������� ��� ������������ �������-
    // ��� ����������.
    g_pSprite->Flush();

    // ���������� ������ �� ��������.
    g_pSprite->End();

    // ����� ��������������� ����������� (������������ �������).
    g_pSwap_Chain->Present(0, DXGI_PRESENT_RESTART);

    Safe_Release(pRender_Target_View);
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
            Render_Scene();
        }
    }

    Cleanup();

    return static_cast<int>(Message.wParam);
}
