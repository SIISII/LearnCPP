int  Min(
    const int  N[],
    int        Q)
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
    const int  N[],
    int        Q)
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


int  Min(
    int  A,
    int  B)
{
    if ( A < B )
    {
        return A;
    }
    else
    {
        return B;
    }
}


int  Max(
    int  A,
    int  B)
{
    if ( A > B )
    {
        return A;
    }
    else
    {
        return B;
    }
}
