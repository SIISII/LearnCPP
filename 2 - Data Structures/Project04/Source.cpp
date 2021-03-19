#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <cstring>


using  int32   = std::int32_t;
using  uint32  = std::uint32_t;


struct  Item;

using  PItem  = Item *;


struct  Item
{
    PItem   Upper;
    PItem   Left;
    PItem   Right;
    uint32  Left_Count;
    uint32  Right_Count;
    int32   Value;
};


PItem  H = nullptr;


// Удаление элемента R из содержащего его дерева с корнем Root, перебалансировка
// дерева и возврат его нового корня. Элемент R физически не уничтожается.
PItem  Remove_Item(
    PItem  Root,
    PItem  R);


// Добавление элемента New к дереву, чьим корнем является Root, перебалансировка
// этого дерева и возврат его нового корня.
PItem  Add_Item(
    PItem  Root,
    PItem  New)
{
    if ( New->Value < Root->Value )
    {
        if ( Root->Left == nullptr )
        {
            Root->Left = New;
            New->Upper = Root;
            Root->Left_Count = 1;
            return Root;
        }

        Root->Left = Add_Item(Root->Left, New);

        if ( Root->Left_Count <= Root->Right_Count )
        {
            Root->Left->Upper = Root;
            Root->Left_Count++;
            return Root;
        }

        PItem   Left = Root->Left;
        uint32  Left_Count = Root->Left_Count;

        Root->Left = nullptr;
        Root->Left_Count = 0;

        PItem   Right = Root->Right;
        uint32  Right_Count = Root->Right_Count;

        Root->Right = nullptr;
        Root->Right_Count = 0;

        PItem  New_Root = Left;
        while ( New_Root->Right != nullptr )
        {
            New_Root = New_Root->Right;
        }

        Left = Remove_Item(Left, New_Root);
        if ( Right == nullptr )
        {
            Right = Root;
        }
        else
        {
            Right = Add_Item(Right, Root);
        }

        New_Root->Left = Left;
        Left->Upper = New_Root;
        New_Root->Left_Count = Left_Count;

        New_Root->Right = Right;
        Right->Upper = New_Root;
        New_Root->Right_Count = Right_Count + 1;

        return New_Root;
    }
    else
    {
        if ( Root->Right == nullptr )
        {
            Root->Right = New;
            New->Upper = Root;
            Root->Right_Count = 1;
            return Root;
        }

        Root->Right = Add_Item(Root->Right, New);

        if ( Root->Right_Count <= Root->Left_Count )
        {
            Root->Right->Upper = Root;
            Root->Right_Count++;
            return Root;
        }

        PItem   Left = Root->Left;
        uint32  Left_Count = Root->Left_Count;

        Root->Left = nullptr;
        Root->Left_Count = 0;

        PItem   Right = Root->Right;
        uint32  Right_Count = Root->Right_Count;

        Root->Right = nullptr;
        Root->Right_Count = 0;

        PItem  New_Root = Right;
        while ( New_Root->Left != nullptr )
        {
            New_Root = New_Root->Left;
        }

        Right = Remove_Item(Right, New_Root);
        if ( Left == nullptr )
        {
            Left = Root;
        }
        else
        {
            Left = Add_Item(Left, Root);
        }

        New_Root->Right = Right;
        Right->Upper = New_Root;
        New_Root->Right_Count = Right_Count;

        New_Root->Left = Left;
        Left->Upper = New_Root;
        New_Root->Left_Count = Left_Count + 1;

        return New_Root;
    }
}


void  Add_Item(int32  Value)
{
    PItem  P = new Item;

    P->Left  = nullptr;
    P->Right = nullptr;
    P->Left_Count = 0;
    P->Right_Count = 0;
    P->Value = Value;

    if ( H == nullptr )
    {
        H = P;
    }
    else
    {
        H = Add_Item(H, P);
    }

    H->Upper = nullptr;
}


void  Delete_Item(PItem  P)
{
    H = Remove_Item(H, P);

    delete P;
}


