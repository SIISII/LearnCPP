#include  "Common_Defs.h"
#include  "Com_Ptr.h"

THIRD_PARTY_START

#include  <cassert>
#include  <cstdio>
#include  <clocale>

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
    std::setlocale(LC_ALL, "Russian");

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

        // Если адаптер успешно получен, запись его имени в файл, а затем пере-
        // числение его выходов. Для каждого выхода получается и выводится в
        // файл список поддерживаемых режимов вывода.
        //
        // Освобождать интерфейсы явным образом не требуется, поскольку для них
        // применяются умные указатели (Com_Ptr): они сами знают, когда и как
        // освобождать интерфейсы.
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

                        const uint32  Flags = DXGI_ENUM_MODES_INTERLACED    |
                                              DXGI_ENUM_MODES_SCALING       |
                                              DXGI_ENUM_MODES_STEREO        |
                                              DXGI_ENUM_MODES_DISABLED_STEREO;

                        // Определение количества видеорежимов.
                        HR3 = pDXGI_Output1->GetDisplayModeList1(
                            DXGI_FORMAT_R8G8B8A8_UNORM,
                            Flags,
                            &Num,
                            nullptr);

                        // Выделение памяти под массив описателей видеорежимов.
                        DXGI_MODE_DESC1  *Ptr = new DXGI_MODE_DESC1[Num];

                        // Получение описателей видеорежимов.
                        HR3 = pDXGI_Output1->GetDisplayModeList1(
                            DXGI_FORMAT_R8G8B8A8_UNORM,
                            Flags,
                            &Num,
                            Ptr);

                        // Вывод информации о видеорежимах.
                        for ( uint32  I = 0; I < Num; ++I )
                        {
                            fwprintf_s(F, L"    %i x %i ", Ptr[I].Width, Ptr[I].Height);

                            if ( Ptr[I].RefreshRate.Numerator % Ptr[I].RefreshRate.Denominator == 0 )
                            {
                                fwprintf_s(F, L"@ %i Hz ", Ptr[I].RefreshRate.Numerator / Ptr[I].RefreshRate.Denominator);
                            }
                            else
                            {
                                float  Res = static_cast<float>(Ptr[I].RefreshRate.Numerator) / static_cast<float>(Ptr[I].RefreshRate.Denominator);

                                fwprintf_s(F, L"@ %7.3f Hz ", Res);
                            }

                            fwprintf_s(F, L"R8G8B8A8 ");

                            if ( Ptr[I].ScanlineOrdering !=  DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE )
                            {
                                fwprintf_s(F, L"чересстрочно ");
                            }

                            switch ( Ptr[I].Scaling )
                            {
                                case DXGI_MODE_SCALING_CENTERED:
                                    fwprintf_s(F, L"центрирование ");
                                    break;

                                case DXGI_MODE_SCALING_STRETCHED:
                                    fwprintf_s(F, L"растяжение ");
                                    break;
                                
                                case DXGI_MODE_SCALING_UNSPECIFIED:
                                    break;
                            }

                            if ( Ptr[I].Stereo )
                            {
                                 fwprintf_s(F, L"Стерео");
                            }

                            fwprintf_s(F, L"\n");
                        }

                        // Освобождение памяти, выделенной под описатели видео-
                        // режимов. Поскольку используется обычный, а не умный
                        // указатель, освобождение нужно делать явным образом.
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
