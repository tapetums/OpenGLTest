// DWM.cpp

#define CLASSNAME DWM

//---------------------------------------------------------------------------//

#include "DWM.h"

//---------------------------------------------------------------------------//

struct DWMHolder
{
    HMODULE module;

    DWMHolder()
    {
        module = ::LoadLibraryExW
        (
            TEXT("dwmapi.dll"), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH
        );
    }

    ~DWMHolder()
    {
        if ( module )
        {
            ::FreeLibrary(module);
            module = nullptr;
        }
    }
};

//---------------------------------------------------------------------------//

CLASSNAME::CLASSNAME()
{
    static DWMHolder holder;

    if ( holder.module )
    {
        DwmIsCompositionEnabled = (HRESULT (__stdcall*)(BOOL*))::GetProcAddress
        (
            holder.module, "DwmIsCompositionEnabled"
        );

        DwmEnableBlurBehindWindow = (HRESULT (__stdcall*)(HWND, const DWM_BLURBEHIND*))::GetProcAddress
        (
            holder.module, "DwmEnableBlurBehindWindow"
        );

        DwmExtendFrameIntoClientArea = (HRESULT (__stdcall*)(HWND, const MARGINS*))::GetProcAddress
        (
            holder.module, "DwmExtendFrameIntoClientArea"
        );
    }
}

//---------------------------------------------------------------------------//

bool __stdcall CLASSNAME::IsAvailable() const
{
    return ( DwmIsCompositionEnabled &&
             DwmEnableBlurBehindWindow &&
             DwmExtendFrameIntoClientArea ) ?
             true : false;
}

//---------------------------------------------------------------------------//

// DWM.cpp