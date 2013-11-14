// OpenGLTexture.hpp

#pragma once

///---------------------------------------------------------------------------//
//
// ITexure の OpenGL版 実装
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#include <stdint.h>
#include <gl/gl.h>

#include "ITexture.hpp"

//---------------------------------------------------------------------------//

// ITexure の OpenGL版 実装
class OpenGLTexture : public ITexture
{
public:
    OpenGLTexture(const TextureDesc* desc, size_t buf_size, const void* buffer);
    ~OpenGLTexture() override;

    const void*        Buffer()   const override;
    const size_t       BufSize()  const override;
    const TextureDesc* Desc()     const override;
    const void*        Instance() const override;

private:
    void Init(const TextureDesc* desc, size_t buf_size, const void* buffer);
    void Uninit();

private:
    GLuint      m_instance = 0;
    size_t      m_buf_size = 0;
    uint8_t*    m_buffer   = nullptr;
    TextureDesc m_desc;
};

//---------------------------------------------------------------------------//

// OpenGLTexture.hpp