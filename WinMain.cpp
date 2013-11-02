// WinMain.cpp

#include <windows.h>

#include "include\DebugPrint.h"
#include "include\Application.h"
#include "GLWnd.h"

//---------------------------------------------------------------------------//

INT32 WINAPI wWinMain
(
    HINSTANCE hInstance,      // 現在のインスタンスのハンドル
    HINSTANCE hPrevInstance,  // 以前のインスタンスのハンドル
    LPWSTR    lpCmdLine,      // コマンドライン
    INT32     nCmdShow        // 表示状態
)
{
    ::CoInitialize(nullptr);

    GLWnd wnd;
    wnd.Create(TEXT("OpenGLTest"));
    wnd.Resize(256, 256);
    wnd.ToCenter();
    wnd.Show();

    Application app;
    app.RegisterGameFunc([](void*args){ ((GLWnd*)args)->Update();}, &wnd);
    app.Run();

    ::CoUninitialize();

    return 0;
}

//---------------------------------------------------------------------------//

// WinMain.cpp