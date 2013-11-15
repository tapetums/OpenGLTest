// DebugPrint.h

#pragma once

//---------------------------------------------------------------------------//
//
// デバッグウィンドウへの出力関数
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#if defined(_DEBUG) || defined(DEBUG)

// Debugのとき
void __stdcall console_out(const wchar_t* format, ...);

#else

// Releaseのとき
#define console_out(x, ...)

#endif

//---------------------------------------------------------------------------//

// DebugPrint.h