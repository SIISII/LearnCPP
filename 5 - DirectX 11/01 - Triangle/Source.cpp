#pragma  comment(lib, "d3d11.lib")
#pragma  comment(lib, "dxgi.lib")
#pragma  comment(lib, "d3dcompiler.lib")

#include  "Common_Defs.h"

THIRD_PARTY_START

#include  <Windows.h>
#include  <D3D11.h>
#include  <D3DCompiler.h>

THIRD_PARTY_END


constexpr uint16  Width  = 640;
constexpr uint16  Height = 480;


HWND                     Window;

D3D_FEATURE_LEVEL        Feature_Level;

IDXGISwapChain          *Swap_Chain         = nullptr;
ID3D11Device            *Device             = nullptr;
ID3D11DeviceContext     *Immediate_Context  = nullptr;
ID3D11RenderTargetView  *Render_Target_View = nullptr;
ID3D11InputLayout       *Input_Layout       = nullptr;
ID3D11Buffer            *Vertex_Buffer      = nullptr;

ID3DBlob                *VS_Blob            = nullptr;
ID3DBlob                *PS_Blob            = nullptr;
ID3D11VertexShader      *Vertex_Shader      = nullptr;
ID3D11PixelShader       *Pixel_Shader       = nullptr;


struct  Vertex_Data
{
    float  X;
    float  Y;
    float  Z;
};

const Vertex_Data  Vertices[] =
{
    {  0.0f,  0.5f,  0.0f },
    {  0.5f, -0.5f,  0.0f },
    { -0.5f, -0.5f,  0.0f }
};


// =============================================================================
//
// Safe_Release
//
// =============================================================================

template<class  T>
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
// D3D11_Init
//
// =============================================================================

bool  D3D11_Init()
{
    const D3D_FEATURE_LEVEL  Feature_Levels[] = 
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    DXGI_SWAP_CHAIN_DESC  SD = {};

    SD.BufferDesc.Width                     = Width;
    SD.BufferDesc.Height                    = Height;
    SD.BufferDesc.RefreshRate.Numerator     = 0;
    SD.BufferDesc.RefreshRate.Denominator   = 1;
    SD.BufferDesc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
    SD.SampleDesc.Count                     = 1;
    SD.SampleDesc.Quality                   = 0;
    SD.BufferUsage                          = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SD.BufferCount                          = 2;
    SD.OutputWindow                         = Window;
    SD.Windowed                             = true;
    SD.SwapEffect                           = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    SD.Flags                                = 0;

    HRESULT  HR =  D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG,
        &Feature_Levels[0],
        1,
        D3D11_SDK_VERSION,
        &SD,
        &Swap_Chain,
        &Device,
        &Feature_Level,
        &Immediate_Context);

    if ( HR == E_INVALIDARG )
    {
        HR =  D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            NULL,
            D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG,
            &Feature_Levels[1],
            COUNT_OF(Feature_Levels) - 1,
            D3D11_SDK_VERSION,
            &SD,
            &Swap_Chain,
            &Device,
            &Feature_Level,
            &Immediate_Context);
    }

    if ( FAILED(HR) )
    {
        return false;
    }

    ID3D11Texture2D  *Frame_Buffer;

    HR = Swap_Chain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        (void**)&Frame_Buffer);

    if ( FAILED(HR) )
    {
        return false;
    }

    HR = Device->CreateRenderTargetView(Frame_Buffer, 0, &Render_Target_View);

    Frame_Buffer->Release();

    if ( FAILED(HR) )
    {
        return false;
    }

#if  defined(DEBUG) || defined(_DEBUG)
    UINT  Flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#else
    UINT  Flags = D3DCOMPILE_ENABLE_STRICTNESS;
