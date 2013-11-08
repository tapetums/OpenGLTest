// Application.h

#pragma once

#include <stdint.h>

//---------------------------------------------------------------------------//

typedef void (__stdcall* GameFunc)(void* args);

//---------------------------------------------------------------------------//

class Application
{
public:
    Application();
    ~Application();

    int32_t __stdcall Run(uint16_t numerator = 1, uint16_t denominator = 1000);
    void    __stdcall Exit(int32_t nExitCode = 0);
    void    __stdcall PauseGameFunc();
    void    __stdcall ResumeGameFunc();
    void    __stdcall SetGameFunc(GameFunc func, void* args = nullptr);

private:
    bool     m_is_running      = false;
    bool     m_is_game_active  = false;
    void*    m_args            = nullptr;
    GameFunc ExecuteGameFunc   = ([](void*){}); // __stdcall

private:
    Application(const Application&)             = delete;
    Application(Application&&)                  = delete;
    Application& operator= (const Application&) = delete;
    Application& operator= (Application&&)      = delete;
};

//---------------------------------------------------------------------------//

extern Application application;

//---------------------------------------------------------------------------//

// Application.h