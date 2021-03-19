#include  <cstdint>

#include  <Windows.h>
#include  <wingdi.h>
#include  <d2d1.h>
#include  <dwrite.h>

#include  <cstdio>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")


// ���� ��������, ���������� �������� ������ (������ ��� �� ����������).
enum  BTN  : uint8_t
{
    BTN_0   = 0,    // �����
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


// ����������� ������� ������.
using  Btn_Handler = void (*)(BTN  Code);

void  Btn_Digit(BTN  Code);
void  Btn_Other(BTN  Code);


// ����������� ������� �� ������ ����������.
struct  Key_Desc
{
    char            Key_Code;
    BTN             Code;
    Btn_Handler     Handler;
};

const Key_Desc  Keys[] =
{
    { '0',    BTN_0,       Btn_Digit },
    { '1',    BTN_1,       Btn_Digit },
    { '2',    BTN_2,       Btn_Digit },
    { '3',    BTN_3,       Btn_Digit },
    { '4',    BTN_4,       Btn_Digit },
    { '5',    BTN_5,       Btn_Digit },
    { '6',    BTN_6,       Btn_Digit },
    { '7',    BTN_7,       Btn_Digit },
    { '8',    BTN_8,       Btn_Digit },
    { '9',    BTN_9,       Btn_Digit },
    { '+',    BTN_ADD,     Btn_Other },
    { '-',    BTN_SUB,     Btn_Other },
    { '*',    BTN_MUL,     Btn_Other },
    { '/',    BTN_DIV,     Btn_Other },
    { '=',    BTN_ASN,     Btn_Other },
    { '\xD',  BTN_ASN,     Btn_Other },
    { '.',    BTN_POINT,   Btn_Other },
    { '%',    BTN_PROCENT, Btn_Other },
    { '\x1B', BTN_C,       Btn_Other },
    { '\x8',  BTN_BS,      Btn_Other }
};


// ���������, ����������� ��������� � ������� ������.
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


// ������� ���������� �������.

constexpr int  Client_Area_Width  = Btn_X4 + Btn_Size + Btn_X1;
constexpr int  Client_Area_Height = Btn_Y6 + Btn_Size + Btn_X1;


struct  Button_Desc
{
    // ���������� ������ �������� � ������� ������� ����� ������.
    int             LX;
    int             TY;
    int             RX;
    int             BY;

    // ��������� �� �����, ��������� �� ������.
    const wchar_t  *Text;

    // ��� ������ ������.
    BTN             Code;

