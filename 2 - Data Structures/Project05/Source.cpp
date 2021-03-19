#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <cstring>

#include  <vector>


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    std::vector<int>  N;

    N.reserve(10);

    printf("\nN.size() = %i", N.size());
    printf("\nN.capacity() = %i\n", N.capacity());

    while (true)
    {
        int  Q;

        printf("Введите число: ");
        scanf_s("%i", &Q);
        if ( Q == 0 )
        {
            break;
        }

        N.push_back(Q);

        printf("\nN.size() = %i", N.size());
        printf("\nN.capacity() = %i\n", N.capacity());
    }

    N.pop_back();

    printf("\nМассив:\n");

    for ( int  I = 0; I < N.size(); ++I )
    {
        printf("%i\n", N[I]);
    }

    printf("\nN.size() = %i", N.size());

    printf("\nN.capacity() = %i", N.capacity());

    std::vector<int>  M = N;

    printf("\nM.size() = %i", M.size());

    printf("\nM.capacity() = %i", M.capacity());
}
