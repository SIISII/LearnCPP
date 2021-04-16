#include "Common_Defs.h"

THIRD_PARTY_START

#include  <exception>

#include  <Windows.h>

#include  <DXGI1_6.h>
#include  <D3D10_1.h>
#include  <D3DX10.h>
#include  <D3DX10Math.h>

THIRD_PARTY_END

constexpr uint32  Render_Width  = 640;
constexpr uint32  Render_Height = 480;

constexpr wchar_t  Shader_File_Name[] = L"Shader.fx";


HWND                         g_hWnd;

IDXGIFactory7               *g_pIDXGI_Factory   = nullptr;
IDXGIAdapter4               *g_pDXGI_Adapter    = nullptr;
ID3D10Device1               *g_pD3D_Device      = nullptr;
IDXGISwapChain1             *g_pSwap_Chain      = nullptr;

ID3D10Effect                *g_pEffect          = nullptr;
ID3D10EffectTechnique       *g_pTechnique       = nullptr;

ID3D10InputLayout           *g_pVertex_Layout   = nullptr;
ID3D10Buffer                *g_pVertex_Buffer   = nullptr;
ID3D10Buffer                *g_pIndex_Buffer    = nullptr;

ID3D10EffectMatrixVariable  *g_pWorld           = nullptr;
ID3D10EffectMatrixVariable  *g_pView            = nullptr;
ID3D10EffectMatrixVariable  *g_pProjection      = nullptr;

D3DXMATRIX                   g_World;
D3DXMATRIX                   g_View;
D3DXMATRIX                   g_Projection;


