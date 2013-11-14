// WinMain.cpp

///---------------------------------------------------------------------------//
//
// アプリケーション エントリポイント
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#include <windows.h>

#include "include\DebugPrint.h"
#include "include\Application.hpp"

#include "OpenGLWnd.hpp"

//---------------------------------------------------------------------------//

#if defined(_DEBUG) || (DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//---------------------------------------------------------------------------//

INT32 WINAPI wWinMain
(
    HINSTANCE hInstance,      // 現在のインスタンスのハンドル
    HINSTANCE hPrevInstance,  // 以前のインスタンスのハンドル
    LPWSTR    lpCmdLine,      // コマンドライン
    INT32     nCmdShow        // 表示状態
)
{
#if _DEBUG
    ::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

    DebugPrintLn(TEXT("**********************************************"));

    ::CoInitialize(nullptr);

    DebugPrintLn(TEXT("Creating main window..."));
    OpenGLWnd wnd;
    wnd.Create(TEXT("OpenGLTest"), WS_OVERLAPPEDWINDOW, WS_EX_ACCEPTFILES);
    wnd.Resize(256, 256);
    wnd.ToCenter();
    wnd.Show();
    DebugPrintLn(TEXT("Created main window"));

    //application.SetGameFunc([](void* args){ ((OpenGLWnd*)args)->Update(); }, &wnd);
    //application.PauseGameFunc();

    DebugPrintLn(TEXT("---------------- Message Loop ----------------"));
    auto ret = application.Run(30, 1001);
    DebugPrintLn(TEXT("---------------- Message Loop ----------------"));

    ::CoUninitialize();

    DebugPrintLn(TEXT("**********************************************"));

    return ret;
}

//---------------------------------------------------------------------------//

// WinMain.cpp