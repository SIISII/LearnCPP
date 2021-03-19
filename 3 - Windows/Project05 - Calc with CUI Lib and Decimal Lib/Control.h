#pragma once

class  Window;


class  Control;


using  Event_Handler  = void  (*)(Control  *C);

using  Key_Event_Handler  = void  (*)(
    Control  *C,
    char      Key);


class  Control
{
protected:

    Window  *Wnd;

    int     X;
    int     Y;
    int     Width;
    int     Height;

public:

    Control(
        Window         *Parent,
        int             Init_X,
        int             Init_Y,
        int             Init_Width,
        int             Init_Height
    );


    void  Set_Pos(
        int   New_X,
        int   New_Y,
        int   New_Width,
        int   New_Height);


    virtual void  Paint() = 0;


    bool  In_Object(
        int  X,
        int  Y);


    // ”казатели на обработчики событий, св€занных с данным контролом.
    Event_Handler  On_Click;
    Event_Handler  On_Move;

    Key_Event_Handler  On_Key_Pressed;

    void    *User_Data;
};
