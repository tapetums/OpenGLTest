// Theme.cpp

#include <windows.h>
#include <uxtheme.h>

#include "Theme.h"

//---------------------------------------------------------------------------//

UxTheme* __stdcall UxTheme::GetInstance()
{
    static UxTheme ut;

    return ut.m_uxtheme ? &ut : nullptr;
}

//---------------------------------------------------------------------------//

HPAINTBUFFER __stdcall UxTheme::BeginBufferedPaint
(
    HDC hdcTarget,
    const RECT* prcTarget,
    BP_BUFFERFORMAT dwFormat,
    BP_PAINTPARAMS* pPaintParams,
    HDC* phdc
)
{
    return m_BeginBufferedPaint(hdcTarget, prcTarget, dwFormat, pPaintParams, phdc);
}

//---------------------------------------------------------------------------//

HRESULT __stdcall UxTheme::BufferedPaintInit()
{
    return m_BufferedPaintInit();
};

//---------------------------------------------------------------------------//

HRESULT __stdcall UxTheme::BufferedPaintSetAlpha
(
    HPAINTBUFFER hBufferedPaint, RECT* prc, BYTE alpha
)
{
    return m_BufferedPaintSetAlpha(hBufferedPaint, prc, alpha);
};

//---------------------------------------------------------------------------//

HRESULT __stdcall UxTheme::BufferedPaintUnInit()
{
    return m_BufferedPaintUnInit();
};

//---------------------------------------------------------------------------//

HRESULT __stdcall UxTheme::CloseThemeData(HTHEME hTheme)
{
    return m_CloseThemeData(hTheme);
};

//---------------------------------------------------------------------------//

HRESULT __stdcall UxTheme::DrawThemeBackground
(
    HTHEME hTheme,
    HDC hdc,
    int iPartId,
    int iStateId,
    const RECT* pRect,
    const RECT* pClipRect
)
{
    return m_DrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}

//---------------------------------------------------------------------------//

HRESULT __stdcall UxTheme::DrawThemeTextEx
(
    HTHEME hTheme,
    HDC hDC,
    LPCTSTR text,
    DWORD dwFlags,
    RECT* pRect,
    const DTTOPTS* dttOpt
)
{
    return m_DrawThemeTextEx
    (
        hTheme, hDC, 0, 0,
        text, -1,
        dwFlags,
        pRect, dttOpt
    );
}

//---------------------------------------------------------------------------//

HRESULT __stdcall UxTheme::EndBufferedPaint
(
    HPAINTBUFFER hBufferedPaint, BOOL fUpdateTarget
)
{
    return m_EndBufferedPaint(hBufferedPaint, fUpdateTarget);
}

//---------------------------------------------------------------------------//

HTHEME __stdcall UxTheme::OpenThemeData(HWND hwnd, LPCWSTR pszClassList)
{
    return m_OpenThemeData(hwnd, pszClassList);
};

//---------------------------------------------------------------------------//

BOOL __stdcall UxTheme::IsThemeActive()
{
    return m_IsThemeActive();
};

//---------------------------------------------------------------------------//

UxTheme::UxTheme()
{
    try
    {
        m_uxtheme = ::LoadLibraryEx(TEXT("uxtheme.dll"), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
        if ( nullptr == m_uxtheme )
        {
            throw m_uxtheme;
        }

        m_BeginBufferedPaint = (BEGINBUFFEREDPAINT)::GetProcAddress
        (
            m_uxtheme, "BeginBufferedPaint"
        );
        if ( nullptr == m_BeginBufferedPaint )
        {
            throw m_uxtheme;
        }

        m_BufferedPaintInit = (BUFFEREDPAINTINIT)::GetProcAddress
        (
            m_uxtheme, "BufferedPaintInit"
        );
        if ( nullptr == m_BufferedPaintInit )
        {
            throw m_uxtheme;
        }

        m_BufferedPaintSetAlpha = (BUFFEREDPAINTSETALPHA)::GetProcAddress
        (
            m_uxtheme, "BufferedPaintSetAlpha"
        );
        if ( nullptr == m_BufferedPaintSetAlpha )
        {
            throw m_uxtheme;
        }

        m_BufferedPaintUnInit = (BUFFEREDPAINTUNINIT)::GetProcAddress
        (
            m_uxtheme, "BufferedPaintUnInit"
        );
        if ( nullptr == m_BufferedPaintUnInit )
        {
            throw m_uxtheme;
        }

        m_CloseThemeData = (CLOSETHEMEDATA)::GetProcAddress
        (
            m_uxtheme, "CloseThemeData"
        );
        if ( nullptr == m_CloseThemeData )
        {
            throw m_uxtheme;
        }

        m_DrawThemeBackground = (DRAWTHEMEBACKGROUND)::GetProcAddress
        (
            m_uxtheme, "DrawThemeBackground"
        );
        if ( nullptr == m_DrawThemeBackground )
        {
            throw m_uxtheme;
        }

        m_DrawThemeTextEx = (DRAWTHEMETEXTEX)::GetProcAddress
        (
            m_uxtheme, "DrawThemeTextEx"
        );
        if ( nullptr == m_DrawThemeTextEx )
        {
            throw m_uxtheme;
        }

        m_EndBufferedPaint = (ENDBUFFEREDPAINT)::GetProcAddress
        (
            m_uxtheme, "EndBufferedPaint"
        );
        if ( nullptr == m_EndBufferedPaint )
        {
            throw m_uxtheme;
        }

        m_OpenThemeData = (OPENTHEMEDATA)::GetProcAddress
        (
            m_uxtheme, "OpenThemeData"
        );
        if ( nullptr == m_OpenThemeData )
        {
            throw m_uxtheme;
        }

        m_IsThemeActive = (ISTHEMEACTIVE)::GetProcAddress
        (
            m_uxtheme, "IsThemeActive"
        );
        if ( nullptr == m_IsThemeActive )
        {
            throw m_uxtheme;
        }
    }
    catch (...)
    {
        if ( m_uxtheme )
        {
            ::FreeLibrary(m_uxtheme);
            m_uxtheme = nullptr;
        }
    }
}

//---------------------------------------------------------------------------//

UxTheme::~UxTheme()
{
    if ( m_uxtheme )
    {
        ::FreeLibrary(m_uxtheme);
        m_uxtheme = nullptr;
    }
}

// Theme.cpp