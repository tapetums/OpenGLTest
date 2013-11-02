// Theme.h

#pragma once

#include <windows.h>

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
    static UxTheme* __stdcall GetInstance();

public:
    HPAINTBUFFER __stdcall BeginBufferedPaint(HDC hdcTarget, const RECT* prcTarget, BP_BUFFERFORMAT dwFormat, BP_PAINTPARAMS* pPaintParams, HDC* phdc);
    HRESULT      __stdcall BufferedPaintInit();
    HRESULT      __stdcall BufferedPaintSetAlpha(HPAINTBUFFER hBufferedPaint, RECT* prc, BYTE alpha);
    HRESULT      __stdcall BufferedPaintUnInit();
    HRESULT      __stdcall CloseThemeData(HTHEME hTheme);
    HRESULT      __stdcall DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, const RECT* pClipRect);
    HRESULT      __stdcall DrawThemeTextEx(HTHEME hTheme, HDC hDC, LPCTSTR text, DWORD dwFlags, RECT* pRect, const DTTOPTS* dttOpt);
    HRESULT      __stdcall EndBufferedPaint(HPAINTBUFFER hBufferedPaint, BOOL fUpdateTarget);
    HTHEME       __stdcall OpenThemeData(HWND hwnd, LPCWSTR pszClassList);
    BOOL         __stdcall IsThemeActive();

private:
    HMODULE m_uxtheme;

    BEGINBUFFEREDPAINT    m_BeginBufferedPaint;
    BUFFEREDPAINTINIT     m_BufferedPaintInit;
    BUFFEREDPAINTSETALPHA m_BufferedPaintSetAlpha;
    BUFFEREDPAINTUNINIT   m_BufferedPaintUnInit;
    CLOSETHEMEDATA        m_CloseThemeData;
    DRAWTHEMETEXTEX       m_DrawThemeTextEx;
    DRAWTHEMEBACKGROUND   m_DrawThemeBackground;
    ENDBUFFEREDPAINT      m_EndBufferedPaint;
    OPENTHEMEDATA         m_OpenThemeData;
    ISTHEMEACTIVE         m_IsThemeActive;

private:
    UxTheme();
    ~UxTheme();

    UxTheme(const UxTheme&);
    UxTheme(UxTheme&&);
    UxTheme& operator= (const UxTheme&);
    UxTheme& operator= (UxTheme&&);
};

// Theme.h