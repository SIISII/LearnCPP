#pragma once

#include  "Common_Defs.h"

THIRD_PARTY_START

#include  <cassert>

#include  <Unknwnbase.h>

THIRD_PARTY_END


// =============================================================================
//
// Com_Ptr class
//
// =============================================================================

template<class  T>
class  Com_Ptr
{
public:

    DISABLE_WARNING_NO_VIRTUAL_DESTRUCTOR

    // ��������������� �����, ����������� ��� ������������ ����������� �������
    // ������� � ������� AddRef � Release, ��������� �������� ->.
    template<class  T2>
    class Restrict_Interface : public T2
    {
        ULONG __stdcall  AddRef();
        ULONG __stdcall  Release();
    };

    ENABLE_WARNING_NO_VIRTUAL_DESTRUCTOR

private:

    // ����� ����� Com_Ptr<T2>, ��� T2 - ������������ ���, �������� ������ ���-
    // ���� ������ Com_Ptr<T>. ��� ��������� �������� ������� ������ ����������
    // � ��������� ������ ������ Com_Ptr<T2> (Com_Ptr<T> � Com_Ptr<T2> ���������
    // ������� ������, ���� T � T2 �� ���������, � ������ ���� �� ����� ����-
    // ������ � ��������� ��� ���������� ������ ���� �����, ���� ��� �� �������-
    // �� ��������).
    template <class  T2>
    friend class  Com_Ptr;

    // ������������ ���� ������ �������� �� ��������� �����������, ��� ������
    // ���� ������ ����� ����� ����������� ��������, ���� ���� � ������������
    // ��� ������ ����� ������� �� ������������� (� ����� ������ ����� ���������
    // ��������� � ���������� ���� �������� �� ��������� - � ������ ������ ���
    // nullptr).
    T  *Ptr = nullptr;


    // ���������� �������� ������ �� ���������, ��������� �� ������� ��������� �
    // ���� Ptr. ���� ��� ��������� nullptr, �� ������ ������ �� ����.
    inline void  Internal_Add_Ref() const noexcept
    {
        if ( Ptr != nullptr )
        {
            Ptr->AddRef();
        }
    }


    // ������������ ����������, �� ������� ��������� Ptr, � ��������� ������ ��-
    // �� Ptr. ����� Release ��������� ������� ������������� ����������, ��� ��-
    // ����� ��� �����������, ���� ������� ������ ����.
    inline void  Internal_Release() noexcept
    {
        // ����� �������� ���������� ������ ������������� �������� �����������
        // Ptr � ����������� Ptr->Release() � Ptr = nullptr �������� ��� �� ��-
        // �����������, �� ��������� ��������, ����� ����� Release ��������� ��-
        // ����� �������, � �������� ����� ���������� � ���������� ������ ������
        // �������. ��� ������������� ������� ��������� ����� ��������� ��, ���
        // Ptr �������� ��������� ��������� (���� Ptr = nullptr ����� Ptr->Rele-
        // ase ��� �� ��������!), � ����� �� ������ Release, �.�. ��������� ��
        // ������� ������������ ����������. � �������� �� ���������� ������� ��-
        // �������� Ptr � ���� ����� ���������� Release.
        T  *P = Ptr;

        if ( P != nullptr )
        {
            Ptr = nullptr;
            P->Release();
        }
    }


    // �������� ����� ��������� �� ���������. ������� ���������� ���������� ��-
    // �������, ��������� �� ������� �������� � Ptr � ������ ������, � ���� ��-
    // ��� ��������� Ptr ����� ��������. ����� ����, ����� ��������� �������
    // ������ �� ����� ���������, ��������� ����� "�����" ���������� �� ����
    // ����������� (������� ����������� ��� ����������� ����������).
    inline void  Internal_Copy(T  *Other) noexcept
    {
        if ( Ptr != Other )
        {
            Internal_Release();
            Ptr = Other;
            Internal_Add_Ref();
        }
    }


    // ����������� ��������� �� ���������. � ������� �� ����������� ������, ��
    // ������ �������� ������ �� � ������ �����������, � � �������� Com_Ptr, ��-
    // ��������� ��������� ������ ����. ������� ��� ����������� ����������� ��-
    // ��������� ����������� ���������� �� ������ ������������ ��������� �������
    // ������ �������.
    template<class  T2>
    inline void  Internal_Move(Com_Ptr<T2>  &Other) noexcept
    {
        if ( Ptr != Other.Ptr )
        {
            Internal_Release();
            Ptr = Other.Ptr;
            Other.Ptr = nullptr;
        }
    }

public:

    // ����������� �� ���������: ������ �� ������, �� ������ ����������������
    // ��������� ���� Ptr (��� �������� ��� ���������� ���� ������� ��������
    // �� ��������� nullptr).
    inline  Com_Ptr() noexcept = default;


    // ����������� �����������: �������� ��������� �� ��������� �� ������� ��-
    // �������� ������ Com_Ptr<T> � ����������� ��������� ����� ������ � �����-
    // ������ ������� ������ �� ��������� (��������� �� ����������� ���������
    // �� ����).
    inline  Com_Ptr(Com_Ptr const  &Other) noexcept
        : Ptr (Other.Ptr)
    {
        Internal_Add_Ref();
    }