    // ����� �������-����������� ������� �� ������ ������.
    Btn_Handler     Handler;
};


const Button_Desc  Buttons[] =
{
    {
        Btn_X1,     Btn_Y1,     Btn_X1 + Btn_Size,      Btn_Y1 + Btn_Size,
        L"MC",
        BTN_MC,
        Btn_Other
    },
    {
        Btn_X1,     Btn_Y2,     Btn_X1 + Btn_Size,      Btn_Y2 + Btn_Size,
        L"C",
        BTN_C,
        Btn_Other
    },
    {
        Btn_X1,     Btn_Y3,     Btn_X1 + Btn_Size,      Btn_Y3 + Btn_Size,
        L"7",
        BTN_7,
        Btn_Digit
    },
    {
        Btn_X1,     Btn_Y4,     Btn_X1 + Btn_Size,      Btn_Y4 + Btn_Size,
        L"4",
        BTN_4,
        Btn_Digit
    },
    {
        Btn_X1,     Btn_Y5,     Btn_X1 + Btn_Size,      Btn_Y5 + Btn_Size,
        L"1",
        BTN_1,
        Btn_Digit
    },
    {
        Btn_X1,     Btn_Y6,     Btn_X1 + Btn_Size,      Btn_Y6 + Btn_Size,
        L"%",
        BTN_PROCENT,
        Btn_Other
    },
    {
        Btn_X2,     Btn_Y1,     Btn_X2 + Btn_Size,      Btn_Y1 + Btn_Size,
        L"M+",
        BTN_M_ADD,
        Btn_Other
    },
    {
        Btn_X2,     Btn_Y2,     Btn_X2 + Btn_Size,      Btn_Y2 + Btn_Size,
        L"/",
        BTN_DIV,
        Btn_Other
    },
    {
        Btn_X2,     Btn_Y3,     Btn_X2 + Btn_Size,      Btn_Y3 + Btn_Size,
        L"8",
        BTN_8,
        Btn_Digit
    },
    {
        Btn_X2,     Btn_Y4,     Btn_X2 + Btn_Size,      Btn_Y4 + Btn_Size,
        L"5",
        BTN_5,
        Btn_Digit
    },
    {
        Btn_X2,     Btn_Y5,     Btn_X2 + Btn_Size,      Btn_Y5 + Btn_Size,
        L"2",
        BTN_2,
        Btn_Digit
    },
    {
        Btn_X2,     Btn_Y6,     Btn_X2 + Btn_Size,      Btn_Y6 + Btn_Size,
        L"0",
        BTN_0,
        Btn_Digit
    },
    {
        Btn_X3,     Btn_Y1,     Btn_X3 + Btn_Size,      Btn_Y1 + Btn_Size,
        L"M-",
        BTN_M_SUB,
        Btn_Other
    },
    {
        Btn_X3,     Btn_Y2,     Btn_X3 + Btn_Size,      Btn_Y2 + Btn_Size,
        L"*",
        BTN_MUL,
        Btn_Other
    },
    {
        Btn_X3,     Btn_Y3,     Btn_X3 + Btn_Size,      Btn_Y3 + Btn_Size,
        L"9",
        BTN_9,
        Btn_Digit
    },
    {
        Btn_X3,     Btn_Y4,     Btn_X3 + Btn_Size,      Btn_Y4 + Btn_Size,
        L"6",
        BTN_6,
        Btn_Digit
    },
    {
        Btn_X3,     Btn_Y5,     Btn_X3 + Btn_Size,      Btn_Y5 + Btn_Size,
        L"3",
        BTN_3,
        Btn_Digit
    },
    {
        Btn_X3,     Btn_Y6,     Btn_X3 + Btn_Size,      Btn_Y6 + Btn_Size,
        L".",
        BTN_POINT,
        Btn_Other
    },
    {
        Btn_X4,     Btn_Y1,     Btn_X4 + Btn_Size,      Btn_Y1 + Btn_Size,
        L"MR",
        BTN_MR,
        Btn_Other
    },
    {
        Btn_X4,     Btn_Y2,     Btn_X4 + Btn_Size,      Btn_Y2 + Btn_Size,
        L"BS",
        BTN_BS,
        Btn_Other
    },
    {
        Btn_X4,     Btn_Y3,     Btn_X4 + Btn_Size,      Btn_Y3 + Btn_Size,
        L"-",
        BTN_SUB,
        Btn_Other
    },
    {
        Btn_X4,     Btn_Y4,     Btn_X4 + Btn_Size,      Btn_Y4 + Btn_Size,
        L"+",
        BTN_ADD,
        Btn_Other
    },
    {
        Btn_X4,     Btn_Y5,     Btn_X4 + Btn_Size,      Btn_Y6 + Btn_Size,
        L"=",
        BTN_ASN,
        Btn_Other
    }
};


HINSTANCE               hApp;
HWND                    hWnd;
ID2D1Factory           *pFactory            = nullptr;
IDWriteFactory         *pDWrite_Factory     = nullptr;
ID2D1HwndRenderTarget  *pRender_Target      = nullptr;
ID2D1SolidColorBrush   *pNumber_Brush       = nullptr;
ID2D1SolidColorBrush   *pButton_Brush       = nullptr;
ID2D1SolidColorBrush   *pText_Brush         = nullptr;
IDWriteTextFormat      *pButton_Text_Format = nullptr;
IDWriteTextFormat      *pNumber_Text_Format = nullptr;


// ����������� - �����, ������� ������������ ��� ������������.
float  Acc = 0.0f;

// ������� �������� ������������ ��� ����� �����.
bool   Clear_Acc = true;

// �����, �� ������� ���� �������� �������� ������ ����� ������������
// � ����� � ������������.
float  Dec_Pos = 0.0f;

// ������ �������.
float  Operand;

// ������ ������.
float  Memory = 0.0f;

// ������� ������� ���������� � ������ ������.
bool   Memory_Filled = false;

// ��������.
enum  Operations : uint8_t
{
    OP_NONE,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
};

Operations  Operation = OP_NONE;


// =============================================================================
//
// Safe_Release
//
// =============================================================================

template <class  T>
void  Safe_Release(T*  &pT)
{
    if ( pT != nullptr )
    {
        pT->Release();
        pT = nullptr;
    }
}


// =============================================================================
//
// Create_Graphics_Resources
//
// =============================================================================

HRESULT  Create_Graphics_Resources()
{
    static const wchar_t  Font_Name[] = L"Verdana";

    HRESULT  HR = S_OK;

    if ( pRender_Target == nullptr )
    {
        RECT  RC;

        // ��������� �������� ���������� (�.�. ��������) ������� ���� � ����
        // ��������� ������ �������� � ������� ������� �����. ���������� ������
        // �������� ���� ������ ����� ����.
        GetClientRect(hWnd, &RC);

        // ��������� �������� ���������� �������; �������� ��� ����� �����������
        // � ������� ������� ����.
        D2D1_SIZE_U  Size = D2D1::SizeU(RC.right, RC.bottom);

        // �������� ���� ���������� (�����������, �� ������� ����� ���� ������-
        // ���). ������������ ����������� ���������, ����������� �������� ����-
        // ��� RenderTargetProperties � HwndRenderTargetProperties.
        HR = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hWnd, Size),
            &pRender_Target);

