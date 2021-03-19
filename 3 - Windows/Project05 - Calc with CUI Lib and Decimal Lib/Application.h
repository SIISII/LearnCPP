#pragma  once

#include  <Windows.h>


// =============================================================================
//
// Safe_Release
//
// =============================================================================

template <class  T>
void  Safe_Release(T*  &pT)
{
    if ( pT != nullptr )
    {
        pT->Release();
        pT = nullptr;
    }
}


// =============================================================================
//
// Application
//
// =============================================================================

class  Application
{
private:

    static HINSTANCE  Instance;

public:

    Application();

    ~Application()
    {}

    int  Run(HINSTANCE  hInstance);

    virtual void  Init()
    {}

    virtual void  Destroy()
    {}

    inline static HINSTANCE  Get_Instance();

    void  Terminate();
};


inline HINSTANCE  Application::Get_Instance()
{
    return Instance;
}
