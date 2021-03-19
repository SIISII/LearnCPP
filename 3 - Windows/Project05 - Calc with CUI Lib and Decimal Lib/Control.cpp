#include  "Control.h"

Control::Control(
    Window         *Parent,
    int             Init_X,
    int             Init_Y,
    int             Init_Width,
    int             Init_Height
)
    : Wnd               (Parent)
    , X                 (Init_X)
    , Y                 (Init_Y)
    , Width             (Init_Width)
    , Height            (Init_Height)
    , On_Click          (nullptr)
    , On_Move           (nullptr)
    , On_Key_Pressed    (nullptr)
    , User_Data         (nullptr)
{}


void  Control::Set_Pos(
    int   New_X,
    int   New_Y,
    int   New_Width,
    int   New_Height)
{
    X = New_X;
    Y = New_Y;
    Width = New_Width;
    Height = New_Height;

    if ( On_Move != nullptr )
    {
        On_Move(this);
    }
}


bool  Control::In_Object(
    int  X,
    int  Y)
{
    if ( X < this->X )
    {
        return false;
    }
    
    if ( X >= this-> X + Width )
    {
        return false;
    }

    if ( Y >= this->Y + Height )
    {
        return false;
    }

    if ( Y < this->Y )
    {
        return false;
    }

    return true;
}