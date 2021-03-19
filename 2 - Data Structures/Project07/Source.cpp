#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <cstring>


using  uint32  = std::uint32_t;


struct  My_Struct
{
    uint32  X;
    uint32  Y;
};


bool  operator > (const My_Struct  &A, const My_Struct  &B)
{
    if ( A.X > B.X )
    {
        return true;
    }
    else if ( A.X < B.X )
    {
        return false;
    }
    else if ( A.Y > B.Y )
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool  operator < (const My_Struct  &A, const My_Struct  &B)
{
    if ( A.X < B.X )
    {
        return true;
    }
    else if ( A.X > B.X )
    {
        return false;
    }
    else if ( A.Y < B.Y )
    {
        return true;
    }
    else
    {
        return false;
    }
}


template <typename  T>
class  Vector
{
public:

    using  value_type  = T;

private:

    using  Ptr_T  = T *;

    uint32  Max_Count;
    uint32  Cur_Count;

    Ptr_T   Storage;

public:

    Vector();

    void  reserve(uint32  N);

    uint32  size();

    T&  operator [] (uint32  I);

    void  push_back(const T  &Value);
};


using  Vec  = Vector<My_Struct>;


void  Sort(Vec  &V)
{
    bool   Changed;

    do
    {
        Changed = false;
        for ( uint32  I = 0; I <= V.size() - 2; ++I )
        {
            if ( V[I] > V[I + 1] )
            {
                Vec::value_type  Q = V[I];
                V[I] = V[I + 1];
                V[I + 1] = Q;
                Changed = true;
            }
        }
    } while ( Changed );
}


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    Vec    N;
    FILE  *F;

    N.reserve(10);

    fopen_s(&F, "File.txt", "r");

    while ( !feof(F) )
    {
        uint32  Q1, Q2;

        fscanf_s(F, "%i%i", &Q1, &Q2);

        N.push_back( {Q1, Q2} );
    }

    fclose(F);

    Sort(N);

    fopen_s(&F, "FileResult.txt", "w");

    for ( uint32  I = 0; I < N.size(); ++I )
    {
        fprintf(F, "%i\t%i\n", N[I].X, N[I].Y);
    }

    fclose(F);
}


template <typename  T>
Vector<T>::Vector()
    : Max_Count (0)
    , Cur_Count (0)
    , Storage   (nullptr)
{}


template <typename  T>
void  Vector<T>::reserve(uint32  N)
{
    if ( Max_Count == N )
    {
        return;
    }

    Ptr_T  P = new T[N];

    uint32  Cnt = N > Cur_Count ? Cur_Count : N;

    for ( uint32  I = 0; I < Cnt; ++I )
    {
        P[I] = Storage[I];
    }

    if ( Storage != nullptr )
    {
        delete [] Storage;
    }

    Storage   = P;
    Max_Count = N;
    Cur_Count = Cnt;
}


template <typename  T>
uint32  Vector<T>::size()
{
    return Cur_Count;
}


template <typename  T>
T&  Vector<T>::operator [] (uint32  I)
{
    return Storage[I];
}


template <typename  T>
void  Vector<T>::push_back(const T  &Value)
{
    if ( Cur_Count == Max_Count )
    {
        reserve(Max_Count + 1 + Max_Count / 2);
    }

    Storage[Cur_Count] = Value;
    ++Cur_Count;
}
