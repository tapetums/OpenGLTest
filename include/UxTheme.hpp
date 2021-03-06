// UxTheme.hpp

#pragma once

//---------------------------------------------------------------------------//
//
// Visual Style API の ラッパークラス
//   Copyright (C) 2007-2013 tapetums
//
//---------------------------------------------------------------------------//

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

#ifdef THIS
#undef THIS
#endif

#define THIS UxTheme

// Visual Style API の ラッパークラス
class UxTheme
{
public:
    THIS();
    ~THIS() = default;

public:
    bool __stdcall IsAvailable() const;

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

private:
    THIS(const THIS&)             = delete;
    THIS(THIS&&)                  = delete;
    THIS& operator= (const THIS&) = delete;
    THIS& operator= (THIS&&)      = delete;
};

#undef THIS

//---------------------------------------------------------------------------//

// UxTheme.hpp