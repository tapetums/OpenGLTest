// UxTheme.cpp

#define CLASSNAME UxTheme

//---------------------------------------------------------------------------//

#include "UxTheme.h"

//---------------------------------------------------------------------------//

struct UxThemeHolder
{
    HMODULE module;

    UxThemeHolder()
    {
        module = ::LoadLibraryExW
        (
            TEXT("uxtheme.dll"), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH
        );
    }

    ~UxThemeHolder()
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
    static UxThemeHolder holder;

    if ( holder.module )
    {
        BeginBufferedPaint = (BEGINBUFFEREDPAINT)::GetProcAddress
        (
            holder.module, "BeginBufferedPaint"
        );

        BufferedPaintInit = (BUFFEREDPAINTINIT)::GetProcAddress
        (
            holder.module, "BufferedPaintInit"
        );

        BufferedPaintSetAlpha = (BUFFEREDPAINTSETALPHA)::GetProcAddress
        (
            holder.module, "BufferedPaintSetAlpha"
        );

        BufferedPaintUnInit = (BUFFEREDPAINTUNINIT)::GetProcAddress
        (
            holder.module, "BufferedPaintUnInit"
        );

        CloseThemeData = (CLOSETHEMEDATA)::GetProcAddress
        (
            holder.module, "CloseThemeData"
        );

        DrawThemeBackground = (DRAWTHEMEBACKGROUND)::GetProcAddress
        (
            holder.module, "DrawThemeBackground"
        );

        DrawThemeTextEx = (DRAWTHEMETEXTEX)::GetProcAddress
        (
            holder.module, "DrawThemeTextEx"
        );

        EndBufferedPaint = (ENDBUFFEREDPAINT)::GetProcAddress
        (
            holder.module, "EndBufferedPaint"
        );

        OpenThemeData = (OPENTHEMEDATA)::GetProcAddress
        (
            holder.module, "OpenThemeData"
        );

        IsThemeActive = (ISTHEMEACTIVE)::GetProcAddress
        (
            holder.module, "IsThemeActive"
        );
    }
}

//---------------------------------------------------------------------------//

bool __stdcall CLASSNAME::IsAvailable() const
{
    return ( BufferedPaintInit && BufferedPaintUnInit ) ? true : false;
}

//---------------------------------------------------------------------------//

// UxTheme.cpp