// UWnd.cpp

//---------------------------------------------------------------------------//
//
// ウィンドウの基底クラス
//   Copyright (C) 2005-2013 tapetums
//
//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS UWnd

//---------------------------------------------------------------------------//

#include <windows.h>

#include "DebugPrint.h"
#include "UWnd.hpp"

//---------------------------------------------------------------------------//

void __stdcall ShowLastError(LPCTSTR mbx_title)
{
    LPTSTR lpMsgBuf = nullptr;
    ::FormatMessage
    (
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        ::GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0,
        nullptr
    );
    ::MessageBox(nullptr, lpMsgBuf, mbx_title, MB_OK);
    ::LocalFree(lpMsgBuf);
}

//---------------------------------------------------------------------------//

UWnd::THIS()
{
    m_className = TEXT("UWnd");
    UWnd::Register(m_className);
}

//---------------------------------------------------------------------------//

UWnd::~THIS()
{
    this->Destroy();
}

//---------------------------------------------------------------------------//

bool __stdcall UWnd::IsFullScreen() const
{
    return m_fullscreen;
}

//---------------------------------------------------------------------------//

INT32 __stdcall UWnd::X() const
{
    return m_x;
}

//---------------------------------------------------------------------------//

INT32 __stdcall UWnd::Y() const
{
    return m_y;
}

//---------------------------------------------------------------------------//

INT32 __stdcall UWnd::Width() const
{
    return m_w;
}

//---------------------------------------------------------------------------//

INT32 __stdcall UWnd::Height() const
{
    return m_h;
}

//---------------------------------------------------------------------------//

DWORD __stdcall UWnd::Style() const
{
    return (DWORD)::GetWindowLongPtr(m_hwnd, GWL_STYLE);
}

//---------------------------------------------------------------------------//

DWORD __stdcall UWnd::StyleEx() const
{
    return (DWORD)::GetWindowLongPtr(m_hwnd, GWL_EXSTYLE);
}

//---------------------------------------------------------------------------//

HWND __stdcall UWnd::Handle() const
{
    return m_hwnd;
}

//---------------------------------------------------------------------------//

HWND __stdcall UWnd::Parent() const
{
    return (HWND)::GetWindowLongPtr(m_hwnd, GWLP_HWNDPARENT);
}

//---------------------------------------------------------------------------//

