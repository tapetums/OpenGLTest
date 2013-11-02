// GLTexture.h

#pragma once

#include "ITexture.h"

typedef unsigned char uint8_t;

//---------------------------------------------------------------------------//

class GLTexture : public ITexture
{
public:
    GLTexture(const TextureDesc* desc, size_t buf_size, const void* buffer);
    ~GLTexture() override;

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

// GLTexture.h