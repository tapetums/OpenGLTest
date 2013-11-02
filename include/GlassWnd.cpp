// GlassWnd.cpp

#define CLASSNAME GlassWnd

//---------------------------------------------------------------------------//

#include <windows.h>
#include <uxtheme.h>
#include <dwmapi.h>

#include "DWM.h"
#include "Theme.h"

#include "GlassWnd.h"

//---------------------------------------------------------------------------//

CLASSNAME::CLASSNAME()
{
    dwm = new DWM;

    m_hBr = ::GetSysColorBrush(COLOR_BTNFACE);

    auto uxtheme = UxTheme::GetInstance();
    if ( uxtheme )
    {
        uxtheme->BufferedPaintInit();
    }
}

//---------------------------------------------------------------------------//

CLASSNAME::~CLASSNAME()
{
    if ( m_hBr )
    {
        ::DeleteObject(m_hBr);
        m_hBr = nullptr;
    }

    auto uxtheme = UxTheme::GetInstance();
    if ( uxtheme )
    {
        if ( m_hTheme )
        {
            uxtheme->CloseThemeData(m_hTheme);
            m_hTheme = nullptr;
        }
        uxtheme->BufferedPaintUnInit();
    }

    delete dwm;
    dwm = nullptr;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall CLASSNAME::WndProc(HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp)
{
    switch (uMsg)
    {
        case WM_DWMCOMPOSITIONCHANGED:
        {
            return this->OnDwmCompositionChanged(hwnd);
        }
        case WM_THEMECHANGED:
        {
            return this->OnThemeChanged(hwnd);
        }
        default:
        {
            return UWnd::WndProc(hwnd, uMsg, wp, lp);
        }
    }
}

//---------------------------------------------------------------------------//

LRESULT __stdcall CLASSNAME::OnDwmCompositionChanged(HWND hwnd)
{
    if ( nullptr == dwm->DwmIsCompositionEnabled )
    {
        return -1L;
    }

    if ( m_hBr )
    {
        ::DeleteObject(m_hBr);
        m_hBr = nullptr;
    }

    dwm->DwmIsCompositionEnabled(&m_compEnabled);
    if ( !m_compEnabled )
    {
        m_hBr = ::GetSysColorBrush(COLOR_BTNFACE);
    }
    else
    {
        m_hBr = ::CreateSolidBrush(0);

        DWM_BLURBEHIND bb = { };
        bb.dwFlags = DWM_BB_ENABLE;
        bb.fEnable = TRUE;
        bb.hRgnBlur = nullptr;
        dwm->DwmEnableBlurBehindWindow(hwnd, &bb);

        MARGINS margins = { -1 };
        dwm->DwmExtendFrameIntoClientArea(hwnd, &margins);
    }

    ::InvalidateRect(hwnd, nullptr, TRUE);

    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall CLASSNAME::OnThemeChanged(HWND hwnd)
{
    auto uxtheme = UxTheme::GetInstance();
    if ( uxtheme )
    {
        if ( m_hTheme )
        {
            uxtheme->CloseThemeData(m_hTheme);
            m_hTheme = nullptr;
        }
        if ( uxtheme->IsThemeActive() )
        {
            m_hTheme = uxtheme->OpenThemeData(hwnd, TEXT("BUTTON"));
        }
    }

    return OnDwmCompositionChanged(hwnd);
}

//---------------------------------------------------------------------------//

// GlassWnd.cpp