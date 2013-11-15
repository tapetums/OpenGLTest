// OpenGLWnd.cpp

///---------------------------------------------------------------------------//
//
// OpenGL で描画するウィンドウのクラス
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#include <windows.h>
#include <windowsx.h>

#include <wincodec.h>
#include <wincodecsdk.h>
#pragma comment(lib, "windowscodecs.lib")

#include <gl/gl.h>
#include <gl/glext.h>
#pragma comment(lib, "opengl32.lib")

#include "include\DebugPrint.h"
#include "include\ComPtr.h"

#include "OpenGLTexture.hpp"
#include "OpenGLWnd.hpp"

//---------------------------------------------------------------------------//

static const PIXELFORMATDESCRIPTOR pfd =
{
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0, 0, 0, 0,
    24,
    8,
    0,
    PFD_MAIN_PLANE,
    0,
    0, 0, 0
};

//---------------------------------------------------------------------------//

ITexture* MakeTexture()
{
    static const TextureDesc desc =
    {
        PixelFormat::RGBA8888, 4, 4, 0, 0,
        GL_NEAREST, GL_NEAREST, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT,
        GL_REPLACE
    };

    static const uint8_t buffer[] =
    {
        255, 255, 255, 255,    0,   0,   0, 255,
        255, 255, 255, 255,    0,   0,   0, 255,
        255,   0,   0, 255,    0, 255,   0, 255,
          0,   0, 255, 255,  255, 255, 255, 255,
        128,   0,   0, 255,    0, 128,   0, 255,
          0,   0, 128, 255,  128, 128, 128, 255,
        255, 255,   0, 255,  255,   0, 255, 255,
          0, 255, 255, 255,  255, 255, 255, 255,
    };

    static const size_t buf_size = sizeof(uint8_t) * sizeof(buffer);

    return new OpenGLTexture(&desc, buf_size, buffer);
}

//---------------------------------------------------------------------------//

ITexture* LoadTexture(LPCWSTR fileName)
{
    HRESULT hr;

    ComPtr<IWICImagingFactory> factory;
    hr = ::CoCreateInstance
    (
        CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory, (void**)&factory
    );
    if ( nullptr == factory.GetInterface() )
    {
        console_out(TEXT("CoCreateInstance() failed: 0x%08x"), hr);
        return nullptr;
    }

    ComPtr<IWICBitmapDecoder> decoder;
    hr = factory->CreateDecoderFromFilename
    (
        fileName, nullptr, GENERIC_READ,
        WICDecodeMetadataCacheOnLoad, &decoder
    );
    if ( nullptr == decoder.GetInterface() )
    {
        console_out(TEXT("CreateDecoderFromFilename() failed: 0x%08x"), hr);
        return nullptr;
    }

    uint32_t frameCount = 0;
    hr = decoder->GetFrameCount(&frameCount);
    console_out(TEXT("frameCount: %d"), frameCount);
    if ( 0 == frameCount )
    {
        console_out(TEXT("GetFrameCount() failed: 0x%08x"), hr);
        return nullptr;
    }

    ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, &frame);
    if ( nullptr == frame.GetInterface() )
    {
        console_out(TEXT("GetFrame() failed: 0x%08x"), hr);
        return nullptr;
    }

    TextureDesc desc =
    {
        PixelFormat::UNKNOWN, 0, 0, 0, 0,
        GL_LINEAR, GL_LINEAR, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT,
        GL_DECAL
    };
    frame->GetSize((UINT*)&desc.width, (UINT*)&desc.height);
    console_out(TEXT("width: %d, height: %d"), desc.width, desc.height);

    frame->GetResolution(&desc.dpiX, &desc.dpiY);
    console_out(TEXT("DPI(X, Y) = (%d, %d)"), desc.dpiX, desc.dpiY);

    GUID format_id = GUID_NULL;
    uint32_t bitCount = 0;
    hr = frame->GetPixelFormat(&format_id);
    console_out
    (
        TEXT("GetPixelFormat(): 0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x"),
        format_id.Data1, format_id.Data2, format_id.Data3,
        format_id.Data4[0], format_id.Data4[1], format_id.Data4[2], format_id.Data4[3],
        format_id.Data4[4], format_id.Data4[5], format_id.Data4[6], format_id.Data4[7]
    );
    if ( IsEqualGUID(format_id, GUID_WICPixelFormat8bppIndexed) )
    {
        bitCount = 8;
        desc.format = PixelFormat::INDEX8;
    }
    else if ( IsEqualGUID(format_id, GUID_WICPixelFormat24bppRGB) )
    {
        bitCount = 24;
        desc.format = PixelFormat::RGB888;
    }
    else if ( IsEqualGUID(format_id, GUID_WICPixelFormat24bppBGR) )
    {
        bitCount = 24;
        desc.format = PixelFormat::BGR888;
    }
    else if ( IsEqualGUID(format_id, GUID_WICPixelFormat32bppRGBA) ||
              IsEqualGUID(format_id, GUID_WICPixelFormat32bppRGB) )
    {
        bitCount = 32;
        desc.format = PixelFormat::RGBA8888;
    }
    else if ( IsEqualGUID(format_id, GUID_WICPixelFormat32bppBGRA) ||
              IsEqualGUID(format_id, GUID_WICPixelFormat32bppBGR) )
    {
        bitCount = 32;
        desc.format = PixelFormat::BGRA8888;
    }
    else
    {
        console_out(TEXT("GetPixelFormat() failed: E_FAIL"));
        return nullptr;
    }
    console_out(TEXT("bitCount: %d"), bitCount);
    console_out(TEXT("PixelFormat: %d"), desc.format);

    auto stride = (desc.width * (bitCount / 8) + 3) & ~3;
    console_out(TEXT("stride: %d"), stride);

    size_t length = stride * desc.height;
    size_t buf_size = sizeof(uint8_t) * length;
    auto buffer = new uint8_t[length];
    hr = frame->CopyPixels(0, stride, static_cast<UINT>(buf_size), buffer);

    auto texture = new OpenGLTexture(&desc, buf_size, buffer);
    delete[] buffer;
    buffer = nullptr;

    return texture;
}

