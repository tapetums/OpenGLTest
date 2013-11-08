// GLWnd.h

#pragma once

#include "include\GlassWnd.h"

//---------------------------------------------------------------------------//

struct ITexture;

//---------------------------------------------------------------------------//

class GLWnd : public GlassWnd
{
public:
    GLWnd();
    ~GLWnd();

    LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp) override;

    void __stdcall Update();

private:
    LRESULT __stdcall OnCreate(WPARAM wp, LPARAM lp);
    LRESULT __stdcall OnDestroy(WPARAM wp, LPARAM lp);
    LRESULT __stdcall OnSize(UINT16 fwSizeType, UINT16 w, UINT16 h);
    LRESULT __stdcall OnNcCalcSize(NCCALCSIZE_PARAMS* pncsp);
    LRESULT __stdcall OnNcHitTest(INT32 x, INT32 y);
    LRESULT __stdcall OnKeyDown(UINT16 nVirtKey, LPARAM lp);
    LRESULT __stdcall OnMouseMove(UINT16 fwKeys, INT32 x, INT32 y);
    LRESULT __stdcall OnLButtonDown(UINT16 fwKeys, INT32 x, INT32 y);
    LRESULT __stdcall OnLButtonUp(UINT16 fwKeys, INT32 x, INT32 y);
    LRESULT __stdcall OnLButtonDblClk(UINT16 fwKeys, INT32 x, INT32 y);
    LRESULT __stdcall OnRButtonDown(UINT16 fwKeys, INT32 x, INT32 y);
    LRESULT __stdcall OnRButtonUp(UINT16 fwKeys, INT32 x, INT32 y);
    LRESULT __stdcall OnMButtonDown(UINT16 fwKeys, INT32 x, INT32 y);
    LRESULT __stdcall OnMButtonUp(UINT16 fwKeys, INT32 x, INT32 y);
    LRESULT __stdcall OnMouseWheel(UINT16 fwKeys, INT16 zDelta, INT32 x, INT32 y);
    LRESULT __stdcall OnMouseHWheel(UINT16 fwKeys, INT16 zDelta, INT32 x, INT32 y);
    LRESULT __stdcall OnDropFiles(HDROP hDrop);

private:
    HDC       m_dc   = nullptr;
    HGLRC     m_glrc = nullptr;
    ITexture* m_tex  = nullptr;

    struct Impl;
    Impl* pimpl;
};

//---------------------------------------------------------------------------//

// GLWnd.h