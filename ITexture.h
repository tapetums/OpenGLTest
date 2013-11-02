// ITexture.h

#pragma once

typedef unsigned __int32 uint32_t;
typedef __int32          int32_t;
typedef double           float64_t;

//---------------------------------------------------------------------------//

enum class PixelFormat : uint32_t
{
    UNKNOWN = (uint32_t)-1,
    A8 = 0,
    RGB888,
    BGR888,
    RGBA8888,
    ARGB8888,
    BGRA8888,
    ABGR8888,
};

//---------------------------------------------------------------------------//

struct TextureDesc
{
    PixelFormat format;
    int32_t     width;
    int32_t     height;
    float64_t   dpiX;
    float64_t   dpiY;
    int32_t     interpolation_min;
    int32_t     interpolation_max;
    int32_t     repeat_s;
    int32_t     repeat_t;
    int32_t     envi;
};

//---------------------------------------------------------------------------//

struct ITexture
{
public:
    ITexture()          = default;
    virtual ~ITexture() = default;

    virtual const void*        Buffer()   const = 0;
    virtual const size_t       BufSize()  const = 0;
    virtual const TextureDesc* Desc()     const = 0;
    virtual const void*        Instance() const = 0;

private:
    ITexture(const ITexture&)             = delete;
    ITexture(ITexture&&)                  = delete;
    ITexture& operator= (const ITexture&) = delete;
    ITexture& operator= (ITexture&&)      = delete;
};

//---------------------------------------------------------------------------//

// ITexture.h