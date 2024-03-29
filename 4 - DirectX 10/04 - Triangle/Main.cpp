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

constexpr wchar_t  Shader_File_Name[] = L"Shader.fx";

HWND                        g_hWnd;

IDXGIFactory7              *g_pDXGI_Factory     = nullptr;
IDXGIAdapter4              *g_pDXGI_Adapter     = nullptr;
ID3D10Device1              *g_pD3D_Device       = nullptr;
IDXGISwapChain1            *g_pSwap_Chain       = nullptr;

ID3D10Effect               *g_pEffect           = nullptr;
ID3D10EffectTechnique      *g_pTechnique        = nullptr;

ID3D10InputLayout          *g_pVertex_Layout    = nullptr;
ID3D10Buffer               *g_pVertex_Buffer    = nullptr;


struct  Vertex
{
    D3DXVECTOR3  Pos;
};


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
        L"DirectX 10: �����������",
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
    // ����, ���� �� ��������� ���������� ��� �� ��, ������������ � ����� �����-
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
    // ����, � � ��� ����� ����� ������� ��� ������.
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

    // ���������� �������� ("��������"), ����������� � �������� �����.
    CHECK_HR( D3DX10CreateEffectFromFileW(
        Shader_File_Name,                       // pFileName
        nullptr,                                // pDefines
        nullptr,                                // pInclude
        "fx_4_1",                               // pProfile
#ifdef  _DEBUG
        D3D10_SHADER_DEBUG              |
#endif
        D3D10_SHADER_ENABLE_STRICTNESS  |
        D3D10_SHADER_WARNINGS_ARE_ERRORS,       // HLSLFlags
        0,                                      // FXFlags
        g_pD3D_Device,                          // pDevice
        nullptr,                                // pEffectPool
        nullptr,                                // pPump
        &g_pEffect,                             // ppEffect
        nullptr,                                // ppErrors
        nullptr) );                             // pHResult

    // ��������� "�������", �.�. ������� ���������� ���������������� ��������.
    g_pTechnique = g_pEffect->GetTechniqueByName("Render_White");

    // �������� ������� ("���������" - layout) ���������� ������.
    D3D10_INPUT_ELEMENT_DESC  Layout;

    Layout.SemanticName         = "POSITION";
    Layout.SemanticIndex        = 0;
    Layout.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
    Layout.InputSlot            = 0;
    Layout.AlignedByteOffset    = 0;
    Layout.InputSlotClass       = D3D10_INPUT_PER_VERTEX_DATA;
    Layout.InstanceDataStepRate = 0;

    D3D10_PASS_DESC  Pass_Desc;

    CHECK_HR( g_pTechnique->GetPassByIndex(0)->GetDesc(&Pass_Desc) );

    CHECK_HR( g_pD3D_Device->CreateInputLayout(
        &Layout,                            // pInputElementDescs
        1,                                  // NumElements
        Pass_Desc.pIAInputSignature,        // pShaderBytecodeWithInputSignature
        Pass_Desc.IAInputSignatureSize,     // BytecodeLength
        &g_pVertex_Layout) );               // ppInputLayout

    // ��� �������, �������� ��������� �����������.
    Vertex  Vertices[] =
    {
        D3DXVECTOR3( 0.0f,  0.5f, 0.5f),
        D3DXVECTOR3( 0.5f, -0.5f, 0.5f),
        D3DXVECTOR3(-0.5f, -0.5f, 0.5f)
    };

    // �������� ���������� ������. �� ����� �������� � ���� �� �� ��� �������,
    // �� ����������� ���, ��� ������� Direct3D � ������������ � �����������
    // ����������� (� ������ ������ - � ����������� ������� � �� � �������� ��
    // ��, ��������� ��������� �� ����� �������� � ���� ������� ������).
    D3D10_BUFFER_DESC  BD;

    BD.Usage          = D3D10_USAGE_DEFAULT;
    BD.ByteWidth      = sizeof(Vertices);
    BD.BindFlags      = D3D10_BIND_VERTEX_BUFFER,
    BD.CPUAccessFlags = 0;
    BD.MiscFlags      = 0;

    // ���������� �������� ���������� ������ � ������������ �������� ���� ����-
    // ����� ������ � ��������� ����� � �����������.
    D3D10_SUBRESOURCE_DATA  Init_Data;

    Init_Data.pSysMem          = Vertices;
    Init_Data.SysMemPitch      = 0;
    Init_Data.SysMemSlicePitch = 0;

    CHECK_HR( g_pD3D_Device->CreateBuffer(&BD, &Init_Data, &g_pVertex_Buffer) );
}


// =============================================================================
//
// Cleanup
//
// =============================================================================

void  Cleanup()
{
    if ( g_pD3D_Device != nullptr )
    {
        g_pD3D_Device->ClearState();
    }

    Safe_Release(g_pVertex_Buffer);
    Safe_Release(g_pVertex_Layout);
    Safe_Release(g_pEffect);
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
    const float  Clear_Color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    // �������� ��������� �������� ������ � ������ ������ �� ����� ������������
    // ���������.
    uint32  Stride = sizeof(Vertex);
    uint32  Offset = 0;

    g_pD3D_Device->IASetInputLayout(g_pVertex_Layout);

    g_pD3D_Device->IASetVertexBuffers(
        0,                      // StartSlot
        1,                      // NumBuffers
        &g_pVertex_Buffer,      // ppVertexBuffers
        &Stride,                // pStrides
        &Offset);               // pOffsets

    g_pD3D_Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

    // ������ ����������: ������� ���������� ������ (�.�. �������� ��� ��������
    // ������ ����).
    g_pD3D_Device->ClearRenderTargetView(pRender_Target_View, Clear_Color);

    // ���������� ���������. ����������� �������, �������� � ������������ ���-
    // ����, ��� ������� ������� ����������� ��������� (����� Draw) - �.�. ��-
    // ������� ������� ������ ����������� ���������� � ������������ � �������-
    // ���� (������������� ��������� � �.�.), ���������� �������� ��� �������
    // �������.
    D3D10_TECHNIQUE_DESC  TD;

    CHECK_HR( g_pTechnique->GetDesc(&TD) );

    for ( uint32  I = 0; I < TD.Passes; ++I )
    {
        g_pTechnique->GetPassByIndex(I)->Apply(0);
        g_pD3D_Device->Draw(3, 0);
    }

    // ����� ��������������� �� ��������� ������ ����������� �� ����� (�������-
    // ����� �������).
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
