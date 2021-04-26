#include "Common_Defs.h"

THIRD_PARTY_START

#include  <cstdio>
#include  <exception>

#include  <Windows.h>

#include  <DXGI1_6.h>

THIRD_PARTY_END


template<class  T>
class  Com_Ptr
{
private:

    T  *Ptr = nullptr;


    void  Internal_Release()
    {
        if ( Ptr != nullptr )
        {
            Ptr->Release();
            Ptr = nullptr;
        }
    }


    void  Internal_Add_Ref()
    {
        if ( Ptr != nullptr )
        {
            Ptr->AddRef();
        }
    }


    void  Internal_Copy(T  *Other) noexcept
    {
        if ( Ptr != Other )
        {
            Internal_Release();
            Ptr = Other;
            Internal_Add_Ref();
        }
    }


    void  Internal_Move(Com_Ptr  &Other) noexcept
    {
        if ( Ptr != Other.Ptr )
        {
            Ptr = Other.Ptr;
            Internal_Add_Ref();
        }

        Other.Internal_Release();
    }

public:

    Com_Ptr() noexcept = default;


    ~Com_Ptr()
    {
        Internal_Release();
    }


    void**  Get_Addr() const
    {
        if ( Ptr != nullptr )
        {
            throw std::exception();
        }

        return &Ptr;
    }


    T*  operator->() const noexcept
    {
        return Ptr;
    }


    Com_Ptr&  operator = (Com_Ptr const  &Other) noexcept
    {
        Internal_Copy(Other.Ptr);
        return *this;
    }

    Com_Ptr&  operator = (T  *Other) noexcept
    {
        Internal_Copy(Other);
        return *this;
    }

    Com_Ptr&  operator = (Com_Ptr  &&Other) noexcept
    {
        Internal_Move(Other);
        return *this;
    }
};


FILE  *F;

Com_Ptr<IDXGIFactory7>  g_pDXGI_Factory;
Com_Ptr<IDXGIAdapter4>  g_pDXGI_Adapter;


int WINAPI  wWinMain(
    HINSTANCE,
    HINSTANCE,
    PWSTR,
    int)
{
    // �������� ������� DXGI, � ������� ������� ��������� ��������� ������� ���
    // ������ � DirectX.
    CreateDXGIFactory2(0, IID_IDXGIFactory2, g_pDXGI_Factory.Get_Addr());

    fopen_s(&F, "Adapters.txt", "w+");

    uint32   N = 0;
    HRESULT  HR;

    do
    {
        // ��������� �������� � ������� N. ���� ��� ���, � HR ����� ������� ���
        // ������, ����� - ��� ������ S_OK.
        HR = g_pDXGI_Factory->EnumAdapterByGpuPreference(
            N,                                      // ���������� ����� ��������
            DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,   // �������� ������ ��������
            IID_IDXGIAdapter4,
            g_pDXGI_Adapter.Get_Addr());       // ������ ��������

        // ���� ������� ������� �������, ������ ��� ����� � ���� � ������������
        // ������ ��������.
        if ( SUCCEEDED(HR) )
        {
            DXGI_ADAPTER_DESC3  Desc;

            g_pDXGI_Adapter->GetDesc3(&Desc);

            fwprintf_s(F, L"%s\n", Desc.Description);

            g_pDXGI_Adapter->Release();
        }

        ++N;
    } while ( SUCCEEDED(HR) );
    
    fclose(F);

    g_pDXGI_Factory->Release();
}
