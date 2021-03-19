#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <cstring>


using  uint32  = std::uint32_t;
using   int32  = std::int32_t;



struct  Item;

using  PItem  = Item *;


struct  Item
{
    PItem   Next;
    PItem   Prev;
    int32   Value;
};


struct  Header
{
    PItem   First;
    PItem   Last;
};


Header  H = { nullptr, nullptr };


void  Add_First(int32  X)
{
    PItem  P;

    P = new Item;

    P->Value = X;

    P->Prev = nullptr;
    P->Next = H.First;

    if ( H.First != nullptr )
    {
        H.First->Prev = P;
    }
    else
    {
        H.Last = P;
    }

    H.First = P;
}


void  Add_Last(int32  X)
{
    PItem  P;

    P = new Item;

    P->Value = X;

    P->Next = nullptr;

    if ( H.Last == nullptr )
    {
        P->Prev = nullptr;
        H.First = P;
    }
    else
    {
        P->Prev = H.Last;
        H.Last->Next = P;
    }

    H.Last = P;
}


void  Delete_First()
{
    PItem  P;
    P = H.First;

    if ( P->Next == nullptr )
    {
        H.First = nullptr;
        H.Last = nullptr;
    }
    else
    {
        P->Next->Prev = nullptr;
        H.First = P->Next;
    }

    delete P;
}


void  Delete_Last()
{
    PItem  P;
    P = H.Last;

    if ( P->Prev == nullptr )
    {
        H.First = nullptr;
        H.Last = nullptr;
    }
    else
    {
        P->Prev->Next = nullptr;
        H.Last = P->Prev;
    }

    delete P;}


void  Delete_Item(PItem  P)
{
    if ( P->Prev == nullptr )
    {
        Delete_First();
    }
    else if ( P->Next == nullptr )
    {
        Delete_Last();
    }
    else
    {
        P->Prev->Next = P->Next;
        P->Next->Prev = P->Prev;
        delete P;
    }
}


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    while (true)
    {
        int32  V;

        printf_s("¬ведите число: ");
        scanf_s("%i", &V);

        if ( V == 0 )
        {
            break;
        }

        Add_Last(V);
    }

   Delete_Item(H.First->Next);

    PItem  P = H.First;

    while ( P != nullptr )
    {
        printf("\n%i", P->Value);
        P = P->Next;
    }


    while ( H.First != nullptr )
    {
        PItem  P2 = H.First->Next;
        delete H.First;
        H.First = P2;
    }

    H.First = nullptr;
    H.Last  = nullptr;
}
