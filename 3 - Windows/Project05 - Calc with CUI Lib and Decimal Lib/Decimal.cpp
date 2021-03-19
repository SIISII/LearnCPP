#include  <cstdint>
#include  <cctype>
#include  <cstring>

#include  "Decimal.h"


static void  Parse_Number(
    const char  *S,
    bool        &Neg,
    int         &Int_N,
    int         &Frac_N)
{
    if ( *S == '-' )
    {
        S++;
        Neg = true;
    }
    else
    {
        Neg = false;
    }

    Int_N = 0;

    while ( *S >= '0' && *S <= '9' )
    {
        Int_N++;
        S++;
    }

    Frac_N = 0;

    if ( *S == '.' )
    {
        S++;

        while ( isdigit(*S) )
        {
            Frac_N++;
            S++;
        }
    }
}


static void  Prepare_Number(
    const char  *S,
    char        *N,
    bool         Neg,
    int          Int_N,
    int          Frac_N,
    int          Int_N_Req,
    int          Frac_N_Req)
{
    if ( Neg )
    {
        S++;
    }

    while ( Int_N < Int_N_Req )
    {
        *N = '0';
        N++;
        Int_N++;
    }

    while ( isdigit(*S) )
    {
        *N = *S;
        N++;
        S++;
    }

    if ( Frac_N > 0 )
    {
        S++;
        while ( isdigit(*S) )
        {
            *N = *S;
            N++;
            S++;
        }
    }

    while ( Frac_N < Frac_N_Req )
    {
        *N = '0';
        N++;
        Frac_N++;
    }

    *N = '\0';
}


static void  Add(
    char        *R,
    bool        &Neg_R,
    const char  *N1,
    const char  *N2,
    bool         Neg_1,
    bool         Neg_2,
    int          Len)
{
    int8_t  C = 0;

    if ( Neg_1 == Neg_2 )
    {
        Neg_R = Neg_1;

        for ( int  I = Len - 1; I >= 0; --I )
        {
            int8_t X = N1[I] - '0';
            X = X + N2[I] - '0';

            X = X + C;

            if ( X > 9 )
            {
                C = 1;
                X = X - 10;
            }
            else
            {
                C = 0;
            }

            R[I + 1] = X + '0';
        }

        if ( C == 1 )
        {
            R[0] = '1';
        }
        else
        {
            R[0] = '0';
        }

        R[Len + 1] = '\0';
    }
    else
    {
        int  A_CMP_B = 0;

        for ( int  I = 0; I < Len; ++I )
        {
            if ( N1[I] > N2[I] )
            {
                A_CMP_B = 1;
                break;
            }

            if ( N2[I] > N1[I] )
            {
                A_CMP_B = -1;
                break;
            }
        }

        if ( A_CMP_B == 0 )
        {
            for ( int  I = 0; I < Len; ++I )
            {
                R[I] = '0';
            }

            R[Len] = '\0';
            Neg_R = false;
        }
        else
        {
            if ( A_CMP_B == 1 )
            {
                Neg_R = Neg_1;
            }
            else
            {
                Neg_R = Neg_2;
            }

            for ( int  I = Len - 1; I >= 0; --I )
            {
                int8_t  X;

                if ( A_CMP_B < 0 )
                {
                    X = N2[I] - '0';
                    X = X - N1[I] + '0';
                }
                else
                {
                    X = N1[I] - '0';
                    X = X - N2[I] + '0';
                }

                X = X - C;

                if ( X >= 0 )
                {
                    C = 0;
                }
                else
                {
                    C = 1;
                    X = X + 10;
                }

                R[I] = X + '0';
            }
            R[Len] = '\0';
        }
    }
}


