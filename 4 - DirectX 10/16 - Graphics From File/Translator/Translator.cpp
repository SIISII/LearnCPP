#include "Common_Defs.h"

THIRD_PARTY_START

#include  <vector>
#include  <cstdio>

#include  <D3DX10math.h>

THIRD_PARTY_END

struct  Vertex
{
    D3DXVECTOR3  Pos;
    D3DXVECTOR3  Normal;
};

int main()
{  

    std::vector<Vertex>  V;
    FILE  *F;

    fopen_s(&F, "Cube.geo", "r");

    uint32  Count = 0;

    while ( !feof(F) )
    {
        ++Count;
        V.resize(Count);
        fscanf_s(F, "%f", &V[Count - 1].Pos.x);
        fscanf_s(F, "%f", &V[Count - 1].Pos.y);
        fscanf_s(F, "%f", &V[Count - 1].Pos.z);
        fscanf_s(F, "%f", &V[Count - 1].Normal.x);
        fscanf_s(F, "%f", &V[Count - 1].Normal.y);
        fscanf_s(F, "%f", &V[Count - 1].Normal.z);
    }

    fclose(F);

    fopen_s(&F, "Cube.bin", "wb+");

    for ( uint32  I = 0; I < Count; ++I )
    {
        fwrite(&V[I], sizeof(Vertex), 1, F);
    }

    fclose(F);
}