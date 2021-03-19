#include  <Windows.h>
#include  "Decimal.h"
#include  "Application.h"
#include  "Button.h"
#include  "Text.h"
#include  "Window.h"
#include  "Label.h"
#include  "Control.h"
#include  <cstring>

// Аккумулятор - число, которое отображается для пользователя.
char  Acc[Max_Dec_Size];

// Признак стирания аккумулятора при вводе цифры.
bool  Clear_Acc = true;

// Число, на которое надо умножить значение кнопки перед прибавлением
// к числу в аккумуляторе.
bool  Has_Point = false;

// Первый операнд.
char  Operand[Max_Dec_Size];

// Ячейка памяти.
char  Memory[Max_Dec_Size];

bool  Memory_Filled = false;


void  Btn_Digit(Control  *C);
void  Btn_Other(Control  *C);

void  Key_Pressed(
    Control  *C,
    char      Key);


// Коды операций, вызываемых нажатием клавиш (мышкой или на клавиатуре).
enum  BTN  : uint8_t
{
    BTN_0   = 0,    // Цифры
    BTN_1,
    BTN_2,
    BTN_3,
    BTN_4,
    BTN_5,
    BTN_6,
    BTN_7,
    BTN_8,
    BTN_9,
    BTN_ADD,        // +
    BTN_SUB,        // -
    BTN_MUL,        // *
    BTN_DIV,        // /
    BTN_ASN,        // =
    BTN_POINT,      // .
    BTN_MC,         // MC
    BTN_MR,         // MR
    BTN_M_ADD,      // M+
    BTN_M_SUB,      // M-
    BTN_C,          // C
    BTN_PROCENT,    // %
    BTN_BS          // Backspace
};


// Константы, описывающие положение и размеры кнопок.
constexpr int  Btn_X1 =  10;
constexpr int  Btn_X2 = 100;
constexpr int  Btn_X3 = 190;
constexpr int  Btn_X4 = 280;

constexpr int  Btn_Y1 = 100;
constexpr int  Btn_Y2 = 190;
constexpr int  Btn_Y3 = 280;
constexpr int  Btn_Y4 = 370;
constexpr int  Btn_Y5 = 460;
constexpr int  Btn_Y6 = 550;

constexpr int  Btn_Size = 70;


// Размеры клиентской области.

constexpr int  Client_Area_Width  = Btn_X4 + Btn_Size + Btn_X1;
constexpr int  Client_Area_Height = Btn_Y6 + Btn_Size + Btn_X1;


struct  Button_Desc
{
    // Координаты левого верхнего и правого нижнего углов кнопки.
    int             LX;
    int             TY;
    int             RX;
    int             BY;

    // Указатель на текст, выводимый на кнопке.
    const wchar_t  *Text;

    // Код данной кнопки.
    BTN             Code;

    // Символ, связанный с кнопкой.
    char            Key;

    // Адрес функции-обработчика нажатия на данную кнопку.
    Event_Handler   Handler;
};


