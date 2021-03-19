#include  "Button.h"
#include  "Text.h"
#include  "Window.h"


Button::Button
(
    Window         *Parent,
    int             Init_X,
    int             Init_Y,
    int             Init_Width,
    int             Init_Height,
    const wchar_t  *Caption,
    Text_Format    *Format,
    std::uint32_t   Text_Color,
    std::uint32_t   Background_Color
)
    : Control(Parent, Init_X, Init_Y, Init_Width, Init_Height)
    , Caption           (Caption)
    , Format            (Format)
    , Text_Color        (Text_Color)
    , Background_Color  (Background_Color)
{}


void  Button::Paint()
{
    Wnd->Rectangle(X, Y, Width, Height, Background_Color);
    Wnd->Write_Text(Caption.c_str(), X, Y, Width, Height, Format, Text_Color);
}
