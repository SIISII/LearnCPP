#pragma once

#include  <cstdint>
#include  <vector>

#include  <Windows.h>
#include  <d2d1.h>
#include  <dwrite.h>


class  Control;
class  Text_Format;


class  Window
{
protected:

    friend Text_Format;

    static ATOM              Wnd_Class;

    static ID2D1Factory     *pD2D_Factory;

    static IDWriteFactory   *pDW_Factory;

    HWND                     Window_Handle;

    ID2D1HwndRenderTarget   *pRender_Target;

    ID2D1SolidColorBrush    *pBrush;

    PAINTSTRUCT              PS;

    std::vector<Control*>    Controls;


    static LRESULT CALLBACK  Window_Proc(
        HWND    hWnd,
        UINT    uMsg,
        WPARAM  wParam,
        LPARAM  lParam);


    void  Free_Resources();

public:

    Window()
        : Window_Handle  (NULL)
        , pRender_Target (nullptr)
        , pBrush         (nullptr)
    {}


    ~Window()
    {}


    void  Repaint();


    virtual bool  On_Create()
    {
        return true;
    }


    virtual void  On_Paint()
    {}


    virtual void  On_Resize()
    {
        Repaint();
    }


    virtual void  On_Destroy()
    {}

    void  Create(
        const wchar_t  *Caption,
        int             X,
        int             Y,
        int             Width,
        int             Height);


    void  Show();


    virtual void  Begin_Paint();


    virtual void  End_Paint();


    void  Rectangle(
        int            X,
        int            Y,
        std::uint32_t  Width,
        std::uint32_t  Height,
        std::uint32_t  Color);


    void  Write_Text(
        const wchar_t  *Str,
        int             X,
        int             Y,
        std::uint32_t   Width,
        std::uint32_t   Height,
        Text_Format    *Format,
        std::uint32_t   Color);


    void  Add_Control(Control  *C);
};
