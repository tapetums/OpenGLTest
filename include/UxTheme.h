// UxTheme.h

#pragma once

#include <windows.h>
#include <uxtheme.h>

//---------------------------------------------------------------------------//

extern "C"
{
    typedef HPAINTBUFFER (__stdcall* BEGINBUFFEREDPAINT)(HDC, const RECT*, BP_BUFFERFORMAT, BP_PAINTPARAMS*, HDC*);
    typedef HRESULT      (__stdcall* BUFFEREDPAINTINIT)();
    typedef HRESULT      (__stdcall* BUFFEREDPAINTSETALPHA)(HPAINTBUFFER, RECT*, BYTE);
    typedef HRESULT      (__stdcall* BUFFEREDPAINTUNINIT)();
    typedef HRESULT      (__stdcall* CLOSETHEMEDATA)(HTHEME);
    typedef HRESULT      (__stdcall* DRAWTHEMEBACKGROUND)(HTHEME, HDC, int, int, const RECT*, const RECT*);
    typedef HRESULT      (__stdcall* DRAWTHEMETEXTEX)(HTHEME, HDC, int, int, LPCWSTR, int, DWORD, LPRECT, const DTTOPTS*);
    typedef HRESULT      (__stdcall* ENDBUFFEREDPAINT)(HPAINTBUFFER, BOOL);
    typedef HTHEME       (__stdcall* OPENTHEMEDATA)(HWND, LPCWSTR);
    typedef BOOL         (__stdcall* ISTHEMEACTIVE)();
}

//---------------------------------------------------------------------------//

class UxTheme
{
public:
    UxTheme();
    ~UxTheme() = default;

public:
    BEGINBUFFEREDPAINT    BeginBufferedPaint    = nullptr;
    BUFFEREDPAINTINIT     BufferedPaintInit     = nullptr;
    BUFFEREDPAINTSETALPHA BufferedPaintSetAlpha = nullptr;
    BUFFEREDPAINTUNINIT   BufferedPaintUnInit   = nullptr;
    CLOSETHEMEDATA        CloseThemeData        = nullptr;
    DRAWTHEMETEXTEX       DrawThemeTextEx       = nullptr;
    DRAWTHEMEBACKGROUND   DrawThemeBackground   = nullptr;
    ENDBUFFEREDPAINT      EndBufferedPaint      = nullptr;
    OPENTHEMEDATA         OpenThemeData         = nullptr;
    ISTHEMEACTIVE         IsThemeActive         = nullptr;

    bool __stdcall IsAvailable() const;

private:
    UxTheme(const UxTheme&)             = delete;
    UxTheme(UxTheme&&)                  = delete;
    UxTheme& operator= (const UxTheme&) = delete;
    UxTheme& operator= (UxTheme&&)      = delete;
};

//---------------------------------------------------------------------------//

// UxTheme.h