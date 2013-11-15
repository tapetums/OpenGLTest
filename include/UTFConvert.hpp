// UTFConvert.hpp

#pragma once

///---------------------------------------------------------------------------//
//
// UTF-8 <-> UTF-16 <-> UTF-32 相互変換 ユーティリティ
//
//---------------------------------------------------------------------------//

#include <string>
#include <codecvt>

//---------------------------------------------------------------------------//

typedef unsigned char char8_t;
namespace std
{
    typedef basic_string<char8_t> u8string;
}

//---------------------------------------------------------------------------//

#ifndef UTFCONVERT_UNICODE_STRING_LITERAL_PREFIX
#define UTFCONVERT_UNICODE_STRING_LITERAL_PREFIX
#define __u8 (const char8_t*)
#define __u  (const char16_t*)
#define __U  (const char32_t*)
#endif

//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS UTFConvert

// UTF-8 <-> UTF-16 <-> UTF-32 相互変換 クラス
struct UTFConvert
{
public:
    THIS(const char8_t*  s);
    THIS(const char16_t* s);
    THIS(const char32_t* s);
    ~THIS();

public:
    const char8_t*  __stdcall toUTF8();
    const char16_t* __stdcall toUTF16();
    const char32_t* __stdcall toUTF32();

protected:
    void __stdcall MakeString(std::u8string& s);
    void __stdcall MakeString(std::u16string& s);
    void __stdcall MakeString(std::u32string& s);

private:
    std::u8string  u8s;
    std::u16string u16s;
    std::u32string u32s;
};

#undef THIS

//---------------------------------------------------------------------------//

// UTFConvert.hpp