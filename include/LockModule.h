﻿// LockModule.h

#pragma once

#include <windows.h>

//---------------------------------------------------------------------------//

extern ULONG g_cLocks;

//---------------------------------------------------------------------------//

void LockModule();
void UnlockModule();

//---------------------------------------------------------------------------//

// LockModule.h