#include  "Common_Defs.h"
#include  "Com_Ptr.h"

THIRD_PARTY_START

#include  <cassert>
#include  <cstdio>

#include  <Windows.h>

#include  <DXGI1_6.h>

THIRD_PARTY_END

HRESULT GetDesc1(
    DXGI_OUTPUT_DESC1* pDesc
);

FILE  *F;

Com_Ptr<IDXGIFactory7>  g_pDXGI_Factory;
Com_Ptr<IDXGIAdapter4>  g_pDXGI_Adapter;


int WINAPI  wWinMain(
    HINSTANCE,
    HINSTANCE,
    PWSTR,
    int)
{
    // Создание фабрики DXGI, с помощью которой создаются различные объекты для
    // работы с DirectX.
    CreateDXGIFactory2(0, IID_PPV_ARGS(g_pDXGI_Factory.Get_Addr()) );

    fopen_s(&F, "Adapters.txt", "w+");

    uint32   N = 0;
    HRESULT  HR;
    HRESULT MN;

    do
    {


        // Получение адаптера с номером N. Если его нет, в HR будет помещён код
        // ошибки, иначе - код успеха S_OK.
        HR = g_pDXGI_Factory->EnumAdapterByGpuPreference(
            N,                                      // Порядковый номер адаптера
            DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,   // Критерий выбора адаптера
            IID_PPV_ARGS(g_pDXGI_Adapter.Get_Addr()) );     // Объект адаптера

        // Если адаптер успешно получен, запись его имени в файл и освобождение
        // самого адаптера.
        if ( SUCCEEDED(HR) )
        {
            DXGI_ADAPTER_DESC3  Desc;

            g_pDXGI_Adapter->GetDesc3(&Desc);

            fwprintf_s(F, L"%s\n", Desc.Description);

            g_pDXGI_Adapter = nullptr;
        }

        ++N;
    } while ( SUCCEEDED(HR) );

    fclose(F);
}