PItem  Remove_Item(
    PItem  Root,
    PItem  R)
{
    if ( R == Root )
    {
        PItem   Left = Root->Left;
        uint32  Left_Count = Root->Left_Count;

        PItem   Right = Root->Right;
        uint32  Right_Count = Root->Right_Count;

        if ( Left == nullptr && Right == nullptr )
        {
            return nullptr;
        }

        if ( Left_Count >= Right_Count )
        {
            PItem  New_Root = Left;
            while ( New_Root->Right != nullptr )
            {
                New_Root = New_Root->Right;
            }

            Left = Remove_Item(Left, New_Root);

            New_Root->Left = Left;
            if ( Left != nullptr )
            {
                Left->Upper = New_Root;
            }
            New_Root->Left_Count = Left_Count - 1;

            New_Root->Right = Right;
            if ( Right != nullptr )
            {
                Right->Upper = New_Root;
            }
            New_Root->Right_Count = Right_Count;

            return New_Root;
        }
        else
        {
            PItem  New_Root = Right;
            while ( New_Root->Left != nullptr )
            {
                New_Root = New_Root->Left;
            }

            Right = Remove_Item(Right, New_Root);

            New_Root->Left = Left;
            if ( Left != nullptr )
            {
                Left->Upper = New_Root;
            }
            New_Root->Left_Count = Left_Count;

            New_Root->Right = Right;
            if ( Right != nullptr )
            {
                Right->Upper = New_Root;
            }
            New_Root->Right_Count = Right_Count - 1;

            return New_Root;
        }
    }
    else if ( R->Value < Root->Value )
    {
        Root->Left = Remove_Item(Root->Left, R);

        if ( Root->Left_Count >= Root->Right_Count )
        {
            if ( Root->Left != nullptr )
            {
                Root->Left->Upper = Root;
            }
            Root->Left_Count--;
            return Root;
        }

        PItem   Left = Root->Left;
        uint32  Left_Count = Root->Left_Count;

        Root->Left = nullptr;
        Root->Left_Count = 0;

        PItem   Right = Root->Right;
        uint32  Right_Count = Root->Right_Count;

        Root->Right = nullptr;
        Root->Right_Count = 0;

        PItem  New_Root = Right;
        while ( New_Root->Left != nullptr )
        {
            New_Root = New_Root->Left;
        }

        Right = Remove_Item(Right, New_Root);
        if ( Left == nullptr )
        {
            Left = Root;
        }
        else
        {
            Left = Add_Item(Left, Root);
        }

        New_Root->Right = Right;
        Right->Upper = New_Root;
        New_Root->Right_Count = Right_Count - 1;

        New_Root->Left = Left;
        Left->Upper = New_Root;
        New_Root->Left_Count = Left_Count;

        return New_Root;
    }
    else
    {
        Root->Right = Remove_Item(Root->Right, R);

        if ( Root->Right_Count >= Root->Left_Count )
        {
            if ( Root->Right != nullptr )
            {
                Root->Right->Upper = Root;
            }
            Root->Right_Count--;
            return Root;
        }

        PItem   Left = Root->Left;
        uint32  Left_Count = Root->Left_Count;

        Root->Left = nullptr;
        Root->Left_Count = 0;

        PItem   Right = Root->Right;
        uint32  Right_Count = Root->Right_Count;

        Root->Right = nullptr;
        Root->Right_Count = 0;

        PItem  New_Root = Left;
        while ( New_Root->Right != nullptr )
        {
            New_Root = New_Root->Right;
        }

        Left = Remove_Item(Left, New_Root);
        if ( Right == nullptr )
        {
            Right = Root;
        }
        else
        {
            Right = Add_Item(Right, Root);
        }

        New_Root->Right = Right;
        Right->Upper = New_Root;
        New_Root->Right_Count = Right_Count;

        New_Root->Left = Left;
        Left->Upper = New_Root;
        New_Root->Left_Count = Left_Count - 1;

        return New_Root;
    }
}


void  Print_Branch(PItem  B)
{
    if ( B->Left != nullptr )
    {
        Print_Branch(B->Left);
    }
    printf("\n%i", B->Value);
    if ( B->Right != nullptr )
    {
        Print_Branch(B->Right);
    }
}


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    int32  V;

    do
    {
        printf("Введите число: ");
        scanf_s("%i", &V);
        if ( V != 0 )
        {
            Add_Item(V);
        }
    } while ( V != 0);

    Delete_Item(H->Left);

    printf("\nРезультаты:");

    Print_Branch(H);
}