static void  Div(
    char        *R,
    bool        &Neg_R,
    int         &Frac_R,
    const char  *N1,
    const char  *N2,
    bool         Neg_1,
    bool         Neg_2,
    int          Len)
{
    int  Cnt_Z1 = 0, Cnt_Z2 = 0;

    while ( N1[Cnt_Z1] == '0' )
    {
        ++Cnt_Z1;
    }

    while ( N2[Cnt_Z2] == '0' )
    {
        ++Cnt_Z2;
    }

    int Int_R = 1;

    if ( Cnt_Z2 > Cnt_Z1 )
    {
        Int_R += Cnt_Z2 - Cnt_Z1;
    }

    Frac_R = Max_Dec_Size - 1 - Int_R;

    char  D[Max_Dec_Size], E[Max_Dec_Size];
    bool  DN;
    int   CNT = 0;

    memmove(E, N1, Len);

    do
    {
        Add(D, DN, E, N2, false, true, Len);
        if ( !DN )
        {
            ++CNT;
            memmove(E, D, Len);
        }
    } while ( !DN );

    for ( int  I = Int_R - 1; I >= 0; --I )
    {
        R[I] = '0' + CNT % 10;
        CNT = CNT / 10;
    }

    E[Len] = '0';

    char NewN2[Max_Dec_Size];
    NewN2[0] = '0';
    memmove(&NewN2[1], N2, Len);

    for ( int  F = 0; F < Max_Dec_Size - 1 - Int_R; ++F )
    {
        if ( E[0] == '0' && E[1] == '\0' )
        {
            R[Int_R + F] = '\0';
            Frac_R = F;
            break;
        }

        CNT = 0;

        do
        {
            Add(D, DN, E, NewN2, false, true, Len + 1);
            if ( !DN )
            {
                ++CNT;
                memmove(E, D, Len + 1);
            }
        } while ( !DN );

        R[Int_R + F] = CNT + '0';
        memmove(E, &E[1], Len);
        E[Len] = '0';
    }

    R[Max_Dec_Size - 1] = '\0';

    Neg_R = Neg_1 != Neg_2;
}


static void  Mul(
    char        *R,
    bool        &Neg_R,
    const char  *N1,
    const char  *N2,
    bool         Neg_1,
    bool         Neg_2,
    int          Len)
{
    char  X[50], Y[50];

    for ( int  I = 0; I < 2 * Len; ++I )
    {
        Y[I] = '0';
    }

    for ( int  I = 0; I < Len; ++I )
    {
        for ( int  J = 0; J < 2 * Len; ++J )
        {
            if ( J >= I + 1 && J < I + 1 + Len )
            {
                X[J] = N1[J - I - 1];
            }
            else
            {
                X[J] = '0';
            }
        }

        for ( int  J = 0; J < N2[I] - '0'; ++J )
        {
            Add(R, Neg_R, X, Y, false, false, 2 * Len);
            for ( int  K = 0; K < 2 * Len; ++K )
            {
                Y[K] = R[K + 1];
            }
        }
    }

    Neg_R = Neg_1 != Neg_2;
}


static void  Format(
    char        *R,
    const char  *N,
    bool         Neg,
    int          Frac_N)
{
    int  IDX = 0;
    int  Len = strlen(N);

    if ( Neg )
    {
        R[IDX] = '-';
        ++IDX;
    }

    bool  Skip = true;

    for ( int  I = 0; I < Len - Frac_N; ++I )
    {
        if ( N[I] == '0' )
        {
            if ( !Skip )
            {
                R[IDX] = '0';
                ++IDX;
            }
        }
        else
        {
            R[IDX] = N[I];
            ++IDX;
            Skip = false;
        }
    }

    if ( Skip )
    {
        R[IDX] = '0';
        ++IDX;
    }

    if ( Frac_N != 0 )
    {
        int  I = Len - 1;

        while ( I >= Len - Frac_N && N[I] == '0' )
        {
            --I;
        }

        if ( I >= Len - Frac_N )
        {
            R[IDX] = '.';
            ++IDX;

            for ( int  J = Len - Frac_N; J <= I; ++J )
            {
                if ( IDX == Max_Dec_Size - 1 )
                {
                    break;
                }

                R[IDX] = N[J];
                ++IDX;
            }
        }
    }

    R[IDX] = '\0';
}


