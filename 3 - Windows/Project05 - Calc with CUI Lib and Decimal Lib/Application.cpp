#include  <exception>

#include  <Windows.h>

#include  "Application.h"


HINSTANCE  Application::Instance = NULL;


Application::Application()
{
    static int  Count = 0;

    if ( Count > 0 )
    {
        throw std::exception("Попытка повторного создания Application");
    }

    Count++;
}


int  Application::Run(HINSTANCE  hInstance)
{
    Instance = hInstance;

    Init();

    MSG  Msg = { };
    while ( GetMessageW(&Msg, NULL, 0, 0) )
    {
        TranslateMessage(&Msg);
        DispatchMessageW(&Msg);
    }

    Destroy();

    return static_cast<int>(Msg.wParam);
}


void  Application::Terminate()
{
    PostQuitMessage(0);
}