//---------------------------------------------------------------------------//

#define BASE GlassWnd

#ifdef THIS
#undef THIS
#endif

#define THIS OpenGLWnd

//---------------------------------------------------------------------------//

struct THIS::Impl
{
    INT32 last_x, last_y;

    void CreateContext(THIS* glwnd)
    {
        if ( glwnd->m_dc )
        {
            ::wglMakeCurrent(glwnd->m_dc, nullptr);
            if ( glwnd->m_glrc )
            {
                ::wglDeleteContext(glwnd->m_glrc);
            }
            ::ReleaseDC(glwnd->m_hwnd, glwnd->m_dc);
        }

        glwnd->m_dc = ::GetDC(glwnd->m_hwnd);

        auto pxfmt = ::ChoosePixelFormat(glwnd->m_dc, &pfd);
        ::SetPixelFormat(glwnd->m_dc, pxfmt, &pfd);

        glwnd->m_glrc = ::wglCreateContext(glwnd->m_dc);
        ::wglMakeCurrent(glwnd->m_dc, glwnd->m_glrc);
    }
};

//---------------------------------------------------------------------------//

OpenGLWnd::THIS()
{
    pimpl = new Impl;

    m_className = TEXT("OpenGLWnd");
    UWnd::Register(m_className);
}

//---------------------------------------------------------------------------//

