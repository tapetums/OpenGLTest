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

extern const CLSID CLSID_Component = GUID_NULL;

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

    console_out(TEXT("CubeMelon.exe"));
    console_out(TEXT("**********************************************"));

    ::CoInitialize(nullptr);

    console_out(TEXT("Creating main window begin"));
    OpenGLWnd wnd;
    wnd.Create(TEXT("OpenGLTest"), WS_OVERLAPPEDWINDOW, WS_EX_ACCEPTFILES);
    wnd.Resize(256, 256);
    wnd.ToCenter();
    wnd.Show();
    console_out(TEXT("Creating main window end"));

    //application.SetGameFunc([](void* args){ ((OpenGLWnd*)args)->Update(); }, &wnd);
    //application.PauseGameFunc();

    INT32 ret;
    console_out(TEXT("---------------- Message Loop ----------------"));
    {
        ret = application.Run(30, 1001);
    }
    console_out(TEXT("---------------- Message Loop ----------------"));

    ::CoUninitialize();

    console_out(TEXT("CubeMelon.exe\n"));

    return ret;
}

//---------------------------------------------------------------------------//

// WinMain.cpp