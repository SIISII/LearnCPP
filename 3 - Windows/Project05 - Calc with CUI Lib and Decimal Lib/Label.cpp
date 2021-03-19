#include  "Label.h"
#include  "Window.h"

Label::Label(
    Window         *Parent,
    int             Init_X,
    int             Init_Y,
    int             Init_Width,
    int             Init_Height,
    const wchar_t  *Caption,
    Text_Format    *Format,
    std::uint32_t   Color
)
    : Control (Parent, Init_X, Init_Y, Init_Width, Init_Height)
    , Caption (Caption)
    , Format  (Format)
    , Color   (Color)
{}


void  Label::Paint()
{
    Wnd->Write_Text(Caption.c_str(), X, Y, Width, Height, Format, Color);
}


void  Label::Set_Caption(const wchar_t  *Caption)
{
    this->Caption = Caption;
}
