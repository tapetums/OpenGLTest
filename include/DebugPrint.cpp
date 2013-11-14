// DebugPrint.cpp

//---------------------------------------------------------------------------//
//
// デバッグウィンドウへの出力関数
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#if defined(_DEBUG) || defined(DEBUG)

#include <windows.h>
#include <strsafe.h>

#include "DebugPrint.h"

//---------------------------------------------------------------------------//

#define BUFSIZE 1024

//---------------------------------------------------------------------------//

void __stdcall DebugPrint(const wchar_t* format, ...)
{
    wchar_t buf[BUFSIZE];

    va_list al;
    va_start(al, format);
    {
        ::StringCchVPrintfW(buf, BUFSIZE, format, al);
    }
    va_end(al);

    ::OutputDebugStringW(buf);
}

//---------------------------------------------------------------------------//

void __stdcall DebugPrintLn(const wchar_t* format, ...)
{
    static int indent = 0;

    wchar_t spaces[BUFSIZE];
    wchar_t buf[BUFSIZE];

    if ( wcsstr(format, TEXT(" end")) )
    {
        if ( indent < 1 )
        {
            ::OutputDebugStringW(TEXT("\t\tA. F. O.\n"));
        }
        else
        {
            --indent;
        }
    }

    int i = 0;
    for ( ; i < indent; ++i )
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
    ::OutputDebugStringW(buf);

    va_list al;
    va_start(al, format);
    {
        ::StringCchVPrintfW(buf, BUFSIZE, format, al);
    }
    va_end(al);
    ::OutputDebugStringW(buf);

    ::OutputDebugStringW(L"\n");

    if ( wcsstr(format, TEXT(" begin")) )
    {
        ++indent;
    }
}

//---------------------------------------------------------------------------//

#endif // #if defined(_DEBUG) || defined(DEBUG)

//---------------------------------------------------------------------------//

// DebugPrint.cpp