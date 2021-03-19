#pragma once

#include  <cstdint>

#include  <Windows.h>
#include  <d2d1.h>
#include  <dwrite.h>


class  Window;


class  Text_Format
{
private:

    friend Window;

    IDWriteTextFormat   *DW_Format;

public:

    enum class  H_Alignment : std::uint8_t
    {
        Left,
        Center,
        Right,
        Justified
    };

    enum class  V_Alignment : std::uint8_t
    {
        Top,
        Center,
        Bottom
    };


    Text_Format(
        const wchar_t  *Font_Name,
        std::uint32_t   Font_Size,
        H_Alignment     H_Align,
        V_Alignment     V_Align);

};
