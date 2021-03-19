#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <cstring>


using  uint32  = std::uint32_t;
using   int32  = std::int32_t;


constexpr uint32  Qty = 10;

int  main()
{
    std::setlocale(LC_ALL, "Russian");

    int   N[Qty];
    bool  Change;

    for ( uint32  I = 0; I < Qty; ++I )
    {
        printf("Введите число %i: ", I);
        scanf_s("%i", &N[I]);
    }

    do
    {
        Change = false;
        for ( int I = 0; I <= Qty - 2; ++I )
        {
            if ( N[I] > N[I + 1] )
            {
                int Q = N[I];
                N[I] = N[I + 1];
                N[I + 1] = Q;
                Change = true;
            }
        }
    } while (Change);

    printf("\nРезультаты:");

    for ( uint32  I = 0; I < Qty; ++I )
    {
        printf("\nN[%i] = %i", I, N[I]);
    }

    int32  M;
    printf("\nВведите искомое число: ");
    scanf_s("%i", &M);

    int32  L = 0;
    int32  R = Qty - 1;

    while (true)
    {
        int32  X = L + (R - L + 1)/2;

        if ( M == N[X] )
        {
            printf("\nЧисло найдено в позиции %i",X);
            break;
        }

        if ( L == R )
        {
            printf("\nЧисло не найдено");
            break;
        }

        if ( M > N[X] )
        {
            L = X + 1;
        }
        else
        {
            R = X - 1;
        }
    }
}
