// WinMain.cpp

#include <windows.h>

#include "include\DebugPrint.h"
#include "include\Application.h"
#include "GLWnd.h"

//---------------------------------------------------------------------------//

INT32 WINAPI wWinMain
(
    HINSTANCE hInstance,      // ���݂̃C���X�^���X�̃n���h��
    HINSTANCE hPrevInstance,  // �ȑO�̃C���X�^���X�̃n���h��
    LPWSTR    lpCmdLine,      // �R�}���h���C��
    INT32     nCmdShow        // �\�����
)
{
    DebugPrintLn(TEXT("**********************************************"));

    ::CoInitialize(nullptr);

    DebugPrintLn(TEXT("Creating main window..."));
    GLWnd wnd;
    wnd.Create(TEXT("OpenGLTest"), WS_OVERLAPPEDWINDOW, WS_EX_ACCEPTFILES);
    wnd.Resize(256, 256);
    wnd.ToCenter();
    wnd.Show();
    DebugPrintLn(TEXT("Created main window"));

    //application.SetGameFunc([](void* args){ ((GLWnd*)args)->Update(); }, &wnd);
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