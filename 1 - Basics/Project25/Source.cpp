#include  <cstdio>
#include  <clocale>
#include  <cstdint>
#include  <cstdlib>
#include  <cstring>


using   int32  = std::int32_t;
using  uint32  = std::uint32_t;


int  main()
{
    std::setlocale(LC_ALL, "Russian");

    uint32  Max_Len = 0;

    uint32  Cur_Len = 0;

    FILE  *F;

    fopen_s(&F, "myfile.txt", "r");

    char  *S = nullptr;

    char  C;

    do
    {
        C = getc(F);

        // ≈сли текуща€ строка заполнена полностью, нужно
        // выделить новую строку.
        if ( Cur_Len == Max_Len )
        {
            // ≈сли строка ещЄ не создавалась, создать новую строку.
            if ( Max_Len == 0 )
            {
                S = new char[100];
                Max_Len = 100;
            }

            // »наче создать новую, более длинную строку, скопировать
            // в неЄ старую строку, уничтожить старую строку и установить
            // новую строку вместо старой.
            else
            {
                char  *P = new char[Max_Len + 100];
                memcpy(P, S, Max_Len);
                delete [] S;
                S = P;
                Max_Len = Max_Len + 100;
            }
        }

        if ( C != '\n' )
        {
            S[Cur_Len] = C;
        }
        else
        {
            S[Cur_Len] = '\0';
        }

        Cur_Len++;

    } while ( C != '\n' );

    fclose(F);

    printf_s("%s", S);

    delete [] S;
}
