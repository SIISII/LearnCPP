#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <cstring>

#include  <vector>


using  uint32  = std::uint32_t;

using  Vec  = std::vector<float>;


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

    Vec    N1, N2, N;
    FILE  *F;

    N1.reserve(10);

    fopen_s(&F, "File1.txt", "r");

    while ( !feof(F) )
    {
        float  Q;

        fscanf_s(F, "%f", &Q);

        N1.push_back(Q);
    }

    fclose(F);

    N2.reserve(10);

    fopen_s(&F, "File2.txt", "r");

    while ( !feof(F) )
    {
        float  Q;

        fscanf_s(F, "%f", &Q);

        N2.push_back(Q);
    }

    fclose(F);

    Sort(N1);
    Sort(N2);

    N.reserve(N1.size() + N2.size());

    uint32  IN1 = 0;
    uint32  IN2 = 0;

    while ( IN1 < N1.size() || IN2 < N2.size() )
    {
        if ( IN1 == N1.size() )
        {
            N.push_back(N2[IN2++]);
        }
        else if ( IN2 == N2.size() )
        {
            N.push_back(N1[IN1++]);
        }
        else if ( N1[IN1] < N2[IN2] )
        {
            N.push_back(N1[IN1++]);
        }
        else
        {
            N.push_back(N2[IN2++]);
        }
    }

    fopen_s(&F, "FileResult.txt", "w");

    for ( auto  Q : N )
    {
        fprintf(F, "%f\n", Q);
    }

    fclose(F);
}