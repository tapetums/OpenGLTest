﻿// OpenGLTexture.cpp

///---------------------------------------------------------------------------//
//
// ITexure の OpenGL版 実装
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#include <windows.h>

#include <gl/gl.h>
#include <gl/glext.h>
#pragma comment(lib, "opengl32.lib")

#include "include\DebugPrint.h"

#include "OpenGLTexture.hpp"

//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS OpenGLTexture

//---------------------------------------------------------------------------//

OpenGLTexture::THIS
(
    const TextureDesc* desc, size_t buf_size, const void* buffer
)
{
    this->Init(desc, buf_size, buffer);
}

//---------------------------------------------------------------------------//

OpenGLTexture::~THIS()
{
    this->Uninit();
}

//---------------------------------------------------------------------------//

const void* OpenGLTexture::Buffer() const
{
    return m_buffer;
}

//---------------------------------------------------------------------------//

const size_t OpenGLTexture::BufSize() const
{
    return m_buf_size;
}

//---------------------------------------------------------------------------//

const TextureDesc* OpenGLTexture::Desc() const
{
    return &m_desc;
}

//---------------------------------------------------------------------------//

const void* OpenGLTexture::Instance() const
{
    return &m_instance;
}

//---------------------------------------------------------------------------//

void OpenGLTexture::Init
(
    const TextureDesc* desc, size_t buf_size, const void* buffer
)
{
    m_desc     = *desc;
    m_buf_size = buf_size;
    m_buffer   = new uint8_t[buf_size / sizeof(uint8_t)];
    ::CopyMemory(m_buffer, buffer, buf_size);

    int32_t internal_format = GL_RGBA;
    int32_t format = 0;
    int32_t type   = 0;
    GLint   param  = 0;
    switch( m_desc.format )
    {
        case PixelFormat::INDEX8:
        {
            internal_format = GL_RED;
            format = GL_COLOR_INDEX;
            type   = GL_UNSIGNED_BYTE;
            param  = 1;
            break;
        }
        case PixelFormat::RGB888:
        {
            format = GL_RGB;
            type   = GL_UNSIGNED_BYTE;
            param  = 1;
            break;
        }
        case PixelFormat::BGR888:
        {
            format = GL_BGR;
            type   = GL_UNSIGNED_BYTE;
            param  = 1;
            break;
        }
        case PixelFormat::RGBA8888:
        {
            format = GL_RGBA;
            type   = GL_UNSIGNED_INT_8_8_8_8_REV;
            param  = 4;
            break;
        }
        case PixelFormat::BGRA8888:
        {
            format = GL_BGRA;
            type   = GL_UNSIGNED_INT_8_8_8_8_REV;
            param  = 4;
            break;
        }
        default:
        {
            format = 0;
            type   = 0;
            param  = 0;
            break;
        }
    }

    // テクスチャを設定
    ::glGenTextures(1, &m_instance);
    if ( m_instance == 0 )
    {
        console_out(TEXT("glGenTextures error: 0x%04x"), glGetError());
    }

    ::glBindTexture(GL_TEXTURE_2D, m_instance);
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, param);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_desc.interpolation_min);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_desc.interpolation_min);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_desc.repeat_s);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_desc.repeat_t);
    ::glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    ::glTexImage2D
    (
        GL_TEXTURE_2D, 0, internal_format,
        m_desc.width, m_desc.height, 0,
        format, type, m_buffer
    );
    ::glBindTexture(GL_TEXTURE_2D, 0);
}

//---------------------------------------------------------------------------//

void OpenGLTexture::Uninit()
{
    if ( m_instance != 0 )
    {
        ::glDeleteTextures(1, &m_instance);
        m_instance = 0;
    }

    m_buf_size = 0;
    if ( m_buffer )
    {
        delete[] m_buffer;
        m_buffer = nullptr;
    }

    ::ZeroMemory(&m_desc, sizeof(m_desc));
}

//---------------------------------------------------------------------------//

#undef THIS

//---------------------------------------------------------------------------//

// OpenGLTexture.cpp