void  Add(
    const char  *A,
    const char  *B,
    char        *Res)
{
    bool  Neg_1, Neg_2;

    int   Int_N1, Int_N2, Frac_N1, Frac_N2;

    Parse_Number(A, Neg_1, Int_N1, Frac_N1);
    Parse_Number(B, Neg_2, Int_N2, Frac_N2);

    char  N1[Max_Dec_Size], N2[Max_Dec_Size];

    int  Int_N = Int_N1 > Int_N2 ? Int_N1 : Int_N2;

    int  Frac_N = Frac_N1 > Frac_N2 ? Frac_N1 : Frac_N2;

    Prepare_Number(A, N1, Neg_1, Int_N1, Frac_N1, Int_N, Frac_N);
    Prepare_Number(B, N2, Neg_2, Int_N2, Frac_N2, Int_N, Frac_N);

    char  R[Max_Dec_Size];

    bool  R_Neg;

    Add(R, R_Neg, N1, N2, Neg_1, Neg_2, Int_N + Frac_N);

    Format(Res, R, R_Neg, Frac_N);
}


void  Sub(
    const char  *A,
    const char  *B,
    char        *Res)
{
    bool  Neg_1, Neg_2;

    int   Int_N1, Int_N2, Frac_N1, Frac_N2;

    Parse_Number(A, Neg_1, Int_N1, Frac_N1);
    Parse_Number(B, Neg_2, Int_N2, Frac_N2);

    char  N1[Max_Dec_Size], N2[Max_Dec_Size];

    int  Int_N = Int_N1 > Int_N2 ? Int_N1 : Int_N2;

    int  Frac_N = Frac_N1 > Frac_N2 ? Frac_N1 : Frac_N2;

    Prepare_Number(A, N1, Neg_1, Int_N1, Frac_N1, Int_N, Frac_N);
    Prepare_Number(B, N2, Neg_2, Int_N2, Frac_N2, Int_N, Frac_N);

    char  R[Max_Dec_Size];

    bool  R_Neg;

    Neg_2 = !Neg_2;

    Add(R, R_Neg, N1, N2, Neg_1, Neg_2, Int_N + Frac_N);

    Format(Res, R, R_Neg, Frac_N);
}


void  Mul(
    const char  *A,
    const char  *B,
    char        *Res)
{
    bool  Neg_1, Neg_2;

    int   Int_N1, Int_N2, Frac_N1, Frac_N2;

    Parse_Number(A, Neg_1, Int_N1, Frac_N1);
    Parse_Number(B, Neg_2, Int_N2, Frac_N2);

    char  N1[Max_Dec_Size], N2[Max_Dec_Size];

    int  Int_N = Int_N1 > Int_N2 ? Int_N1 : Int_N2;

    int  Frac_N = Frac_N1 > Frac_N2 ? Frac_N1 : Frac_N2;

    Prepare_Number(A, N1, Neg_1, Int_N1, Frac_N1, Int_N, Frac_N);
    Prepare_Number(B, N2, Neg_2, Int_N2, Frac_N2, Int_N, Frac_N);

    char  R[Max_Dec_Size];

    bool  R_Neg;

    Mul(R, R_Neg, N1, N2, Neg_1, Neg_2, Int_N + Frac_N);

    Format(Res, R, R_Neg, 2 * Frac_N);
}


void  Div(
    const char  *A,
    const char  *B,
    char        *Res)
{
    bool  Neg_1, Neg_2;

    int   Int_N1, Int_N2, Frac_N1, Frac_N2;

    Parse_Number(A, Neg_1, Int_N1, Frac_N1);
    Parse_Number(B, Neg_2, Int_N2, Frac_N2);

    char  N1[Max_Dec_Size], N2[Max_Dec_Size];

    int  Int_N = Int_N1 > Int_N2 ? Int_N1 : Int_N2;

    int  Frac_N = Frac_N1 > Frac_N2 ? Frac_N1 : Frac_N2;

    Prepare_Number(A, N1, Neg_1, Int_N1, Frac_N1, Int_N, Frac_N);
    Prepare_Number(B, N2, Neg_2, Int_N2, Frac_N2, Int_N, Frac_N);

    char  R[Max_Dec_Size];

    bool  R_Neg;
    int   R_Frac;

    Div(R, R_Neg, R_Frac, N1, N2, Neg_1, Neg_2, Int_N + Frac_N);

    Format(Res, R, R_Neg, R_Frac);
}