const Button_Desc  Buttons[] =
{
    {
        Btn_X1,     Btn_Y1,     Btn_Size,      Btn_Size,
        L"MC",
        BTN_MC,
        '\0',
        Btn_Other
    },
    {
        Btn_X1,     Btn_Y2,     Btn_Size,      Btn_Size,
        L"C",
        BTN_C,
        '\x1B',
        Btn_Other
    },
    {
        Btn_X1,     Btn_Y3,     Btn_Size,      Btn_Size,
        L"7",
        BTN_7,
        '7',
        Btn_Digit
    },
    {
        Btn_X1,     Btn_Y4,     Btn_Size,      Btn_Size,
        L"4",
        BTN_4,
        '4',
        Btn_Digit
    },
    {
        Btn_X1,     Btn_Y5,     Btn_Size,      Btn_Size,
        L"1",
        BTN_1,
        '1',
        Btn_Digit
    },
    {
        Btn_X1,     Btn_Y6,     Btn_Size,      Btn_Size,
        L"%",
        BTN_PROCENT,
        '%',
        Btn_Other
    },
    {
        Btn_X2,     Btn_Y1,     Btn_Size,      Btn_Size,
        L"M+",
        BTN_M_ADD,
        '\0',
        Btn_Other
    },
    {
        Btn_X2,     Btn_Y2,     Btn_Size,      Btn_Size,
        L"/",
        BTN_DIV,
        '/',
        Btn_Other
    },
    {
        Btn_X2,     Btn_Y3,     Btn_Size,      Btn_Size,
        L"8",
        BTN_8,
        '8',
        Btn_Digit
    },
    {
        Btn_X2,     Btn_Y4,     Btn_Size,      Btn_Size,
        L"5",
        BTN_5,
        '5',
        Btn_Digit
    },
    {
        Btn_X2,     Btn_Y5,     Btn_Size,      Btn_Size,
        L"2",
        BTN_2,
        '2',
        Btn_Digit
    },
    {
        Btn_X2,     Btn_Y6,     Btn_Size,      Btn_Size,
        L"0",
        BTN_0,
        '0',
        Btn_Digit
    },
    {
        Btn_X3,     Btn_Y1,     Btn_Size,      Btn_Size,
        L"M-",
        BTN_M_SUB,
        '\0',
        Btn_Other
    },
    {
        Btn_X3,     Btn_Y2,     Btn_Size,      Btn_Size,
        L"*",
        BTN_MUL,
        '*',
        Btn_Other
    },
    {
        Btn_X3,     Btn_Y3,     Btn_Size,      Btn_Size,
        L"9",
        BTN_9,
        '9',
        Btn_Digit
    },
    {
        Btn_X3,     Btn_Y4,     Btn_Size,      Btn_Size,
        L"6",
        BTN_6,
        '6',
        Btn_Digit
    },
    {
        Btn_X3,     Btn_Y5,     Btn_Size,      Btn_Size,
        L"3",
        BTN_3,
        '3',
        Btn_Digit
    },
    {
        Btn_X3,     Btn_Y6,     Btn_Size,      Btn_Size,
        L".",
        BTN_POINT,
        '.',
        Btn_Other
    },
    {
        Btn_X4,     Btn_Y1,     Btn_Size,      Btn_Size,
        L"MR",
        BTN_MR,
        '\0',
        Btn_Other
    },
    {
        Btn_X4,     Btn_Y2,     Btn_Size,      Btn_Size,
        L"BS",
        BTN_BS,
        '\x8',
        Btn_Other
    },
    {
        Btn_X4,     Btn_Y3,     Btn_Size,      Btn_Size,
        L"-",
        BTN_SUB,
        '-',
        Btn_Other
    },
    {
        Btn_X4,     Btn_Y4,     Btn_Size,      Btn_Size,
        L"+",
        BTN_ADD,
        '+',
        Btn_Other
    },
    {
        Btn_X4,     Btn_Y5,     Btn_Size,      2 * Btn_Size + 20,
        L"=",
        BTN_ASN,
        '\xD',
        Btn_Other
    }
};


class  My_Application : public Application
{
public:

    My_Application();

    virtual void  Init() override;
};


class  My_Window : public Window
{
public:

    My_Window();

    virtual bool  On_Create() override;

    virtual void  On_Destroy() override;

    virtual void  On_Paint() override;
};


My_Application  App;

My_Window  Wnd;

// Указатель на строку вывода значения аккумулятора.
Label  *Acc_Label;


int WINAPI  wWinMain(
    HINSTANCE  hInstance,
    HINSTANCE  hPrevInstance,
    PWSTR      pCmdLine,
    int        nCmdShow)
{
    return App.Run(hInstance);
}


My_Application::My_Application()
{}


void  My_Application::Init()
{
    Acc[0] = '0';
    Acc[1] = '\0';

    Wnd.Create(
        L"My Application",
        10,
        10,
        Client_Area_Width + 15,
        Client_Area_Height + 40);

    Wnd.Show();
}


My_Window::My_Window()
{}


bool  My_Window::On_Create()
{
    Text_Format  *Format = new Text_Format(
        L"Verdana",
        36,
        Text_Format::H_Alignment::Center,
        Text_Format::V_Alignment::Center);

    for ( auto  &B: Buttons )
    {
        Button  *Btn = new Button(
            this,
            B.LX,
            B.TY,
            B.RX,
            B.BY,
            B.Text,
            Format,
            0xFFFFFFFF,
            0xFF7F50C0);

        Btn->On_Click = B.Handler;
        Btn->On_Key_Pressed = Key_Pressed;

        Btn->User_Data = reinterpret_cast<void*>(
            static_cast<uint32_t>(B.Code)
            |
            ( static_cast<uint32_t>(B.Key) << 8 ) );

        Add_Control(Btn);
    }

    Format = new Text_Format(
        L"Verdana",
        36,
        Text_Format::H_Alignment::Right,
        Text_Format::V_Alignment::Center);

    Acc_Label = new Label(
        this,
        Btn_X1,
        Btn_Y1 - 20 - Btn_Size,
        Btn_X4 + Btn_Size - Btn_X1,
        Btn_Size,
        L"0",
        Format,
        0xFF000000);

    Add_Control(Acc_Label);

    return true;
}


