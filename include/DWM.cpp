// DWM.cpp

#define CLASSNAME DWM

//---------------------------------------------------------------------------//

#include <windows.h>
#include <dwmapi.h>

#include "DWM.h"

//---------------------------------------------------------------------------//

CLASSNAME::CLASSNAME()
{
    this->Init();
}

//---------------------------------------------------------------------------//

CLASSNAME::~CLASSNAME()
{
    this->Uninit();
}

//---------------------------------------------------------------------------//

void __stdcall CLASSNAME::Init()
{
    m_dwmapi = ::LoadLibraryExW
    (
        TEXT("dwmapi.dll"), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH
    );
    if ( nullptr == m_dwmapi )
    {
        goto FREE_DLL;
    }

    DwmIsCompositionEnabled = (HRESULT (__stdcall*)(BOOL*))::GetProcAddress
    (
        m_dwmapi, "DwmIsCompositionEnabled"
    );
    if ( nullptr == DwmIsCompositionEnabled )
    {
        goto FREE_DLL;
    }

    DwmEnableBlurBehindWindow = (HRESULT (__stdcall*)(HWND, const DWM_BLURBEHIND*))::GetProcAddress
    (
        m_dwmapi, "DwmEnableBlurBehindWindow"
    );
    if ( nullptr == DwmEnableBlurBehindWindow )
    {
        goto FREE_DLL;
    }

    DwmExtendFrameIntoClientArea = (HRESULT (__stdcall*)(HWND, const MARGINS*))::GetProcAddress
    (
        m_dwmapi, "DwmExtendFrameIntoClientArea"
    );
    if ( nullptr == DwmExtendFrameIntoClientArea )
    {
        goto FREE_DLL;
    }

    return;

FREE_DLL:
    this->Uninit();

    return;
}

//---------------------------------------------------------------------------//

void __stdcall CLASSNAME::Uninit()
{
    if ( m_dwmapi )
    {
        ::FreeLibrary(m_dwmapi);
        m_dwmapi = nullptr;
    }

    DwmIsCompositionEnabled      = nullptr;
    DwmEnableBlurBehindWindow    = nullptr;
    DwmExtendFrameIntoClientArea = nullptr;
}

//---------------------------------------------------------------------------//

// DWM.cpp