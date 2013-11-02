// GLWnd.cpp

#define CLASSNAME GLWnd

//---------------------------------------------------------------------------//

#include <windows.h>

#include <wincodec.h>
#include <wincodecsdk.h>
#pragma comment(lib, "windowscodecs.lib")

#include <gl/gl.h>
#include <gl/glext.h>
#pragma comment(lib, "opengl32.lib")

#include "include\DebugPrint.h"
#include "include\ComPtr.h"

#include "GLTexture.h"
#include "GLWnd.h"

//---------------------------------------------------------------------------//

HRESULT MakeTexture(ITexture** pTexture);
HRESULT LoadTexture(LPCWSTR fileName, ITexture** pTexture);

static size_t tex_index = 0;
static const wchar_t* texture_file[] =
{
    L"octocat.bmp",
    L"takoyaki.bmp",
};

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

struct CLASSNAME::Impl
{
    void CreateContext(CLASSNAME* glwnd)
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

CLASSNAME::CLASSNAME()
{
    pimpl = new Impl;

    m_className = TEXT("GLWnd");
    UWnd::Register(m_className);
}

//---------------------------------------------------------------------------//

CLASSNAME::~CLASSNAME()
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

LRESULT __stdcall CLASSNAME::WndProc
(
    HWND hwnd, UINT uMsg, WPARAM wp, LPARAM lp
)
{
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
            auto fwSizeType = (INT32)wp;
            auto w = (INT16)(lp & 0xFFFF);
            auto h = (INT16)((lp  >> 16) & 0xFFFF);
            return this->OnSize(fwSizeType, w, h);
        }
        case WM_LBUTTONDOWN:
        {
            auto fwKeys = (INT32)wp;
            auto x = (INT16)(lp & 0xFFFF);
            auto y = (INT16)((lp  >> 16) & 0xFFFF);
            return this->OnLButtonDown(fwKeys, x, y);
        }
        case WM_RBUTTONDOWN:
        {
            auto fwKeys = (INT32)wp;
            auto x = (INT16)(lp & 0xFFFF);
            auto y = (INT16)((lp  >> 16) & 0xFFFF);
            return this->OnRButtonDown(fwKeys, x, y);
        }
        case WM_ERASEBKGND:
        {
            return 1L;
        }
        default:
        {
            return GlassWnd::WndProc(hwnd, uMsg, wp, lp);
        }
    }
}

//---------------------------------------------------------------------------//

void __stdcall CLASSNAME::Update()
{
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
        DebugPrintLn(TEXT("Missing Texture"));
    }

    ::glBegin(GL_TRIANGLE_STRIP);
    {
        ::glColor4f(0.0f, 0.5f, 1.0f, 0.0f);

        ::glTexCoord2f(0.0f, 1.0f);
        ::glVertex4f(-1.0f, -1.0f, 0.0f, 1.0f);

        ::glTexCoord2f(0.0f, 0.0f);
        ::glVertex4f(-1.0f,  1.0f, 0.0f, 1.0f);

        ::glTexCoord2f(1.0f, 1.0f);
        ::glVertex4f( 1.0f, -1.0f, 0.0f, 1.0f);

        ::glTexCoord2f(1.0f, 0.0f);
        ::glVertex4f( 1.0f,  1.0f, 0.0f, 1.0f);
    }
    ::glEnd();

    ::glDisable(GL_TEXTURE_2D);
    ::glDisable(GL_ALPHA_TEST);
 
    ::glFlush();
    ::SwapBuffers(m_dc);
}

//---------------------------------------------------------------------------//