    // ��������� ����������� �����������: �� ��������� ����������� ��������� ��
    // ��������� T2 �� ���������� ������ Com_Ptr<T2> � ����������� ���������
    // ������ Com_Ptr<T>, ���� T2 � T ���������� (�.�. ���� T2 ��������� � T ���
    // ���� T2 �������� ����������� T). ��������, ����� ����������� ��������� ��
    // IDXGIFactory2 � ��������� �� IDXGIFactory, ��������� ������ ��������� ��-
    // ������ ����������� �������, � �������� ����������� ������.
    template<class  T2>
    inline  Com_Ptr(Com_Ptr<T2> const  *Other) noexcept
        : Ptr (Other.Ptr)
    {
        Internal_Add_Ref();
    }


    // ����������� �����������: ��������� ������� ����� ��������� Com_Ptr<T>,
    // ���������� � ���� �������� ��������� �� ������� ���������� ����� �� ����-
    // ��, ����� ���� �������� ��������� � ����������-���������. ����� ������
    // �����������, ���� ���������-�������� ����� ����� ������������, �.�. ����
    // ��������� ���� �� ��������� ��� ��������, � ����������� ��� ����������
    // ������ ������ ���������� (��������� ����� ������).
    inline  Com_Ptr(Com_Ptr  &&Other) noexcept
        : Ptr (Other.Ptr)
    {
        Other.Ptr = nullptr;
    }


    // ��������� ������ ������������ �����������.
    template<class  T2>
    inline  Com_Ptr(Com_Ptr<T2> const  &&Other) noexcept
        : Ptr (Other.Ptr)
    {
        Other.Ptr = nullptr;
    }


    // ���������� ����������� ���������, ��������� �� ������� �������� � ������
    // ���������� ������.
    inline  ~Com_Ptr()
    {
        Internal_Release();
    }


    // �������� ������������ ��������� nullptr.
    inline Com_Ptr&  operator = (std::nullptr_t) noexcept
    {
        Internal_Copy(nullptr);
        return *this;
    }


    // �������� ������������ ������������ ������ ���������.
    template<class  T2>
    inline Com_Ptr&  operator = (T2  *Other) noexcept
    {
        Internal_Copy(Other);
        return *this;
    }


    // �������� ������������ ��������, ������� �� ������� ���������� Com_Ptr.
    inline Com_Ptr&  operator = (Com_Ptr const  &Other) noexcept
    {
        Internal_Copy(Other.Ptr);
        return *this;
    }


    // �������� ������������ ����������� ��������.
    template<class  T2>
    inline Com_Ptr&  operator = (Com_Ptr<T2> const  &Other) noexcept
    {
        Internal_Copy(Other.Ptr);
        return *this;
    }


    // �������� ��������� ������������ ��������� �� ���������.
    template<class  T2>
    inline Com_Ptr&  operator = (Com_Ptr<T2>  &&Other) noexcept
    {
        Internal_Move(Other);
        return *this;
    }


    // �������� ��������� � ����� ����������.
    inline Restrict_Interface<T>*  operator->() const noexcept
    {
        return static_cast<Restrict_Interface<T> *>(Ptr);
    }


    // �������� ���������.
    inline bool  operator == (std::nullptr_t) const noexcept
    {
        return Ptr == nullptr;
    }


    template<class  T2>
    inline bool  operator == (Com_Ptr<T2> const  &Other) const noexcept
    {
        return Ptr == Other.Ptr;
    }


    template<class  T2>
    inline bool  operator == (T2 const  *Other) const noexcept
    {
        return Ptr == Other;
    }


    inline bool  operator != (std::nullptr_t) const noexcept
    {
        return Ptr != nullptr;
    }


    template<class  T2>
    inline bool  operator != (Com_Ptr<T2> const  &Other) const noexcept
    {
        return Ptr != Other.Ptr;
    }


    template<class  T2>
    inline bool  operator != (T2 const  *Other) const noexcept
    {
        return Ptr != Other;
    }


    // ����� ������� ���������� �� ���������� ����������.
    inline void  Swap(Com_Ptr  &Other) noexcept
    {
        T  *P = Ptr;
        Ptr = Other.Ptr;
        Other.Ptr = P;
    }


    // ��������� ��������� ���������.
    inline T*  Get() const noexcept
    {
        return Ptr;
    }


    // ��������� ������ ������ ���� Ptr, ��� ���������� ��� ���������� � ��
    // ������ �� ���������, ������������ ������� ��������� Windows. ���������
    // ������� ���������� Ptr ����� ���� �������� � �� �� ����� ��� ��������
    // ������� ����������, ���������� ��������� ����� �����������.
    inline T**  Get_Addr() noexcept
    {
        assert(Ptr == nullptr);
        return &Ptr;
    }
};


// ��� ������� ��������� ������������� ����������� ������ ���������� ����������
// �� ���������� � ����������� �����������.
template<class  T>
static inline void  swap(
    Com_Ptr<T>  &Left, 
    Com_Ptr<T>  &Right) noexcept
{
    Left.Swap(Right);
}
