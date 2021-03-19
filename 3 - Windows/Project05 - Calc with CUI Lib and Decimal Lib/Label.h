#pragma once
#include  "Control.h"
#include  <string>
#include  "Text.h"

class  Label : public Control
{
protected:

    std::wstring   Caption;
    Text_Format   *Format;
    std::uint32_t  Color;

public:

    Label(
        Window         *Parent,
        int             Init_X,
        int             Init_Y,
        int             Init_Width,
        int             Init_Height,
        const wchar_t  *Caption,
        Text_Format    *Format,
        std::uint32_t   Color);

    virtual void  Paint() override;

    void  Set_Caption(const wchar_t  *Caption);
};
