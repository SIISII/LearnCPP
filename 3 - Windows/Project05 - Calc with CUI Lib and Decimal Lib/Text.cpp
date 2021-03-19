#include  "Text.h"
#include  "Window.h"


Text_Format::Text_Format(
    const wchar_t  *Font_Name,
    std::uint32_t   Font_Size,
    H_Alignment     H_Align,
    V_Alignment     V_Align)
{
    Window::pDW_Factory->CreateTextFormat(
            Font_Name,
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            static_cast<FLOAT>(Font_Size),
            L"", //locale
            &DW_Format);

    switch ( H_Align )
    {
        case  H_Alignment::Left:
            DW_Format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
            break;

        case  H_Alignment::Center:
            DW_Format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
            break;

        case  H_Alignment::Right:
            DW_Format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
            break;

        case  H_Alignment::Justified:
            DW_Format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
            break;
    }

    switch ( V_Align )
    {
        case  V_Alignment::Top:
            DW_Format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            break;

        case  V_Alignment::Center:
            DW_Format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            break;

        case  V_Alignment::Bottom:
            DW_Format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
            break;
    }
}
