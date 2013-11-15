// DebugPrint.cpp

//---------------------------------------------------------------------------//
//
// デバッグウィンドウへの出力関数
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//

#if defined(_DEBUG) || defined(DEBUG)

#include <windows.h>
#include <strsafe.h>

#include "DebugPrint.h"

#include "DWM.hpp"
#include "UxTheme.hpp"

//---------------------------------------------------------------------------//

#define BUFSIZE 1024

//---------------------------------------------------------------------------//

struct ConsoleHolder
{
public:
    HANDLE hout   = nullptr;
    int    indent = 0;

    UxTheme uxtheme;

    ConsoleHolder();
    ~ConsoleHolder();
};

static ConsoleHolder console;

//---------------------------------------------------------------------------//

ConsoleHolder::ConsoleHolder()
{
    if ( FALSE == ::AttachConsole(ATTACH_PARENT_PROCESS) )
    {
        ::AllocConsole();
    }

    if ( uxtheme.IsAvailable() )
    {
        uxtheme.BufferedPaintInit();
    }

    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    if ( nullptr == hout )
    {
        return;
    }

    /// 以下はお遊び

    if ( true )
    {
        return;
    }

    WCHAR buf[MAX_PATH];
    ::GetConsoleTitle(buf, MAX_PATH);

    auto hwnd = ::FindWindow(nullptr, buf);
    if ( nullptr == hwnd )
    {
        return;
    }

    DWM dwm;

    BOOL enabled = FALSE;
    dwm.DwmIsCompositionEnabled(&enabled);
    if ( FALSE == enabled )
    {
        return;
    }

    DWM_BLURBEHIND bb = { };
    bb.dwFlags = DWM_BB_ENABLE;
    bb.fEnable = TRUE;
    bb.hRgnBlur = nullptr;
    dwm.DwmEnableBlurBehindWindow(hwnd, &bb);

    MARGINS margins = { -1 };
    dwm.DwmExtendFrameIntoClientArea(hwnd, &margins);

    ::InvalidateRect(hwnd, nullptr, TRUE);
}

//---------------------------------------------------------------------------//

ConsoleHolder::~ConsoleHolder()
{
    if ( uxtheme.IsAvailable() )
    {
        uxtheme.BufferedPaintUnInit();
    }

    #if defined(_DEBUG) || defined(DEBUG)
    if ( hout )
    {
        system("pause");
    }
    #endif

    ::FreeConsole();
}

//---------------------------------------------------------------------------//

void __stdcall console_out(const wchar_t* format, ...)
{
    DWORD cb_w = 0;

    wchar_t spaces[BUFSIZE];
    wchar_t buf[BUFSIZE];

    if ( wcsstr(format, TEXT(" end")) )
    {
        if ( console.indent < 1 )
        {
            auto buf = TEXT("!\n");
            ::WriteConsole(console.hout, buf, ::lstrlen(buf), &cb_w, nullptr);
        }
        else
        {
            --console.indent;
        }
    }

    int i = 0;
    for ( ; i < console.indent; ++i )
    {
        spaces[i*2]     = '.';
        spaces[i*2 + 1] = ' ';
    }
    spaces[i*2] = '\0';

    SYSTEMTIME st;
    ::GetLocalTime(&st);
    ::StringCchPrintf
    (
        buf, BUFSIZE,
        TEXT("%02d:%02d:%02d;%03d> %s"),
        st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
        spaces
    );
    ::WriteConsole(console.hout, buf, ::lstrlen(buf), &cb_w, nullptr);

    va_list al;
    va_start(al, format);
    {
        ::StringCchVPrintfW(buf, BUFSIZE, format, al);
    }
    va_end(al);
    ::WriteConsole(console.hout, buf, ::lstrlen(buf), &cb_w, nullptr);

    ::WriteConsole(console.hout, TEXT("\n"), 1, &cb_w, nullptr);

    if ( wcsstr(format, TEXT(" begin")) )
    {
        ++console.indent;
    }
}

//---------------------------------------------------------------------------//

#endif // #if defined(_DEBUG) || defined(DEBUG)

//---------------------------------------------------------------------------//

// DebugPrint.cpp