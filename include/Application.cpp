// Application.cpp

#define CLASSNAME Application

//---------------------------------------------------------------------------//

#include <windows.h>

#include "DebugPrint.h"
#include "Application.h"

//---------------------------------------------------------------------------//

Application application;

//---------------------------------------------------------------------------//

struct Timer
{
    int64_t  frequency;
    int64_t  start_time;
    int64_t  next_time;
    uint16_t fps_numerator;
    uint16_t fps_denominator;
    size_t   frame_count;

    Timer()
    {
        this->Rest();
    }

    ~Timer()
    {
    }

    void Rest()
    {
        ::QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
        ::QueryPerformanceCounter((LARGE_INTEGER*)&start_time);

        next_time       = 0;
        fps_numerator   = 1;
        fps_denominator = 1000;
        frame_count     = 0;

        DebugPrintLn(TEXT("PerformanceFrequency: %d"), frequency);
    }

    void SetFPS(uint16_t numerator = 1, uint16_t denominator = 1000)
    {
        fps_numerator   = (numerator > 0) ? numerator : 1;
        fps_denominator = denominator;

        DebugPrintLn(TEXT("Target FPS: %f"), fps_numerator * 1000.0L / fps_denominator);
        DebugPrintLn(TEXT("Time Interval: %fms"), 1.0L * fps_denominator / fps_numerator);
    }

    bool HasTimeCome() const
    {
        int64_t present_time;
        ::QueryPerformanceCounter((LARGE_INTEGER*)&present_time);

        return (present_time >= next_time);
    }

    void SetNextTime()
    {
        DebugPrintLn(TEXT("frame_count: %d"), frame_count);

        int64_t present_time;
        ::QueryPerformanceCounter((LARGE_INTEGER*)&present_time);

        while ( next_time < present_time )
        {
            ++frame_count;
            next_time = start_time +
                        (frame_count * frequency * fps_denominator) /
                        (fps_numerator * 1000);
        }
    }
};

//---------------------------------------------------------------------------//

CLASSNAME::CLASSNAME()
{
}

//---------------------------------------------------------------------------//

CLASSNAME::~CLASSNAME()
{
    this->Exit(0);
}

//---------------------------------------------------------------------------//

int32_t __stdcall CLASSNAME::Run(uint16_t numerator, uint16_t denominator)
{
    MSG msg = {};

    Timer timer;
    timer.SetFPS(numerator, denominator);
    timer.SetNextTime();

    m_is_running = true;
    while ( m_is_running )
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
        else if ( m_is_game_active )
        {
            if ( timer.HasTimeCome() )
            {
                ExecuteGameFunc(m_args);
                timer.SetNextTime();
            }
            else
            {
                ::Sleep(1);
            }
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

void __stdcall CLASSNAME::Exit(int32_t nExitCode)
{
    m_is_running = false;

    ::PostQuitMessage(nExitCode);
}

//---------------------------------------------------------------------------//

void __stdcall CLASSNAME::PauseGameFunc()
{
    m_is_game_active = false;
}

//---------------------------------------------------------------------------//

void __stdcall CLASSNAME::ResumeGameFunc()
{
    m_is_game_active = true;
}

//---------------------------------------------------------------------------//

void __stdcall CLASSNAME::SetGameFunc(GameFunc func, void* args)
{
    if ( nullptr == func )
    {
        return;
    }

    m_args          = args;
    ExecuteGameFunc = func;

    m_is_game_active = true;
}

//---------------------------------------------------------------------------//

// Application.cpp