OpenGLWnd::~THIS()
{
    if ( m_tex )
    {
        delete m_tex;
        m_tex = nullptr;
    }

    delete pimpl;
    pimpl = nullptr;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::WndProc
(
    HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp
)
{
    //console_out(TEXT("uMsg: 0x%04X"), uMsg);

    UINT16 fwKeys;
    INT16  zDelta;
    INT32  x;
    INT32  y;

    if ( WM_MOUSEMOVE <= uMsg && uMsg <= WM_MOUSEHWHEEL )
    {
        fwKeys = GET_KEYSTATE_WPARAM(wp);
        zDelta = GET_WHEEL_DELTA_WPARAM(wp);
        x = GET_X_LPARAM(lp);
        y = GET_Y_LPARAM(lp);
    }

    switch ( uMsg )
    {
        case WM_CREATE:
        {
            return this->OnCreate(wp, lp);
        }
        case WM_DESTROY:
        {
            return this->OnDestroy(wp, lp);
        }
        case WM_SIZE:
        {
            auto fwSizeType = LOWORD(wp);
            auto w = (UINT16)(lp & 0xFFFF);
            auto h = (UINT16)((lp  >> 16) & 0xFFFF);
            return this->OnSize(fwSizeType, w, h);
        }
        case WM_ERASEBKGND:
        {
            return 1L;
        }
        case WM_NCCALCSIZE:
        {
            if ( wp == TRUE )
            {
                return this->OnNcCalcSize((NCCALCSIZE_PARAMS*)lp);
            }
            else
            {
                return BASE::WndProc(hwnd, uMsg, wp, lp);
            }
        }
        case WM_NCHITTEST:
        {
            POINT pt;
            ::GetCursorPos(&pt);
            return this->OnNcHitTest(pt.x, pt.y);
        }
        case WM_MOUSEMOVE:
        {
            return this->OnMouseMove(fwKeys, x, y);
        }
        case WM_KEYDOWN:
        {
            return this->OnKeyDown((UINT16)wp, lp);
        }
        case WM_LBUTTONDOWN:
        {
            return this->OnLButtonDown(fwKeys, x, y);
        }
        case WM_LBUTTONUP:
        {
            return this->OnLButtonUp(fwKeys, x, y);
        }
        case WM_LBUTTONDBLCLK:
        {
            return this->OnLButtonDblClk(fwKeys, x, y);
        }
        case WM_RBUTTONDOWN:
        {
            return this->OnRButtonDown(fwKeys, x, y);
        }
        case WM_RBUTTONUP:
        {
            return this->OnRButtonUp(fwKeys, x, y);
        }
        case WM_MBUTTONDOWN:
        {
            return this->OnMButtonDown(fwKeys, x, y);
        }
        case WM_MBUTTONUP:
        {
            return this->OnMButtonUp(fwKeys, x, y);
        }
        case WM_MOUSEWHEEL:
        {
            return this->OnMouseWheel(fwKeys, zDelta, x, y);
        }
        case WM_MOUSEHWHEEL:
        {
            return this->OnMouseHWheel(fwKeys, zDelta, x, y);
        }
        case WM_DROPFILES:
        {
            return this->OnDropFiles((HDROP)wp);
        }
        default:
        {
            return BASE::WndProc(hwnd, uMsg, wp, lp);
        }
    }
}

//---------------------------------------------------------------------------//

void __stdcall OpenGLWnd::Update()
{
    auto h_caption = ::GetSystemMetrics(SM_CYCAPTION);
    auto left   = -1.0f;
    auto right  =  1.0f;
    auto top    =  1.0f;
    auto bottom = -1.0f;

    ::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    ::glClear(GL_COLOR_BUFFER_BIT);

    ::glEnable(GL_ALPHA_TEST);
    ::glEnable(GL_TEXTURE_2D);

    if ( m_tex )
    {
         auto tex_instance = *(GLuint*)m_tex->Instance();
        ::glBindTexture(GL_TEXTURE_2D , tex_instance);
    }
    else
    {
        console_out(TEXT("Missing Texture"));
        m_tex = MakeTexture();
    }

    ::glBegin(GL_TRIANGLE_STRIP);
    {
        ::glColor4f(0.0f, 0.5f, 1.0f, 0.5f);

        ::glTexCoord2f(0.0f, 1.0f);
        ::glVertex4f( left, bottom, 0.0f, 1.0f);

        ::glTexCoord2f(0.0f, 0.0f);
        ::glVertex4f( left,    top, 0.0f, 1.0f);

        ::glTexCoord2f(1.0f, 1.0f);
        ::glVertex4f(right, bottom, 0.0f, 1.0f);

        ::glTexCoord2f(1.0f, 0.0f);
        ::glVertex4f(right,    top, 0.0f, 1.0f);
    }
    ::glEnd();

    ::glDisable(GL_TEXTURE_2D);
    ::glDisable(GL_ALPHA_TEST);

    ::glFlush();
    ::SwapBuffers(m_dc);
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnCreate(WPARAM wp, LPARAM lp)
{
    // AeroGlass化
    ::SendMessage(m_hwnd, WM_THEMECHANGED, 0, 0);

    // ウィンドウアイコンを登録
    HICON icon = nullptr;
    icon = (HICON)::LoadImage
    (
        ::GetModuleHandle(nullptr), MAKEINTRESOURCE(1000), IMAGE_ICON,
        0, 0, LR_DEFAULTSIZE | LR_SHARED
    );
   ::SendMessage(m_hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)icon);

    // D&D を受け付ける
    ::DragAcceptFiles(m_hwnd,TRUE);

    // テクスチャの読み込み
    pimpl->CreateContext(this);
    m_tex = MakeTexture();

    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnDestroy(WPARAM wp, LPARAM lp)
{
    ::wglMakeCurrent(m_dc, nullptr);

    ::wglDeleteContext(m_glrc);
    m_glrc = nullptr;

    ::ReleaseDC(m_hwnd, m_dc);
    m_dc = nullptr;

    return ::DefWindowProc(m_hwnd, WM_DESTROY, wp, lp);
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnSize(UINT16 fwSizeType, UINT16 w, UINT16 h)
{
    if ( !m_fullscreen )
    {
        h -= ::GetSystemMetrics(SM_CYCAPTION);
    }

    auto x  = 0;
    auto y  = 0;
    auto sw = w;
    auto sh = h;
    auto iw = m_tex->Desc()->width;
    auto ih = m_tex->Desc()->height;
    //console_out(TEXT("(iw, ih) = (%d, %d)"), iw, ih);

    if ( w * ih < h * iw )
    {
        // 縦余りのとき
        sh = ih * w / iw;
        y = (h - sh) / 2;
    }
    else
    {
        // 横余りのとき
        sw = iw * h / ih;
        x = (w - sw) / 2;
    }
    //console_out(TEXT("(sw, sh) = (%d, %d)"), sw, sh);

    ::glViewport(x, y, (GLsizei)sw, (GLsizei)sh);

    this->Update();

    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnNcCalcSize(NCCALCSIZE_PARAMS* pncsp)
{
    if ( m_fullscreen )
    {
        return 0L;
    }

    auto cx = ::GetSystemMetrics(SM_CXSIZEFRAME);
    auto cy = ::GetSystemMetrics(SM_CYSIZEFRAME);

    pncsp->rgrc[0].left   = pncsp->rgrc[0].left   + cx;
    pncsp->rgrc[0].right  = pncsp->rgrc[0].right  - cx;
    pncsp->rgrc[0].top    = pncsp->rgrc[0].top    + 0;
    pncsp->rgrc[0].bottom = pncsp->rgrc[0].bottom - cy;

    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnMouseMove(UINT16 fwKeys, INT32 x, INT32 y)
{
    if ( fwKeys & MK_LBUTTON )
    {
        console_out(TEXT("L dragging: (X, Y) = (%02d, %02d)"), x, y);
    }
    if ( fwKeys & MK_RBUTTON )
    {
        console_out(TEXT("R dragging: (X, Y) = (%02d, %02d)"), x, y);
    }
    if ( fwKeys & MK_MBUTTON )
    {
        console_out(TEXT("M dragging: (X, Y) = (%02d, %02d)"), x, y);
    }

    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnNcHitTest(INT32 x, INT32 y)
{
    if ( m_fullscreen )
    {
        return BASE::WndProc(m_hwnd, WM_NCHITTEST, 0, MAKELPARAM(x, y));
    }

    auto cx = ::GetSystemMetrics(SM_CXSIZEFRAME);
    auto cy = ::GetSystemMetrics(SM_CYSIZEFRAME);

    if ( x >= m_x + cx && x < m_x + m_w - cx && y <= m_y + cy )
    {
        return HTTOP;
    }
    else
    {
        return BASE::WndProc(m_hwnd, WM_NCHITTEST, 0, MAKELPARAM(x, y));
    }
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnKeyDown(UINT16 nVirtKey, LPARAM lp)
{
    auto cx = ::GetSystemMetrics(SM_CXSIZEFRAME);
    auto cy = ::GetSystemMetrics(SM_CYSIZEFRAME);

    switch ( nVirtKey )
    {
        case 'C':
        {
            this->ToCenter();
            break;
        }
        case 'R':
        {
            if ( (::GetKeyState(VK_CONTROL) & 0x80) )
            {
                this->Update();
            }
            break;
        }
        case 'W':
        {
            if ( (::GetKeyState(VK_CONTROL) & 0x80) )
   
            {
                ::PostMessage(m_hwnd, WM_CLOSE, 0, 0);
            }
            break;
        }
        case VK_ESCAPE:
        {
            if ( this->IsFullScreen() )
            {
                this->ToggleFullScreen(256, 256);
            }
            break;
        }
        case VK_RETURN:
        case VK_F11:
        {
            this->ToggleFullScreen(256, 256);
            break;
        }
        case VK_LEFT:
        {
            this->Move(m_x - cx - 1, m_y);
            break;
        }
        case VK_UP:
        {
            this->Move(m_x - cx, m_y - 1);
            break;
        }
        case VK_RIGHT:
        {
            this->Move(m_x - cx + 1, m_y);
            break;
        }
        case VK_DOWN:
        {
            this->Move(m_x - cx, m_y + 1);
            break;
        }
        case VK_F5:
        {
            this->Update();
            break;
        }
        default:
        {
            break;
        }
    }

    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnLButtonDown(UINT16 fwKeys, INT32 x, INT32 y)
{
    if ( m_fullscreen )
    {
        return 0L;
    }

    auto h_caption = ::GetSystemMetrics(SM_CYCAPTION);
    if ( y <= h_caption )
    {
        ::SendMessage(m_hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
    }

    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnLButtonDblClk(UINT16 fwKeys, INT32 x, INT32 y)
{
    auto h_caption = ::GetSystemMetrics(SM_CYCAPTION);
    if ( y <= h_caption )
    {
        ::SendMessage(m_hwnd, WM_NCLBUTTONDBLCLK, HTCAPTION, 0);

    }
    else
    {
        this->ToggleFullScreen(256, 256);
    }

    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnLButtonUp(UINT16 fwKeys, INT32 x, INT32 y)
{
    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnRButtonDown(UINT16 fwKeys, INT32 x, INT32 y)
{
    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnRButtonUp(UINT16 fwKeys, INT32 x, INT32 y)
{
    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnMButtonDown(UINT16 fwKeys, INT32 x, INT32 y)
{
    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnMButtonUp(UINT16 fwKeys, INT32 x, INT32 y)
{
    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnMouseWheel(UINT16 fwKeys, INT16 zDelta, INT32 x, INT32 y)
{
    console_out(TEXT("V scroll: delta = %04d"), zDelta);
    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnMouseHWheel(UINT16 fwKeys, INT16 zDelta, INT32 x, INT32 y)
{
    console_out(TEXT("H scroll: delta = %04d"), zDelta);
    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall OpenGLWnd::OnDropFiles(HDROP hDrop)
{
    WCHAR filename[MAX_PATH + 1];
    ::DragQueryFile(hDrop, 0, filename, MAX_PATH);
    console_out(TEXT("Dropped:"));
    console_out(TEXT("\t%s"), filename);

    if ( m_tex )
    {
        delete m_tex;
        m_tex = nullptr;
    }

    m_tex = LoadTexture(filename);
    if ( m_tex )
    {
        this->OnSize(0, m_w, m_h);
    }
    ::SetFocus(m_hwnd);

    return 0L;
}

//---------------------------------------------------------------------------//

#undef THIS

//---------------------------------------------------------------------------//

// OpenGLWnd.cpp