#include "Common_Defs.h"

THIRD_PARTY_START

#include  <Windows.h>
#include  <cstdio>

#include  <DXGI1_6.h>

THIRD_PARTY_END

FILE *F;

IDXGIFactory7               *g_pDXGI_Factory    = nullptr;
IDXGIAdapter4               *g_pDXGI_Adapter    = nullptr;


int WINAPI  wWinMain(
    HINSTANCE,
    HINSTANCE,
    PWSTR,
    int)
{
    // Создание фабрики DXGI, с помощью которой создаются различные объекты для
    // работы с DirectX.
    CreateDXGIFactory2(0, IID_PPV_ARGS(&g_pDXGI_Factory) );

    fopen_s(&F, "Adapters.txt", "w+");

    uint32   N = 0;
    HRESULT  HR;

    do
    {
        HR = g_pDXGI_Factory->EnumAdapterByGpuPreference(
            N,                                      // Порядковый номер адаптера
            DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,   // Критерий выбора адаптера
            IID_PPV_ARGS(&g_pDXGI_Adapter) );       // Объект адаптера

        if ( SUCCEEDED(HR) )
        {
            DXGI_ADAPTER_DESC3  Desc;
            g_pDXGI_Adapter->GetDesc3(&Desc);
            g_pDXGI_Adapter->Release();

            fwprintf_s(F, L"%s\n", Desc.Description);
        }

        ++N;
    } while ( SUCCEEDED(HR) );
    
    fclose(F);

    g_pDXGI_Factory->Release();
}