        // ���� ���� ���������� ������� �������, ������ ��������� �����������
        // ��� ��������� ����.
        if ( SUCCEEDED(HR) )
        {
            D2D1_COLOR_F  Color = D2D1::ColorF(0.8f, 0.8f, 0.8f);

            pRender_Target->CreateSolidColorBrush(Color, &pNumber_Brush);

            Color = D2D1::ColorF(0.5f, 0.3f, 0.8f);

            pRender_Target->CreateSolidColorBrush(Color, &pButton_Brush);

            Color = D2D1::ColorF(1.0f, 1.0f, 1.0f);

            pRender_Target->CreateSolidColorBrush(Color, &pText_Brush);

            pDWrite_Factory->CreateTextFormat(
                    Font_Name,
                    nullptr,
                    DWRITE_FONT_WEIGHT_NORMAL,
                    DWRITE_FONT_STYLE_NORMAL,
                    DWRITE_FONT_STRETCH_NORMAL,
                    36,
                    L"", //locale
                    &pButton_Text_Format);

            pButton_Text_Format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

            pButton_Text_Format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

            pDWrite_Factory->CreateTextFormat(
                    Font_Name,
                    nullptr,
                    DWRITE_FONT_WEIGHT_NORMAL,
                    DWRITE_FONT_STYLE_NORMAL,
                    DWRITE_FONT_STRETCH_NORMAL,
                    36,
                    L"", //locale
                    &pNumber_Text_Format);

            pNumber_Text_Format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

            pNumber_Text_Format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        }
    }

    return HR;
}


// =============================================================================
//
// Discard_Graphics_Resources
//
// =============================================================================

void  Discard_Graphics_Resources()
{
    Safe_Release(pNumber_Text_Format);
    Safe_Release(pButton_Text_Format);
    Safe_Release(pText_Brush);
    Safe_Release(pButton_Brush);
    Safe_Release(pNumber_Brush);
    Safe_Release(pRender_Target);
}


// =============================================================================
//
// Paint
//
// =============================================================================