HRESULT __stdcall UWnd::Create
(
    LPCTSTR lpWindowName,
    DWORD   style, 
    DWORD   styleEx,
    HWND    hwndParent,
    HMENU   hMenu
)
{
    // 二重生成防止!
    if ( m_hwnd )
    {
        return S_FALSE;
    }

    // 親ウィンドウを持つ場合はウィンドウスタイルにWS_CHILDを追加
    style |= hwndParent ? WS_CHILD : 0;

    // ウィンドウを生成 … UWnd::StaticWndProc() 内で m_hwnd を格納している
    ::CreateWindowEx
    (
        styleEx, m_className, lpWindowName, style,
        m_x, m_y, m_w, m_h,
        hwndParent, hMenu, ::GetModuleHandle(nullptr), (LPVOID)this
    );
    if ( nullptr == m_hwnd )
    {
        // エラーメッセージの表示
        ShowLastError(m_className);
        return E_FAIL;
    }

    //::UpdateWindow(m_hwnd);

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall UWnd::Destroy()
{
    if ( ::IsWindow(m_hwnd) == FALSE )
    {
        m_hwnd = nullptr;
        return S_FALSE;
    }

    ::DestroyWindow(m_hwnd);
    m_hwnd = nullptr;

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall UWnd::Bounds(INT32 x, INT32 y, INT32 w, INT32 h)
{
    this->AdjustRect(w, h);

    ::SetWindowPos
    (
        m_hwnd, nullptr,
        x, y, w, h,
        SWP_NOZORDER | SWP_FRAMECHANGED
    );

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall UWnd::Hide()
{
    ::ShowWindow(m_hwnd, SW_HIDE);

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall UWnd::Move(INT32 x, INT32 y)
{
    ::SetWindowPos
    (
        m_hwnd, nullptr,
        x, y, 0, 0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
    );

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall UWnd::Refresh(BOOL bErase)
{
    ::InvalidateRect(m_hwnd, nullptr, bErase);

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall UWnd::Resize(INT32 w, INT32 h)
{
    this->AdjustRect(w, h);

    ::SetWindowPos
    (
        m_hwnd, nullptr,
        0, 0, w, h,
        SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED
    );

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall UWnd::Show()
{
    ::ShowWindow(m_hwnd, SW_SHOWNORMAL);
    ::UpdateWindow(m_hwnd);

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall UWnd::ToCenter()
{
    if ( this->Parent() )
    {
        return S_FALSE;
    }

    POINT pt;
    ::GetCursorPos(&pt);
    auto hMonitor = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
    
    MONITORINFO mi = { };
    mi.cbSize = sizeof(mi);
    ::GetMonitorInfo(hMonitor, &mi);
    
    auto sx = mi.rcWork.left;
    auto sy = mi.rcWork.top;
    auto sw = mi.rcWork.right  - mi.rcWork.left;
    auto sh = mi.rcWork.bottom - mi.rcWork.top;

    return this->Move((sw - m_w) / 2 + sx, (sh - m_h) / 2 + sy);
}

//---------------------------------------------------------------------------//

struct MonitorUnderCursor
{
    INT32 x, y, witdh, height;
    WCHAR name[32];

    MonitorUnderCursor()
    {
        POINT pt;
        ::GetCursorPos(&pt);
        auto hMonitor = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);

        MONITORINFOEX miex = { };
        miex.cbSize = sizeof(miex);
        ::GetMonitorInfo(hMonitor, &miex);
        ::CopyMemory(name, miex.szDevice, 32 * sizeof(WCHAR));

        x      = miex.rcMonitor.left;
        y      = miex.rcMonitor.top;
        witdh  = miex.rcMonitor.right  - miex.rcMonitor.left;
        height = miex.rcMonitor.bottom - miex.rcMonitor.top;

        DebugPrintLn(TEXT("%s: (X, Y) = (%d, %d)"), name, x, y);
        DebugPrintLn(TEXT("%s: (Witdh, Height) = (%d, %d)"), name, witdh, height);
    }
};

//---------------------------------------------------------------------------//

HRESULT __stdcall UWnd::ToggleFullScreen(INT32 w, INT32 h)
{
    m_fullscreen = !m_fullscreen;
    DebugPrintLn(TEXT("ToggleFullScreen: %s"), m_fullscreen ? TEXT("true") : TEXT("false"));

    MonitorUnderCursor moniter;

    if ( m_fullscreen )
    {
        auto style = this->Style() | WS_POPUP;
        ::SetWindowLongPtr(m_hwnd, GWL_STYLE, (LONG_PTR)style);

        DEVMODE dm = { };
        dm.dmSize       = sizeof(dm);
        dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT;
        dm.dmPelsWidth  = moniter.witdh;
        dm.dmPelsHeight = moniter.height;
        auto ret = ::ChangeDisplaySettingsEx
        (
            moniter.name, &dm, nullptr, CDS_TEST, 0
        );
        if ( ret != DISP_CHANGE_SUCCESSFUL )
        {
            m_fullscreen = false;
            DebugPrintLn(TEXT("ChangeDisplaySettingsEx(CDS_TEST) failed"));

            return E_FAIL;
        }

        ::ChangeDisplaySettingsEx
        (
            moniter.name, &dm, nullptr, CDS_FULLSCREEN, 0
        );

        ::SetWindowPos
        (
            m_hwnd, nullptr,
            moniter.x, moniter.y, moniter.witdh, moniter.height,
            SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW
        );

        return S_OK;
    }
    else
    {
        ::ChangeDisplaySettingsEx
        (
            moniter.name, nullptr, nullptr, 0, 0
        );

        auto style = this->Style() ^ WS_POPUP;
        ::SetWindowLongPtr(m_hwnd, GWL_STYLE, (LONG_PTR)style);

        this->AdjustRect(w, h);

        ::SetWindowPos
        (
            m_hwnd, nullptr,
            (moniter.witdh  - w) / 2 + moniter.x,
            (moniter.height - h) / 2 + moniter.y,
            w, h,
            SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW
        );

        return S_FALSE;
    }
}

//---------------------------------------------------------------------------//

LRESULT __stdcall UWnd::WndProc(HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp)
{
    if ( uMsg == WM_CLOSE )
    {
        ::PostQuitMessage(0);
        return 0L;
    }
    else
    {
        return ::DefWindowProc(hwnd, uMsg, wp, lp);
    }
}

//---------------------------------------------------------------------------//

void __stdcall UWnd::Register(LPCTSTR lpszClassName)
{
    // ウィンドウクラスを登録
    WNDCLASSEX wc = { };
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc   = UWnd::StaticWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = ::GetModuleHandle(nullptr);
    wc.hIcon         = nullptr;
    wc.hCursor       = ::LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName  = nullptr;
    wc.lpszClassName = lpszClassName;
    wc.hIconSm       = nullptr;

    auto atom = ::RegisterClassEx(&wc);
    if ( atom == 0 )
    {
        if ( GetLastError() == 0x582 )
        {
            // そのクラスは既にあります。
            return;
        }
        else
        {
            // エラーメッセージの表示
            ShowLastError(lpszClassName);
        }
    }
}

//---------------------------------------------------------------------------//

LRESULT __stdcall UWnd::StaticWndProc
(
    HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp
)
{
    UWnd* wnd = nullptr;

    // UWndオブジェクトのポインタを取得
    if ( uMsg == WM_NCCREATE )
    {
        wnd = (UWnd*)((CREATESTRUCT*)lp)->lpCreateParams;

        ::SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)wnd);
    }
    else
    {
        wnd = (UWnd*)::GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    }

    // ウィンドウプロシージャの呼び出し
    if ( nullptr == wnd )
    {
        DebugPrintLn(TEXT("Call DefWindowProc(0x%x)"), uMsg);
        return ::DefWindowProc(hwnd, uMsg, wp, lp);
    }
    else
    {
        // メンバ変数に情報を保存
        switch ( uMsg )
        {
            case WM_CREATE:
            {
                wnd->m_hwnd = hwnd;    // ウィンドウハンドル
                DebugPrintLn(TEXT("Window Handle: 0x%p"), hwnd);
                break;
            }
            case WM_MOVE:
            {
                wnd->m_x = LOWORD(lp); // ウィンドウx座標
                wnd->m_y = HIWORD(lp); // ウィンドウy座標
                DebugPrintLn(TEXT("(X, Y) = (%d, %d)"), wnd->m_x, wnd->m_y);
                break;
            }
            case WM_SIZE:
            {
                wnd->m_w = LOWORD(lp); // ウィンドウ幅
                wnd->m_h = HIWORD(lp); // ウィンドウ高
                DebugPrintLn(TEXT("(Width, Height) = (%d, %d)"), wnd->m_w, wnd->m_h);
                break;
            }
            default:
            {
                break;
            }
        }
        return wnd->WndProc(hwnd, uMsg, wp, lp);
    }
}

//---------------------------------------------------------------------------//

void __stdcall UWnd::AdjustRect(INT32& w, INT32& h) const
{
    DebugPrintLn(TEXT("AdjustRect(%d, %d) begin"), w, h);

    RECT rc = { 0, 0, w, h };
    BOOL  hasMenu = ::GetMenu(m_hwnd) ? TRUE : FALSE;
    DWORD style   = this->Style();
    DWORD styleEx = this->StyleEx();

    ::AdjustWindowRectEx(&rc, style, hasMenu, styleEx);
    w = rc.right  - rc.left;
    h = rc.bottom - rc.top;

    DebugPrintLn(TEXT("AdjustRect(%d, %d) end"), w, h);
}

//---------------------------------------------------------------------------//

#undef THIS

///---------------------------------------------------------------------------//

// UWnd.cpp