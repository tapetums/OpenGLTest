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