void  Paint()
{
    HRESULT  HR = Create_Graphics_Resources();

    if ( SUCCEEDED(HR) )
    {
        wchar_t  S[32];

        PAINTSTRUCT  PS;
        BeginPaint(hWnd, &PS);

        pRender_Target->BeginDraw();

        pRender_Target->Clear(D2D1::ColorF(D2D1::ColorF::White));

        // ���� ��� �����.
        pRender_Target->FillRectangle(
            D2D1::Rect(Btn_X1, Btn_Y1 - 20 - Btn_Size, Btn_X4 + Btn_Size, Btn_Y1 - 20),
            pNumber_Brush);

        swprintf(S, sizeof(S) / sizeof(S[0]), L"%f", Acc);

        pRender_Target->DrawTextW(
            S,
            wcslen(S),
            pNumber_Text_Format,
            D2D1::RectF(Btn_X1, Btn_Y1 - 20 - Btn_Size, Btn_X4 + Btn_Size, Btn_Y1 - 20),
            pText_Brush);

        if ( Memory_Filled )
        {
            pRender_Target->DrawTextW(
                L"m",
                1,
                pNumber_Text_Format,
                D2D1::RectF(Btn_X1 + 5, Btn_Y1 - 5 - Btn_Size, Btn_X1 + 40, Btn_Y1 - 20),
                pText_Brush);
        }

        // �������� ������.
        for ( const auto &B: Buttons )
        {
            pRender_Target->FillRectangle(
                D2D1::Rect(B.LX, B.TY, B.RX, B.BY),
                pButton_Brush);

            pRender_Target->DrawTextW(
                B.Text,
                wcslen(B.Text),
                pButton_Text_Format,
                D2D1::RectF(B.LX, B.TY, B.RX, B.BY),
                pText_Brush);
        }

        HR = pRender_Target->EndDraw();

        if ( FAILED(HR) || HR == D2DERR_RECREATE_TARGET )
        {
            Discard_Graphics_Resources();
        }

        EndPaint(hWnd, &PS);
    }
}


// =============================================================================
//
// Handle_Mouse_Click
//
// =============================================================================

void  Handle_Mouse_Click(POINT  Point)
{
    for ( const auto &B: Buttons )
    {
        if ( B.LX <= Point.x
             &&
             B.TY <= Point.y
             &&
             B.RX >= Point.x
             &&
             B.BY >= Point.y )
        {
            B.Handler(B.Code);
        }
    }
}


// =============================================================================
//
// Window_Proc
//
// =============================================================================