#endif

    ID3DBlob  *Error_Blob = nullptr;

    HR = D3DCompileFromFile(
        L"Shaders.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "Vertex_Shader",
        "vs_5_0",
        Flags,
        0,
        &VS_Blob,
        &Error_Blob);

    if ( FAILED(HR) )
    {
        if ( Error_Blob != nullptr )
        {
            OutputDebugStringA(
                reinterpret_cast<char*>(Error_Blob->GetBufferPointer()) );

            Safe_Release(Error_Blob);
        }

        Safe_Release(VS_Blob);
        return false;
    }

    HR = D3DCompileFromFile(
        L"Shaders.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "Pixel_Shader",
        "ps_5_0",
        Flags,
        0,
        &PS_Blob,
        &Error_Blob);

    if ( FAILED(HR) )
    {
        if ( Error_Blob != nullptr )
        {
            OutputDebugStringA(
                reinterpret_cast<char*>(Error_Blob->GetBufferPointer()) );

            Safe_Release(Error_Blob);
        }

        Safe_Release(PS_Blob);

        return false;
    }

    HR = Device->CreateVertexShader(
        VS_Blob->GetBufferPointer(),
        VS_Blob->GetBufferSize(),
        nullptr,
        &Vertex_Shader);

    if ( FAILED(HR) )
    {
        return false;
    }

    HR = Device->CreatePixelShader(
        PS_Blob->GetBufferPointer(),
        PS_Blob->GetBufferSize(),
        nullptr,
        &Pixel_Shader);

    if ( FAILED(HR) )
    {
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC  Input_Element_Desc[] =
    {
        {
            "POS",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            0,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };

    HR = Device->CreateInputLayout(
        Input_Element_Desc,
        COUNT_OF(Input_Element_Desc),
        VS_Blob->GetBufferPointer(),
        VS_Blob->GetBufferSize(),
        &Input_Layout);

    if ( FAILED(HR) )
    {
        return false;
    }

    D3D11_BUFFER_DESC  BD = {};
    BD.ByteWidth = sizeof(Vertices);
    BD.Usage     = D3D11_USAGE_DEFAULT;
    BD.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA  SR_Data = {};
    SR_Data.pSysMem = Vertices;

    HR = Device->CreateBuffer(
        &BD,
        &SR_Data,
        &Vertex_Buffer);

    if ( FAILED(HR) )
    {
        return false;
    }

    return true;
}


// =============================================================================
//
// D3D11_Done
//
// =============================================================================

void  D3D11_Done()
{
    Safe_Release(Vertex_Buffer);
    Safe_Release(Input_Layout);
    Safe_Release(Pixel_Shader);
    Safe_Release(Vertex_Shader);
    Safe_Release(PS_Blob);
    Safe_Release(VS_Blob);
    Safe_Release(Render_Target_View);
    Safe_Release(Swap_Chain);
    Safe_Release(Immediate_Context);
    Safe_Release(Device);
}


// =============================================================================
//
// D3D11_Draw
//
// =============================================================================

void  D3D11_Draw()
{
    float  Background_Color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Immediate_Context->ClearRenderTargetView(
      Render_Target_View,
      Background_Color);

    RECT  Win_Rect;

    GetClientRect(Window, &Win_Rect);

    D3D11_VIEWPORT  Viewport =
    {
        0.0f,                                                   // TopLeftX
        0.0f,                                                   // TopLeftY
        static_cast<float>(Win_Rect.right  - Win_Rect.left),    // Width
        static_cast<float>(Win_Rect.bottom - Win_Rect.top),     // Height
        0.0f,                                                   // MinDepth
        1.0f                                                    // MaxDepth
    };

    Immediate_Context->RSSetViewports(1, &Viewport);

    Immediate_Context->OMSetRenderTargets(1, &Render_Target_View, nullptr);

    Immediate_Context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Immediate_Context->IASetInputLayout(Input_Layout);

    uint32  Vertex_Stride = sizeof(Vertices[0]);
    uint32  Vertex_Offset = 0;

    Immediate_Context->IASetVertexBuffers(
        0,                          // StartSlot
        1,                          // NumBuffers
        &Vertex_Buffer,             // ppVertexBuffers
        &Vertex_Stride,             // pStrides
        &Vertex_Offset);            // pOffsets

    Immediate_Context->VSSetShader(Vertex_Shader, nullptr, 0);
    Immediate_Context->PSSetShader(Pixel_Shader, nullptr, 0);

    Immediate_Context->Draw(COUNT_OF(Vertices), 0);

    Swap_Chain->Present(1, 0);
}


// =============================================================================
//
// WindowProc
//
// =============================================================================

LRESULT CALLBACK  WindowProc(
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
    switch ( uMsg )
    {
        case  WM_CLOSE:
        {
            DestroyWindow(hWnd);
            return 0;
        }

        case  WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        case  WM_PAINT:
        {
            D3D11_Draw();
            return 0;
        }
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
    HINSTANCE,
    PWSTR,
    int        nCmdShow)
{
    const wchar_t  CLASS_NAME[]  = L"Sample Window Class";

    WNDCLASSW  wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    Window = CreateWindowExW(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        L"D3D11 Triangle",              // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        10,                             // X
        10,                             // Y
        Width,                          // nWidth
        Height,                         // nHeight
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        nullptr);                       // Additional application data

    if ( Window == NULL )
    {
        return 0;
    }

    ShowWindow(Window, nCmdShow);

    if ( D3D11_Init() )
    {
        MSG  msg = { };

        while ( GetMessageW(&msg, NULL, 0, 0) )
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    D3D11_Done();

    return 0;
}