struct  Vertex
{
    D3DXVECTOR3  Pos;
    D3DXVECTOR4  Color;
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
        L"DirectX 10: ��������� ���",
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
        IID_PPV_ARGS(&g_pIDXGI_Factory) ) );

    // ��������� ������������ ������ ����������������������� �������� � �������.
    // ������� - ��� ����������� ��������� (��) � ��, ��� ������������ ��� ��-
    // ����, ���� �� ��������� ���������� ��� �� ��, ����������� � ����� �����-
    // ����� � ����������� ����������� (��).
    CHECK_HR( g_pIDXGI_Factory->EnumAdapterByGpuPreference(
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

    CHECK_HR( g_pIDXGI_Factory->CreateSwapChainForHwnd(
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

    // ����������� ����� ����� ����������� ��������� � ��������.
    g_pWorld      = g_pEffect->GetVariableByName("World")->AsMatrix();
    g_pView       = g_pEffect->GetVariableByName("View")->AsMatrix();
    g_pProjection = g_pEffect->GetVariableByName("Projection")->AsMatrix();

    // �������� ������� ("���������" - layout) ���������� ������.
    D3D10_INPUT_ELEMENT_DESC  Layout[2];

    Layout[0].SemanticName         = "POSITION";
    Layout[0].SemanticIndex        = 0;
    Layout[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
    Layout[0].InputSlot            = 0;
    Layout[0].AlignedByteOffset    = 0;
    Layout[0].InputSlotClass       = D3D10_INPUT_PER_VERTEX_DATA;
    Layout[0].InstanceDataStepRate = 0;

    Layout[1].SemanticName         = "COLOR";
    Layout[1].SemanticIndex        = 0;
    Layout[1].Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
    Layout[1].InputSlot            = 0;
    Layout[1].AlignedByteOffset    = 12;
    Layout[1].InputSlotClass       = D3D10_INPUT_PER_VERTEX_DATA;
    Layout[1].InstanceDataStepRate = 0;

    D3D10_PASS_DESC  Pass_Desc;

    CHECK_HR( g_pTechnique->GetPassByIndex(0)->GetDesc(&Pass_Desc) );

    CHECK_HR( g_pD3D_Device->CreateInputLayout(
        &Layout[0],                         // pInputElementDescs
        2,                                  // NumElements
        Pass_Desc.pIAInputSignature,        // pShaderBytecodeWithInputSignature
        Pass_Desc.IAInputSignatureSize,     // BytecodeLength
        &g_pVertex_Layout) );               // ppInputLayout

    // ��� �������, �������� ��������� �����������.
    Vertex  Vertices[] =
    {
        {   // 0
            D3DXVECTOR3(-0.5f,  0.5f, -0.5f),
            D3DXVECTOR4( 1.0f,  0.0f,  0.0f,  1.0f)
        },
        {   // 1
            D3DXVECTOR3( 0.5f,  0.5f, -0.5f),
            D3DXVECTOR4( 1.0f,  0.0f,  0.0f,  1.0f)
        },
        {   // 2
            D3DXVECTOR3( 0.5f,  0.5f,  0.5f),
            D3DXVECTOR4( 1.0f,  0.0f,  0.0f,  1.0f)
        },
        {   // 3
            D3DXVECTOR3(-0.5f,  0.5f,  0.5f),
            D3DXVECTOR4( 1.0f,  0.0f,  0.0f,  1.0f)
        },

        {   // 4
            D3DXVECTOR3(-0.5f, -0.5f, -0.5f),
            D3DXVECTOR4( 0.0f,  1.0f,  0.0f,  1.0f)
        },
        {   // 5
            D3DXVECTOR3( 0.5f, -0.5f, -0.5f),
            D3DXVECTOR4( 0.0f,  1.0f,  0.0f,  1.0f)
        },
        {   // 6
            D3DXVECTOR3( 0.5f, -0.5f,  0.5f),
            D3DXVECTOR4( 0.0f,  1.0f,  0.0f,  1.0f)
        },
        {   // 7
            D3DXVECTOR3(-0.5f, -0.5f,  0.5f),
            D3DXVECTOR4( 0.0f,  1.0f,  0.0f,  1.0f)
        },

        {   // 8
            D3DXVECTOR3(-0.5f, -0.5f,  0.5f),
            D3DXVECTOR4( 0.0f,  0.0f,  1.0f,  1.0f)
        },
        {   // 9
            D3DXVECTOR3(-0.5f, -0.5f, -0.5f),
            D3DXVECTOR4( 0.0f,  0.0f,  1.0f,  1.0f)
        },
        {   // 10
            D3DXVECTOR3(-0.5f,  0.5f, -0.5f),
            D3DXVECTOR4( 0.0f,  0.0f,  1.0f,  1.0f)
        },
        {   // 11
            D3DXVECTOR3(-0.5f,  0.5f,  0.5f),
            D3DXVECTOR4( 0.0f,  0.0f,  1.0f,  1.0f)
        },

        {   // 12
            D3DXVECTOR3( 0.5f, -0.5f,  0.5f),
            D3DXVECTOR4( 0.0f,  1.0f,  1.0f,  1.0f)
        },
        {   // 13
            D3DXVECTOR3( 0.5f, -0.5f, -0.5f),
            D3DXVECTOR4( 0.0f,  1.0f,  1.0f,  1.0f)
        },
        {   // 14
            D3DXVECTOR3( 0.5f,  0.5f, -0.5f),
            D3DXVECTOR4( 0.0f,  1.0f,  1.0f,  1.0f)
        },
        {   // 15
            D3DXVECTOR3( 0.5f,  0.5f,  0.5f),
            D3DXVECTOR4( 0.0f,  1.0f,  1.0f,  1.0f)
        },

        {   // 16
            D3DXVECTOR3(-0.5f, -0.5f, -0.5f),
            D3DXVECTOR4( 1.0f,  0.0f,  1.0f,  1.0f)
        },
        {   // 17
            D3DXVECTOR3( 0.5f, -0.5f, -0.5f),
            D3DXVECTOR4( 1.0f,  0.0f,  1.0f,  1.0f)
        },
        {   // 18
            D3DXVECTOR3( 0.5f,  0.5f, -0.5f),
            D3DXVECTOR4( 1.0f,  0.0f,  1.0f,  1.0f)
        },
        {   // 19
            D3DXVECTOR3(-0.5f,  0.5f, -0.5f),
            D3DXVECTOR4( 1.0f,  0.0f,  1.0f,  1.0f)
        },

        {   // 20
            D3DXVECTOR3(-0.5f, -0.5f,  0.5f),
            D3DXVECTOR4( 1.0f,  1.0f,  0.0f,  1.0f)
        },
        {   // 21
            D3DXVECTOR3( 0.5f, -0.5f,  0.5f),
            D3DXVECTOR4( 1.0f,  1.0f,  0.0f,  1.0f)
        },
        {   // 22
            D3DXVECTOR3( 0.5f,  0.5f,  0.5f),
            D3DXVECTOR4( 1.0f,  1.0f,  0.0f,  1.0f)
        },
        {   // 23
            D3DXVECTOR3(-0.5f,  0.5f,  0.5f),
            D3DXVECTOR4( 1.0f,  1.0f,  0.0f,  1.0f)
        }
    };

    // ������� ������ ��� ������������ �������������.
    uint32  Indices[] =
    {
        // �������� �����.
        19, 17, 16,
        18, 17, 19,

        // ������ �����.
        22, 20, 21,
        23, 20, 22,

        // ������� �����.
        3, 1, 0,
        2, 1, 3,

        // ������ �����.
        6, 4, 5,
        7, 4, 6,

        // ����� �����.
        11, 9, 8,
        10, 9, 11,

        // ������ �����.
        14, 12, 13,
        15, 12, 14
    };

    // �������� ���������� ������. �� ����� �������� � ���� �� �� �������, ��
    // ����������� ���, ��� ������� Direct3D � ������������ � ����������� ����-
    // ������� (� ������ ������ - � ����������� ������� � �� � �������� �� ��,
    // ��������� ��������� �� ����� �������� � ���� ������� ������).
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
    
    // �������� ���������� ������. ���������� �� �� ���������, ��� � ��� ������-
    // ���� ������: ������������ � ��� ���������� ��� �� �����, ��� ��� ������-
    // ��� ����� ��� ������.
    BD.Usage          = D3D10_USAGE_DEFAULT;
    BD.ByteWidth      = sizeof(Indices);
    BD.BindFlags      = D3D10_BIND_INDEX_BUFFER,
    BD.CPUAccessFlags = 0;
    BD.MiscFlags      = 0;

    // ���������� �������� ���������� ������.
    Init_Data.pSysMem          = Indices;
    Init_Data.SysMemPitch      = 0;
    Init_Data.SysMemSlicePitch = 0;

    CHECK_HR( g_pD3D_Device->CreateBuffer(&BD, &Init_Data, &g_pIndex_Buffer) );
    
    // ������� ������� ����� ���������, �.�. ���������� ������� (������������)
    // � ��� ����������� � � ������� �������� ������� ����� ���������.
    D3DXMatrixIdentity(&g_World);

    // ������� ���� (��� ������ ������� � ��� - � ��������� � �����������
    // �������).
    D3DXVECTOR3  Eye(0.0f, 2.0f, -3.0f);
    D3DXVECTOR3  At(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3  Up(0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtLH(&g_View, &Eye, &At, &Up);

    // ������� �������� (��� ��������� ����� ����� �������������� �� �������
    // "�����" ������).
    D3DXMatrixPerspectiveFovLH(
        &g_Projection,
        static_cast<float>(D3DX_PI) / 4.0f,
        static_cast<float>(Render_Width) / static_cast<float>(Render_Height),
        0.1f,
        100.0f);
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

    Safe_Release(g_pIndex_Buffer);
    Safe_Release(g_pVertex_Buffer);
    Safe_Release(g_pVertex_Layout);
    Safe_Release(g_pEffect);
    Safe_Release(g_pSwap_Chain);
    Safe_Release(g_pD3D_Device);
    Safe_Release(g_pDXGI_Adapter);
    Safe_Release(g_pIDXGI_Factory);
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

    // ���� �������� ����.
    static float  Angle = 0.0f;

    // ��������� �����.
    static uint32  Last_Time = GetTickCount();

    // �����, ��������� � ������� ���������� ����������.
    uint32  Cur_Time = GetTickCount();

    uint32  Delta_Time =  Cur_Time - Last_Time;

    Last_Time = Cur_Time;

    Angle += static_cast<float>(Delta_Time) / 1000.0f;
    
    // ������������� ������� ������� ��� �������� ����.
    D3DXMatrixRotationY(&g_World, Angle);

    // �������� ������ � ���������� ��������.
    g_pWorld->SetMatrix(reinterpret_cast<float*>(&g_World));
    g_pView->SetMatrix(reinterpret_cast<float*>(&g_View));
    g_pProjection->SetMatrix(reinterpret_cast<float*>(&g_Projection));

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

    // �������� ���������� ������.
    g_pD3D_Device->IASetIndexBuffer(
        g_pIndex_Buffer,        // pIndexBuffer
        DXGI_FORMAT_R32_UINT,   // Format
        0);                     // Offset

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
        g_pD3D_Device->DrawIndexed(36, 0, 0);
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