LRESULT __stdcall CLASSNAME::OnCreate(WPARAM wp, LPARAM lp)
{
    this->OnDwmCompositionChanged(m_hwnd);

    pimpl->CreateContext(this);

    LoadTexture(texture_file[tex_index], &m_tex);
    if ( nullptr == m_tex )
    {
        MakeTexture(&m_tex);
    }

    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall CLASSNAME::OnDestroy(WPARAM wp, LPARAM lp)
{
    ::wglMakeCurrent(m_dc, nullptr);

    ::wglDeleteContext(m_glrc);
    m_glrc = nullptr;

    ::ReleaseDC(m_hwnd, m_dc);
    m_dc = nullptr;

    return ::DefWindowProc(m_hwnd, WM_DESTROY, wp, lp);
}

//---------------------------------------------------------------------------//

LRESULT __stdcall CLASSNAME::OnSize(INT32 fwSizeType, INT16 w, INT16 h)
{
    ::glViewport(0, 0, w, h);

    this->Update();

    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall CLASSNAME::OnLButtonDown(INT32 fwKeys, INT16 x, INT16 y)
{
    /*POINT pt;
    GetCursorPos(&pt);
    auto lp = MAKELPARAM(pt.x, pt.y);*/
    ::SendMessage(m_hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);//lp);

    return 0L;
}

//---------------------------------------------------------------------------//

LRESULT __stdcall CLASSNAME::OnRButtonDown(INT32 fwKeys, INT16 x, INT16 y)
{
    DebugPrintLn(TEXT("tex_index: %d"), tex_index);
    if ( m_tex )
    {
        delete m_tex;
        m_tex = nullptr;
    }

    //tex_index = ++tex_index;
    DebugPrintLn(TEXT("tex_index: %d"), tex_index);
    tex_index = tex_index ^ 1;
    DebugPrintLn(TEXT("tex_index: %d"), tex_index);

    LoadTexture(texture_file[tex_index], &m_tex);
    if ( nullptr == m_tex )
    {
        MakeTexture(&m_tex);
    }

    return 0L;
}

//---------------------------------------------------------------------------//

HRESULT MakeTexture(ITexture** pTexture)
{
    if ( nullptr == pTexture )
    {
        return E_INVALIDARG;
    }
    *pTexture = nullptr;

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

    *pTexture = new GLTexture(&desc, buf_size, buffer);

    return 0L;
}

//---------------------------------------------------------------------------//

HRESULT LoadTexture(LPCWSTR fileName, ITexture** pTexture)
{
    if ( nullptr == pTexture )
    {
        return E_INVALIDARG;
    }
    *pTexture = nullptr;

    HRESULT hr;

    ComPtr<IWICImagingFactory> factory;
    hr = ::CoCreateInstance
    (
        CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory, (void**)&factory
    );
    if ( nullptr == factory.GetInterface() )
    {
        DebugPrintLn(TEXT("CoCreateInstance() failed"));
        return hr;
    }

    ComPtr<IWICBitmapDecoder> decoder;
    hr = factory->CreateDecoderFromFilename
    (
        fileName, nullptr, GENERIC_READ,
        WICDecodeMetadataCacheOnLoad, &decoder
    );
    if ( nullptr == decoder.GetInterface() )
    {
        DebugPrintLn(TEXT("CreateDecoderFromFilename() failed"));
        return hr;
    }

    uint32_t frameCount = 0;
    hr = decoder->GetFrameCount(&frameCount);
    DebugPrintLn(TEXT("frameCount: %d"), frameCount);
    if ( 0 == frameCount )
    {
        DebugPrintLn(TEXT("GetFrameCount() failed"));
        return hr;
    }

    ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, &frame);
    if ( nullptr == frame.GetInterface() )
    {
        DebugPrintLn(TEXT("GetFrame() failed"));
        return hr;
    }

    TextureDesc desc =
    {
        PixelFormat::UNKNOWN, 0, 0, 0, 0,
        GL_LINEAR, GL_LINEAR, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT,
        GL_REPLACE
    };
    frame->GetSize((UINT*)&desc.width, (UINT*)&desc.height);
    DebugPrintLn(TEXT("width: %d, height: %d"), desc.width, desc.height);

    frame->GetResolution(&desc.dpiX, &desc.dpiY);
    DebugPrintLn(TEXT("DPI(X, Y) = (%d, %d)"), desc.dpiX, desc.dpiY);

    GUID pixelFormat = GUID_NULL;
    uint32_t bitCount = 0;
    hr = frame->GetPixelFormat(&pixelFormat);
    DebugPrintLn
    (
        TEXT("pixelFormat: 0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x"),
        pixelFormat.Data1, pixelFormat.Data2, pixelFormat.Data3,
        pixelFormat.Data4[0], pixelFormat.Data4[1], pixelFormat.Data4[2], pixelFormat.Data4[3],
        pixelFormat.Data4[4], pixelFormat.Data4[5], pixelFormat.Data4[6], pixelFormat.Data4[7]
    );
    if ( IsEqualGUID(pixelFormat, GUID_WICPixelFormat8bppIndexed) )
    {
        bitCount = 8;
        desc.format = PixelFormat::A8;
    }
    else if ( IsEqualGUID(pixelFormat, GUID_WICPixelFormat24bppRGB) )
    {
        bitCount = 24;
        desc.format = PixelFormat::RGB888;
    }
    else if ( IsEqualGUID(pixelFormat, GUID_WICPixelFormat24bppBGR) )
    {
        bitCount = 24;
        desc.format = PixelFormat::BGR888;
    }
    else if ( IsEqualGUID(pixelFormat, GUID_WICPixelFormat32bppRGBA) ||
              IsEqualGUID(pixelFormat, GUID_WICPixelFormat32bppRGB) )
    {
        bitCount = 32;
        desc.format = PixelFormat::RGBA8888;
    }
    else if ( IsEqualGUID(pixelFormat, GUID_WICPixelFormat32bppBGRA) ||
              IsEqualGUID(pixelFormat, GUID_WICPixelFormat32bppBGR) )
    {
        bitCount = 32;
        desc.format = PixelFormat::BGRA8888;
    }
    else
    {
        DebugPrintLn(TEXT("GetPixelFormat() failed"));
        return E_FAIL;
    }
    DebugPrintLn(TEXT("bitCount: %d"), bitCount);
    DebugPrintLn(TEXT("PixelFormat: %d"), desc.format);

    auto stride = (desc.width * (bitCount / 8) + 3) & ~3;
    DebugPrintLn(TEXT("stride: %d"), stride);

    auto buffer = new uint8_t[stride * desc.height];
    auto size = sizeof(uint8_t) * (stride * desc.height);
    //::FillMemory(buffer, size, 0xFF);
    hr = frame->CopyPixels(0, stride, size, buffer);

    *pTexture = new GLTexture(&desc, size, buffer);
    delete[] buffer;
    buffer = nullptr;

    return hr;
}

//---------------------------------------------------------------------------//

// GLWnd.cpp