LRESULT CALLBACK  Window_Proc(
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
    switch ( uMsg )
    {
        // ���������� �������� ����. ��� ��������� ������� � �������������� �
        // �������� ���������� CreateWindowExW (�� ������ �������� ����� ����-
        // �����). ���� ������������ �������� -1, �������� ���� ������������, �
        // CreateWindowExW ����� ������� �����.
        //
        // ����� �� ������ ������� ���� ID2D1Factory, ������� ������������
        // ������������ ��� �������� �������� Direct2D. ��� ������� ������������
        // ������������ �� ������ ���� ���������� pFactory, � ������� �����
        // ������� ��������� �� ��������� �������.
        case WM_CREATE:
            if ( FAILED( D2D1CreateFactory(
                    D2D1_FACTORY_TYPE_SINGLE_THREADED,
                    &pFactory) ) )
            {
                return -1;
            }

            if ( FAILED( DWriteCreateFactory(
                    DWRITE_FACTORY_TYPE_SHARED,
                    __uuidof(IDWriteFactory),
                    reinterpret_cast<IUnknown**>(&pDWrite_Factory)) ) )
            {
                return -1;
            }

            return 0;

        // ��������� ����. ����� ������� ����������� ������� � ������������
        // ���������� ����������� ����������� ����.
        case WM_PAINT:
            Paint();
            return 0;

        // ��������� ������� �����.
        case WM_LBUTTONDOWN:
            POINT  Point;

            Point.x = static_cast<std::uint16_t>(lParam);
            Point.y = static_cast<std::uint16_t>(lParam >> 16);
            Handle_Mouse_Click(Point);
            return 0;

        // ���� ������� � ����������.
        case WM_CHAR:
            for ( const auto  &K : Keys )
            {
                if ( K.Key_Code == wParam )
                {
                    K.Handler(K.Code);
                    break;
                }
            }
            break;

        // ����������� ���� �������� ���������� ���������, ��� ���� � �������
        // ��������� ����������� ��������� WM_QUIT. ����� ����, ������, ������-
        // ������ ��� ��������� �������, � ����� �������.
        case  WM_DESTROY:
            Discard_Graphics_Resources();
            Safe_Release(pDWrite_Factory);
            Safe_Release(pFactory);
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}


// =============================================================================
//
// wWinMain
//
// =============================================================================

int WINAPI  wWinMain(
    HINSTANCE  hInstance,
    HINSTANCE  hPrevInstance,
    PWSTR      pCmdLine,
    int        nCmdShow)
{
    hApp = hInstance;

    // -----------------------
    // ����������� ������ ����
    // -----------------------

    WNDCLASSW  WC = { };

    const wchar_t  Class_Name[]  = L"Circle Window Class";

    WC.lpfnWndProc   = Window_Proc;
    WC.hInstance     = hInstance;
    WC.lpszClassName = Class_Name;

    RegisterClassW(&WC);


    // ---------------------------
    // �������� � ����������� ����
    // ---------------------------

    hWnd = CreateWindowExW(
        WS_EX_DLGMODALFRAME,            // Optional window styles
        Class_Name,                     // Window class
        L"���������� �����������",      // Window text
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT,                  // Size and position
        CW_USEDEFAULT,
        Client_Area_Width + 15,
        Client_Area_Height + 40,
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        nullptr);                       // Additional application data

    if ( hWnd == NULL )
    {
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);


    // ------------------------
    // ���� ��������� ���������
    // ------------------------

    MSG  Msg = { };

    while ( GetMessageW(&Msg, NULL, 0, 0) )
    {
        TranslateMessage(&Msg);
        DispatchMessageW(&Msg);
    }

    return 0;
}


// =============================================================================
//
// Btn_Digit
//
// =============================================================================

void  Btn_Digit(BTN  Code)
{
    if ( Clear_Acc )
    {
        Acc = 0;
        Clear_Acc = false;
    }

    if ( Dec_Pos == 0.0f )
    {
        Acc = Acc * 10.0f + static_cast<float>(Code);
    }
    else
    {
        Acc = Acc + Dec_Pos * static_cast<float>(Code);
        Dec_Pos = Dec_Pos * 0.1f;
    }

    Paint();
}


// =============================================================================
//
// Btn_Other
//
// =============================================================================

void  Btn_Other(BTN  Code)
{
    switch ( Code )
    {
        case BTN_C:
            Acc = 0;
            Dec_Pos = 0.0f;
            break;

        case BTN_POINT:
            if ( Dec_Pos == 0.0f )
            {
                Dec_Pos = 0.1f;
            }
            break;

        case BTN_ADD:
            Operand = Acc;
            Clear_Acc = true;
            Operation = OP_ADD;
            Dec_Pos = 0.0f;
            break;

        case BTN_SUB:
            Operand = Acc;
            Clear_Acc = true;
            Operation = OP_SUB;
            Dec_Pos = 0.0f;
            break;

        case BTN_MUL:
            Operand = Acc;
            Clear_Acc = true;
            Operation = OP_MUL;
            Dec_Pos = 0.0f;
            break;

        case BTN_DIV:
            Operand = Acc;
            Clear_Acc = true;
            Operation = OP_DIV;
            Dec_Pos = 0.0f;
            break;

        case BTN_ASN:
            switch ( Operation )
            {
                case OP_ADD:
                    Acc = Operand + Acc;
                    break;

                case OP_SUB:
                    Acc = Operand - Acc;
                    break;

                case OP_MUL:
                    Acc = Operand * Acc;
                    break;

                case OP_DIV:
                    Acc = Operand / Acc;
                    break;
            }

            Clear_Acc = true;
            break;

        case BTN_MC:
            Memory = 0.0f;
            Memory_Filled = false;
            break;

        case BTN_MR:
            Acc = Memory;
            Clear_Acc = true;
            break;

        case BTN_M_ADD:
            Memory = Memory + Acc;
            Memory_Filled = true;
            break;

        case BTN_M_SUB:
            Memory = Memory - Acc;
            Memory_Filled = true;
            break;

        case BTN_PROCENT:
            Acc = Operand / 100.0f * Acc;
            Clear_Acc = true;
            break;
    }

    Paint();
}
