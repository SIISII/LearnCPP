#include  "Common_Defs.h"
#include  "Com_Ptr.h"

THIRD_PARTY_START

#include  <cassert>
#include  <cstdio>

#include  <Windows.h>

#include  <DXGI1_6.h>

THIRD_PARTY_END


FILE  *F;

Com_Ptr<IDXGIFactory7>  g_pDXGI_Factory;

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

    do
    {
        Com_Ptr<IDXGIAdapter4>  pDXGI_Adapter;

        // Получение адаптера с номером N. Если его нет, в HR будет помещён код
        // ошибки, иначе - код успеха S_OK.
        HR = g_pDXGI_Factory->EnumAdapterByGpuPreference(
            N,                                      // Порядковый номер адаптера
            DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,   // Критерий выбора адаптера
            IID_PPV_ARGS(pDXGI_Adapter.Get_Addr()) );     // Объект адаптера

        // Если адаптер успешно получен, запись его имени в файл и освобождение
        // самого адаптера.
        if ( SUCCEEDED(HR) )
        {
            DXGI_ADAPTER_DESC3  Desc;

            pDXGI_Adapter->GetDesc3(&Desc);

            fwprintf_s(F, L"%s\n", Desc.Description);

            HRESULT  HR2;
            uint32   M = 0;

            do
            {
                Com_Ptr<IDXGIOutput>  pDXGI_Output;

                HR2 = pDXGI_Adapter->EnumOutputs(M, pDXGI_Output.Get_Addr());

                if ( SUCCEEDED(HR2) )
                {
                    fwprintf_s(F, L"  Output %i\n", M);

                    Com_Ptr<IDXGIOutput1>  pDXGI_Output1;

                    HRESULT  HR3 = pDXGI_Output->QueryInterface(pDXGI_Output1.Get_Addr());

                    if ( SUCCEEDED(HR3) )
                    {
                        uint32  Num;

                        HR3 = pDXGI_Output1->GetDisplayModeList1(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &Num, nullptr);

                        DXGI_MODE_DESC1  *Ptr = new DXGI_MODE_DESC1[Num];

                        HR3 = pDXGI_Output1->GetDisplayModeList1(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &Num, Ptr);

                        for ( uint32  I = 0; I < Num; ++I )
                        {
                            fwprintf_s(F, L"    %i x %i\n", Ptr[I].Width, Ptr[I].Height);
                        }

                        delete [] Ptr;
                    }
                }

                ++M;
            } while ( SUCCEEDED(HR2) );
        }

        ++N;
    } while ( SUCCEEDED(HR) );

    fclose(F);
}
