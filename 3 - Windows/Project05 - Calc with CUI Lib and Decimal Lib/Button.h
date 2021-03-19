#pragma once

#include  <string>

#include  "Control.h"


class  Text_Format;


class  Button : public Control
{
protected:

    std::wstring    Caption;
    Text_Format    *Format;
    std::uint32_t   Background_Color;
    std::uint32_t   Text_Color;

public:

    Button(
        Window         *Parent,
        int             Init_X,
        int             Init_Y,
        int             Init_Width,
        int             Init_Height,
        const wchar_t  *Caption,
        Text_Format    *Format,
        std::uint32_t   Text_Color,
        std::uint32_t   Background_Color);


    inline std::uint32_t  Get_Background_Color();

    inline void  Set_Background_Color(std::uint32_t  Color);


    virtual void  Paint() override;
};


inline std::uint32_t  Button::Get_Background_Color()
{
    return Background_Color;
}


inline void  Button::Set_Background_Color(std::uint32_t  Color)
{
    Background_Color = Color;
}
