// DebugPrint.h

#pragma once

//---------------------------------------------------------------------------//

#if defined(_DEBUG) || defined(DEBUG) // Debugのとき

void __stdcall DebugPrint(const wchar_t* format, ...);
void __stdcall DebugPrintLn(const wchar_t* format, ...);

#else // Releaseのとき

#define DebugPrint(x, ...)
#define DebugPrintLn(x, ...)

#endif

//---------------------------------------------------------------------------//

// DebugPrint.h