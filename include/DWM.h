// DWM.h

#pragma once

#include <windows.h>

//---------------------------------------------------------------------------//

class DWM
{
public:
    DWM();
    ~DWM();

public:
    HRESULT (__stdcall* DwmIsCompositionEnabled)(BOOL*) = nullptr;
    HRESULT (__stdcall* DwmEnableBlurBehindWindow)(HWND, const DWM_BLURBEHIND*) = nullptr;
    HRESULT (__stdcall* DwmExtendFrameIntoClientArea)(HWND, const MARGINS*) = nullptr;

private:
    void __stdcall Init();
    void __stdcall Uninit();

private:
    HMODULE m_dwmapi = nullptr;

private:
    DWM(const DWM&)             = delete;
    DWM(DWM&&)                  = delete;
    DWM& operator= (const DWM&) = delete;
    DWM& operator= (DWM&&)      = delete;
};

//---------------------------------------------------------------------------//

// DWM.h