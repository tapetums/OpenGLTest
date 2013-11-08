// GlassWnd.h

#pragma once

#include "UWnd.h"

//---------------------------------------------------------------------------//

class DWM;
class UxTheme;

//---------------------------------------------------------------------------//

class GlassWnd : public UWnd
{
public:
    GlassWnd();
    ~GlassWnd();

    LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp) override;

protected:
    LRESULT __stdcall OnDwmCompositionChanged(HWND hwnd);
    LRESULT __stdcall OnThemeChanged(HWND hwnd);

protected:
    DWM*     dwm           = nullptr;
    UxTheme* uxtheme       = nullptr;
    HBRUSH   m_hBr         = nullptr;
    HANDLE   m_hTheme      = nullptr;
    BOOL     m_compEnabled = FALSE;
};

//---------------------------------------------------------------------------//

// GlassWnd.h