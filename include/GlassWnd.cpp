// GlassWnd.cpp

//---------------------------------------------------------------------------//
//
// AeroGlass対応 ウィンドウの基底クラス
//   Copyright (C) 2007-2013 tapetums
//
//---------------------------------------------------------------------------//

#include <windows.h>

#include "DWM.hpp"
#include "UxTheme.hpp"

#include "UWnd.hpp"

//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS GlassWnd
#define BASE UWnd

//---------------------------------------------------------------------------//

GlassWnd::THIS()
{
    dwm     = new DWM;
    uxtheme = new UxTheme;

    m_hBr = ::GetSysColorBrush(COLOR_BTNFACE);

    if ( uxtheme->IsAvailable() )
    {
        uxtheme->BufferedPaintInit();
    }
}

//---------------------------------------------------------------------------//

GlassWnd::~THIS()
{
    if ( m_hBr )
    {
        ::DeleteObject(m_hBr);
        m_hBr = nullptr;
    }

    if ( uxtheme->IsAvailable() )
    {
        if ( m_hTheme )
        {
            uxtheme->CloseThemeData(m_hTheme);
            m_hTheme = nullptr;
        }
        uxtheme->BufferedPaintUnInit();
    }

    delete uxtheme;
    uxtheme = nullptr;

    delete dwm;
    dwm = nullptr;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall GlassWnd::WndProc(HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp)
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
            return BASE::WndProc(hwnd, uMsg, wp, lp);
        }
    }
}

//---------------------------------------------------------------------------//

LRESULT __stdcall GlassWnd::OnDwmCompositionChanged(HWND hwnd)
{
    if ( !dwm->IsAvailable() )
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

LRESULT __stdcall GlassWnd::OnThemeChanged(HWND hwnd)
{
    if ( uxtheme->IsAvailable() )
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

#undef BASE
#undef THIS

//---------------------------------------------------------------------------//

// GlassWnd.cpp