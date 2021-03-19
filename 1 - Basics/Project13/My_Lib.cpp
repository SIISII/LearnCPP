int  Min(
    int  N[],
    int  Q)
{
    int  M = N[0];

    for ( int  I = 1; I < Q; ++I )
    {
        if ( N[I] < M )
        {
            M = N[I];
        }
    }

    return M;
}


int  Max(
    int  N[],
    int  Q)
{
    int  M = N[0];

    for ( int  I = 1; I < Q; ++I )
    {
        if ( N[I] > M )
        {
            M = N[I];
        }
    }

    return M;
}
