// UWnd.h

#pragma once

#include <windows.h>

//---------------------------------------------------------------------------//

class UWnd
{
public:
    UWnd();
    virtual ~UWnd();

    INT32 __stdcall X()       const;
    INT32 __stdcall Y()       const;
    INT32 __stdcall Width()   const;
    INT32 __stdcall Height()  const;
    DWORD __stdcall Style()   const;
    DWORD __stdcall StyleEx() const;
    HWND  __stdcall Handle()  const;
    HWND  __stdcall Parent()  const;

    virtual HRESULT __stdcall Create
    (
        LPCTSTR lpWindowName = nullptr,
        DWORD   style        = WS_OVERLAPPEDWINDOW, 
        DWORD   styleEx      = 0,
        HWND    hwndParent   = nullptr,
        HMENU   hMenu        = nullptr
    );
    virtual HRESULT __stdcall Destroy();
    virtual HRESULT __stdcall Bounds(INT32 x, INT32 y, INT32 w, INT32 h);
    virtual HRESULT __stdcall Hide();
    virtual HRESULT __stdcall Move(INT32 x, INT32 y);
    virtual HRESULT __stdcall Refresh(BOOL bErase = FALSE);
    virtual HRESULT __stdcall Resize(INT32 w, INT32 h);
    virtual HRESULT __stdcall Show();
    virtual HRESULT __stdcall ToCenter();
    virtual LRESULT __stdcall WndProc(HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp);

protected:
    static void    __stdcall Register(LPCTSTR lpszClassName);
    static LRESULT __stdcall StaticWndProc(HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp);

private:
    void __stdcall AdjustRect(INT32& w, INT32& h) const;

protected:
    INT32   m_x         = CW_USEDEFAULT;
    INT32   m_y         = CW_USEDEFAULT;
    INT32   m_w         = CW_USEDEFAULT;
    INT32   m_h         = CW_USEDEFAULT;
    HWND    m_hwnd      = nullptr;
    LPCTSTR m_className = nullptr;

private:
    UWnd(const UWnd&)             = delete;
    UWnd(UWnd&&)                  = delete;
    UWnd& operator= (const UWnd&) = delete;
    UWnd& operator= (UWnd&&)      = delete;
};

//---------------------------------------------------------------------------//

// UWnd.h