void  My_Window::On_Destroy()
{
    App.Terminate();
}


// Операция.
enum  Operations : uint8_t
{
    OP_NONE,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
};

Operations  Operation = OP_NONE;


void  Btn_Digit(Control  *C)
{
    if ( Clear_Acc )
    {
        Acc[0] = '0';
        Acc[1] = '\0';
        Clear_Acc = false;
        Has_Point = false;
    }

    int L = strlen(Acc);
    if (L != Max_Dec_Size - 1)
    {
        if (L == 1 && Acc[0] == '0')
        {
            L = 0;
        }
        Acc[L] = reinterpret_cast<uint8_t>(C->User_Data) + '0';
        Acc[L + 1] = '\0';
    }

    Wnd.Repaint();
}


void  Btn_Other(Control  *C)
{
    switch ( static_cast<BTN>( reinterpret_cast<uint8_t>(C->User_Data) ) )
    {
        case BTN_C:
            Acc[0] = '0';
            Acc[1] = '\0';
            Has_Point = false;
            break;

        case BTN_POINT:
            if ( !Has_Point )
            {
                Has_Point = true;
                int L = strlen(Acc);
                if (L != Max_Dec_Size - 1)
                {
                    Acc[L] = '.';
                    Acc[L + 1] = '\0';
                }
            }
            break;

        case BTN_ADD:
            memcpy(Operand, Acc, sizeof(Operand));
            Clear_Acc = true;
            Operation = OP_ADD;
            break;

        case BTN_SUB:
            memcpy(Operand, Acc, sizeof(Operand));
            Clear_Acc = true;
            Operation = OP_SUB;
            break;

        case BTN_MUL:
            memcpy(Operand, Acc, sizeof(Operand));
            Clear_Acc = true;
            Operation = OP_MUL;
            break;

        case BTN_DIV:
            memcpy(Operand, Acc, sizeof(Operand));
            Clear_Acc = true;
            Operation = OP_DIV;
            break;

        case BTN_ASN:
            switch ( Operation )
            {
                case OP_ADD:
                    Add(Operand, Acc, Acc);
                    break;

                case OP_SUB:
                    Sub(Operand, Acc, Acc);
                    break;

                case OP_MUL:
                    Mul(Operand, Acc, Acc);
                    break;

                case OP_DIV:
                    Div(Operand, Acc, Acc);
                    break;
            }

            Clear_Acc = true;
            break;

        case BTN_MC:
            Memory_Filled = false;
            break;

        case BTN_MR:
            if (Memory_Filled)
            {
                Clear_Acc = true;
                memcpy(Acc, Memory, sizeof(Memory));
            }
            break;

        case BTN_M_ADD:
            if (!Memory_Filled)
            {
                memcpy(Memory, Acc, sizeof(Acc));
                Memory_Filled = true;
            }
            else
            {
                Add(Memory, Acc, Memory);
            }
            break;

        case BTN_M_SUB:
            if (!Memory_Filled)
            {
                memcpy(Memory, Acc, sizeof(Acc));
                Memory_Filled = true;
            }
            else
            {
                Sub(Memory, Acc, Memory);
            }
            break;

        case BTN_PROCENT:
            Div(Operand, "100", Operand);
            Mul(Operand, Acc, Acc);
            Clear_Acc = true;
            break;
    }

    Wnd.Repaint();
}


void  Key_Pressed(
    Control  *C,
    char      Key)
{
    char  K = static_cast<char>( reinterpret_cast<uint16_t>(C->User_Data) >> 8 );

    if ( Key == K )
    {
        C->On_Click(C);
    }
}


void  My_Window::On_Paint()
{
    wchar_t  S[Max_Dec_Size];

    for ( int i = 0; i < Max_Dec_Size; ++i )
    {
        S[i] = Acc[i];
    }

    Acc_Label->Set_Caption(S);
}
