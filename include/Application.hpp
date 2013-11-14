// Application.hpp

#pragma once

//---------------------------------------------------------------------------//
//
// メッセージループをカプセル化するクラス
// （ゲームループ内包）
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#include <stdint.h>

//---------------------------------------------------------------------------//

typedef void (__stdcall* GameFunc)(void* args);

//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS Application

// メッセージループをカプセル化するクラス
class Application
{
public:
    THIS();
    ~THIS();

    int32_t __stdcall Run(uint16_t numerator = 1, uint16_t denominator = 1000);
    void    __stdcall Exit(int32_t nExitCode = 0);
    void    __stdcall PauseGameFunc();
    void    __stdcall ResumeGameFunc();
    void    __stdcall SetGameFunc(GameFunc func, void* args = nullptr);

private:
    bool     m_is_running     = false;
    bool     m_is_game_active = false;
    void*    m_args           = nullptr;
    GameFunc ExecuteGameFunc  = ([](void*){});

private:
    THIS(const THIS&)             = delete;
    THIS(THIS&&)                  = delete;
    THIS& operator= (const THIS&) = delete;
    THIS& operator= (THIS&&)      = delete;
};

#undef THIS

//---------------------------------------------------------------------------//

// アプリケーションクラスの唯一のインスタンス（Application.cppで定義）
extern Application application;

//---------------------------------------------------------------------------//

// Application.hpp