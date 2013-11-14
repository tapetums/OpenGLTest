// ITexture.hpp

#pragma once

//---------------------------------------------------------------------------//
//
// テクスチャオブジェクトのためのインターフェイス
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#include <stdint.h>
typedef double float64_t;

//---------------------------------------------------------------------------//

// ピクセルフォーマットの内部表現
enum class PixelFormat : uint32_t
{
    UNKNOWN = (uint32_t)-1,
    INDEX8 = 0,
    RGB888,
    BGR888,
    RGBA8888,
    ARGB8888,
    BGRA8888,
    ABGR8888,
};

//---------------------------------------------------------------------------//

// テクスチャ情報を保持する構造体
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

// テクスチャオブジェクトのためのインターフェイス
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

// ITexture.hpp