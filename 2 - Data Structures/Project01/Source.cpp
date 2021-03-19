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
    int32   Value;
};


PItem  First = nullptr;


void  Add_First_Item(int32  X)
{
    PItem  P = new Item;

    P->Value = X;

    P->Next = First;
    First = P;
}


void  Add_Last_Item(int32  X)
{
    PItem  P = new Item;

    P->Value = X;

    P->Next = nullptr;

    if ( First == nullptr )
    {
        First = P;
    }
    else
    {
        PItem  S =  First;

        while ( S->Next != nullptr )
        {
            S = S->Next;
        }
        S->Next = P;
    }
}


void  Delete_First_Item()
{
    PItem  P = First;

    First = P->Next;

    delete P;
}


void  Delete_Last_Item()
{
    PItem  S = nullptr;
    PItem  P = First;

    while ( P->Next != nullptr )
    {
        S = P;
        P = P->Next;
    }

    if ( S == nullptr )
    {
        First = nullptr;
    }
    else
    {
        S->Next = nullptr;
    }

    delete P;
}


void  Delete_Item(PItem  I)
{
    PItem  S = nullptr;
    PItem  P = First;

    while ( P != I )
    {
        S = P;
        P = P->Next;
    }

    if ( S == nullptr )
    {
        First = nullptr;
    }
    else
    {
        S->Next = nullptr;
    }

    delete I;
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

        Add_Last_Item(V);
    }

    Delete_First_Item();
    Delete_Last_Item();

    PItem  P = First;

    while ( P != nullptr )
    {
        printf("\n%i", P->Value);
        P = P->Next;
    }


    while ( First != nullptr )
    {
        PItem  P2 = First->Next;
        delete First;
        First = P2;
    }

    First = nullptr;
}
