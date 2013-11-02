// Application.cpp

#include <windows.h>

#include "Application.h"

//---------------------------------------------------------------------------//

Application::Application()
{
}

//---------------------------------------------------------------------------//

Application::~Application()
{
    this->Exit(0);
}

//---------------------------------------------------------------------------//

int32_t __stdcall Application::Run()
{
    MSG msg = {};

    while ( true )
    {
        if ( ::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE) )
        {
            if ( ::GetMessage(&msg, nullptr, 0, 0) < 1 )
            {
                break;
            }
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else if ( m_is_game_active && m_game_function )
        {
            m_game_function(m_gf_args);
            ::Sleep(1);
        }
        else
        {
            ::MsgWaitForMultipleObjects
            (
                0, nullptr, FALSE, INFINITE, QS_ALLINPUT
            );
        }
    }

    return static_cast<int32_t>(msg.wParam);
}

//---------------------------------------------------------------------------//

void __stdcall Application::Exit(int32_t nExitCode)
{
    ::PostQuitMessage(nExitCode);
}

//---------------------------------------------------------------------------//

void __stdcall Application::Pause()
{
    m_is_game_active = false;
}

//---------------------------------------------------------------------------//

void __stdcall Application::Resume()
{
    m_is_game_active = true;
}

//---------------------------------------------------------------------------//

void __stdcall Application::RegisterGameFunc(GameFunc func, void* args)
{
    m_game_function = func;
    m_gf_args       = args;

    m_is_game_active = true;
}

//---------------------------------------------------------------------------//

// Application.cpp