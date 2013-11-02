// Application.h

#pragma once

typedef __int32 int32_t;

//---------------------------------------------------------------------------//

typedef void (__stdcall* GameFunc)(void* args);

//---------------------------------------------------------------------------//

class Application
{
public:
    Application();
    ~Application();

    int32_t __stdcall Run();
    void    __stdcall Exit(int32_t nExitCode = 0);
    void    __stdcall Pause();
    void    __stdcall Resume();
    void    __stdcall RegisterGameFunc(GameFunc func, void* args);

private:
    bool     m_is_game_active = false;
    GameFunc m_game_function  = nullptr;
    void*    m_gf_args        = nullptr;

private:
    Application(const Application&)             = delete;
    Application(Application&&)                  = delete;
    Application& operator= (const Application&) = delete;
    Application& operator= (Application&&)      = delete;
};

//---------------------------------------------------------------------------//

// Application.h