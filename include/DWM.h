// DWM.h

#pragma once

#include <windows.h>
#include <dwmapi.h>

//---------------------------------------------------------------------------//

class DWM
{
public:
    DWM();
    ~DWM() = default;

public:
    HRESULT (__stdcall* DwmIsCompositionEnabled)(BOOL*) = nullptr;
    HRESULT (__stdcall* DwmEnableBlurBehindWindow)(HWND, const DWM_BLURBEHIND*) = nullptr;
    HRESULT (__stdcall* DwmExtendFrameIntoClientArea)(HWND, const MARGINS*) = nullptr;

    bool __stdcall IsAvailable() const;

private:
    DWM(const DWM&)             = delete;
    DWM(DWM&&)                  = delete;
    DWM& operator= (const DWM&) = delete;
    DWM& operator= (DWM&&)      = delete;
};

//---------------------------------------------------------------------------//

// DWM.h