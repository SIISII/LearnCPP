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

    // Вспомогательный класс, необходимый для блокирования возможности прямого
    // доступа к методам AddRef и Release, используя операцию ->.
    template<class  T2>
    class Restrict_Interface : public T2
    {
        ULONG __stdcall  AddRef();
        ULONG __stdcall  Release();
    };

    ENABLE_WARNING_NO_VIRTUAL_DESTRUCTOR

private:

    // Любой класс Com_Ptr<T2>, где T2 - произвольный тип, является другом дан-
    // ного класса Com_Ptr<T>. Это позволяет функциям данного класса обращаться
    // к приватным членам класса Com_Ptr<T2> (Com_Ptr<T> и Com_Ptr<T2> считаются
    // разными типами, если T и T2 не совпадают, а разные типы не могут обра-
    // щаться к приватным или защищённым членам друг друга, если они не объявле-
    // ны друзьями).
    template <class  T2>
    friend class  Com_Ptr;

    // Присваивание полю класса значения по умолчанию гарантирует, что данное
    // поле всегда будет иметь определённое значение, даже если в конструкторе
    // ему ничего явным образом не присваивается (в таком случае будет присвоено
    // указанное в объявлении поля значение по умолчанию - в данном случае это
    // nullptr).
    T  *Ptr = nullptr;


    // Увеличение счётчика ссылок на интерфейс, указатель на который находится в
    // поле Ptr. Если там находится nullptr, то ничего делать не надо.
    inline void  Internal_Add_Ref() const noexcept
    {
        if ( Ptr != nullptr )
        {
            Ptr->AddRef();
        }
    }


    // Освобождение интерфейса, на который указывает Ptr, и обнуление самого по-
    // ля Ptr. Вызов Release уменьшает счётчик использования интерфейса, что вы-
    // зовет его уничтожение, если счётчик достиг нуля.
    inline void  Internal_Release() noexcept
    {
        // Такая странная реализация вместо прямолинейной проверки содержимого
        // Ptr с последующим Ptr->Release() и Ptr = nullptr защищает нас от ма-
        // ловероятной, но возможной ситуации, когда вызов Release порождает це-
        // почку событий, в конечном итоге приводящих к повторному вызову данной
        // функции. При прямолинейном подходе повторный вызов обнаружил бы, что
        // Ptr содержит ненулевой указатель (ведь Ptr = nullptr после Ptr->Rele-
        // ase ещё не выполнен!), и вновь бы вызвал Release, т.е. произошло бы
        // двойное освобождение интерфейса. В нынешней же реализации сначала об-
        // нуляется Ptr и лишь затем вызывается Release.
        T  *P = Ptr;

        if ( P != nullptr )
        {
            Ptr = nullptr;
            P->Release();
        }
    }


    // Создание копии указателя на интерфейс. Сначала необходимо освободить ин-
    // терфейс, указатель на который хранится в Ptr в данный момент, и лишь по-
    // том присвоить Ptr новое значение. Кроме того, нужно увеличить счётчик
    // ссылок на новый интерфейс, поскольку число "живых" указателей на него
    // увеличилось (счётчик уменьшается при уничтожении указателей).
    inline void  Internal_Copy(T  *Other) noexcept
    {
        if ( Ptr != Other )
        {
            Internal_Release();
            Ptr = Other;
            Internal_Add_Ref();
        }
    }


    // Перемещение указателя на интерфейс. В отличие от предыдущего случая, мы
    // должны начинать работу не с самими указателями, а с классами Com_Ptr, со-
    // держащими указатели внутри себя. Поэтому для обеспечения возможности пе-
    // ремещения совместимых указателей мы должны использовать шаблонную функцию
    // вместо обычной.
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

    // Конструктор по умолчанию: ничего не делает, не считая гарантированного
    // обнуления поля Ptr (для которого при объявлении было указано значение
    // по умолчанию nullptr).
    inline  Com_Ptr() noexcept = default;


    // Конструктор копирования: копирует указатель на интерфейс из другого эк-
    // земпляра класса Com_Ptr<T> в создаваемый экземпляр этого класса и увели-
    // чивает счётчик ссылок на интерфейс (поскольку мы скопировали указатель
    // на него).
    inline  Com_Ptr(Com_Ptr const  &Other) noexcept
        : Ptr (Other.Ptr)
    {
        Internal_Add_Ref();
    }


    // Шаблонный конструктор копирования: он позволяет скопировать указатель на
    // интерфейс T2 из экземпляра класса Com_Ptr<T2> в создаваемый экземпляр
    // класса Com_Ptr<T>, если T2 и T совместимы (т.е. если T2 совпадает с T или
    // если T2 является наследником T). Например, можно скопировать указатель на
    // IDXGIFactory2 в указатель на IDXGIFactory, поскольку первый интерфейс яв-
    // ляется наследником второго, а наоборот скопировать нельзя.
    template<class  T2>
    inline  Com_Ptr(Com_Ptr<T2> const  *Other) noexcept
        : Ptr (Other.Ptr)
    {
        Internal_Add_Ref();
    }


    // Конструктор перемещения: позволяет создать новый экземпляр Com_Ptr<T>,
    // скопировав в него значение указателя из другого экземпляра этого же клас-
    // са, после чего обнулить указатель в экземпляре-источнике. Такой способ
    // применяется, если экземпляр-источник после этого уничтожается, т.е. если
    // логически надо не создавать его дубликат, а переместить его содержимое
    // внутрь нового экземпляра (уничтожив затем старый).
    inline  Com_Ptr(Com_Ptr  &&Other) noexcept
        : Ptr (Other.Ptr)
    {
        Other.Ptr = nullptr;
    }


    // Шаблонная версия конструктора перемещения.
    template<class  T2>
    inline  Com_Ptr(Com_Ptr<T2> const  &&Other) noexcept
        : Ptr (Other.Ptr)
    {
        Other.Ptr = nullptr;
    }


    // Деструктор освобождает интерфейс, указатель на который хранится в данном
    // экземпляре класса.
    inline  ~Com_Ptr()
    {
        Internal_Release();
    }


    // Операция присваивания указателя nullptr.
    inline Com_Ptr&  operator = (std::nullptr_t) noexcept
    {
        Internal_Copy(nullptr);
        return *this;
    }


    // Операция присваивания совместимого сырого указателя.
    template<class  T2>
    inline Com_Ptr&  operator = (T2  *Other) noexcept
    {
        Internal_Copy(Other);
        return *this;
    }


    // Операция присваивания значения, взятого из другого экземпляра Com_Ptr.
    inline Com_Ptr&  operator = (Com_Ptr const  &Other) noexcept
    {
        Internal_Copy(Other.Ptr);
        return *this;
    }


    // Операция присваивания совместимых значений.
    template<class  T2>
    inline Com_Ptr&  operator = (Com_Ptr<T2> const  &Other) noexcept
    {
        Internal_Copy(Other.Ptr);
        return *this;
    }


    // Операция пересылки совместимого указателя на интерфейс.
    template<class  T2>
    inline Com_Ptr&  operator = (Com_Ptr<T2>  &&Other) noexcept
    {
        Internal_Move(Other);
        return *this;
    }


    // Операция обращения к члену интерфейса.
    inline Restrict_Interface<T>*  operator->() const noexcept
    {
        return static_cast<Restrict_Interface<T> *>(Ptr);
    }


    // Операции сравнения.
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


    // Обмен местами указателей на идентичные интерфейсы.
    inline void  Swap(Com_Ptr  &Other) noexcept
    {
        T  *P = Ptr;
        Ptr = Other.Ptr;
        Other.Ptr = P;
    }


    // Получение хранимого указателя.
    inline T*  Get() const noexcept
    {
        return Ptr;
    }


    // Получение адреса самого поля Ptr, что необходимо для сохранения в нём
    // ссылки на интерфейс, возвращаемой многими функциями Windows. Поскольку
    // текущее содержимое Ptr может быть изменено и мы не можем это разумным
    // образом обработать, необходимо запретить такую возможность.
    inline T**  Get_Addr() noexcept
    {
        assert(Ptr == nullptr);
        return &Ptr;
    }
};


// Эта функция обеспечит использование возможности обмена значениями указателей
// на интерфейсы в стандартных контейнерах.
template<class  T>
static inline void  swap(
    Com_Ptr<T>  &Left, 
    Com_Ptr<T>  &Right) noexcept
{
    Left.Swap(Right);
}
