// GLWnd.h

#pragma once

#include "include\GlassWnd.h"

//---------------------------------------------------------------------------//

interface ITexture;

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
    LRESULT __stdcall OnSize(INT32 fwSizeType, INT16 w, INT16 h);
    LRESULT __stdcall OnLButtonDown(INT32 fwKeys, INT16 x, INT16 y);
    LRESULT __stdcall OnRButtonDown(INT32 fwKeys, INT16 x, INT16 y);

private:
    HDC       m_dc   = nullptr;
    HGLRC     m_glrc = nullptr;
    ITexture* m_tex  = nullptr;

    struct Impl;
    Impl* pimpl;
};

//---------------------------------------------------------------------------//

// GLWnd.h