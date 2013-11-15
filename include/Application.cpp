// Application.cpp

//---------------------------------------------------------------------------//
//
// メッセージループをカプセル化するクラス
// （ゲームループ内包）
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#include <windows.h>

#include "DebugPrint.h"
#include "Application.hpp"

//---------------------------------------------------------------------------//

// アプリケーションクラスの唯一のインスタンス
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
        this->Reset();
    }

    ~Timer()
    {
    }

    void Reset()
    {
        ::QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
        ::QueryPerformanceCounter((LARGE_INTEGER*)&start_time);

        next_time   = 0;
        frame_count = 0;

        console_out(TEXT("PerformanceFrequency: %d"), frequency);
    }

    void SetFPS(uint16_t numerator = 1, uint16_t denominator = 1000)
    {
        fps_numerator   = (numerator > 0) ? numerator : 1;
        fps_denominator = denominator;

        console_out(TEXT("Target FPS: %f"), fps_numerator * 1000.0L / fps_denominator);
        console_out(TEXT("Time Interval: %fms"), 1.0L * fps_denominator / fps_numerator);
    }

    bool HasTimeCome() const
    {
        int64_t present_time;
        ::QueryPerformanceCounter((LARGE_INTEGER*)&present_time);
        //console_out(TEXT("present_time: %d"), present_time);

        return (present_time >= next_time);
    }

    void SetNextTime()
    {
        // 64bit 環境であれば、frame_count は 240fps の場合で
        // 約 2,435,643,939 年後に オーバーフローします。
        // （32bit 環境だと 約 207 日後。）
        if ( frame_count > (1 << (32 - 1)) )
        {
            console_out(TEXT("念のためオーバーフロー防止"));
            this->Reset();
        }
        //console_out(TEXT("frame_count: %u"), frame_count);

        int64_t present_time;
        ::QueryPerformanceCounter((LARGE_INTEGER*)&present_time);

        while ( next_time < present_time )
        {
            ++frame_count;
            next_time = start_time +
                        (frame_count * frequency * fps_denominator) /
                        (fps_numerator * 1000);
        }
        //console_out(TEXT("next_time: %d, present_time: %d"), next_time, present_time);
    }
};

//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS Application

//---------------------------------------------------------------------------//

Application::THIS()
{
}

//---------------------------------------------------------------------------//

Application::~THIS()
{
    this->Exit(0);
}

//---------------------------------------------------------------------------//

int32_t __stdcall Application::Run(uint16_t numerator, uint16_t denominator)
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

void __stdcall Application::Exit(int32_t nExitCode)
{
    m_is_running = false;

    ::PostQuitMessage(nExitCode);
}

//---------------------------------------------------------------------------//

void __stdcall Application::PauseGameFunc()
{
    m_is_game_active = false;
}

//---------------------------------------------------------------------------//

void __stdcall Application::ResumeGameFunc()
{
    m_is_game_active = true;
}

//---------------------------------------------------------------------------//

void __stdcall Application::SetGameFunc(GameFunc func, void* args)
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

#undef THIS

//---------------------------------------------------------------------------//

// Application.cpp