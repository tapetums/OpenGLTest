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

#ifdef THIS
#undef THIS
#endif

#define THIS UTFConvert

// UTF-8 <-> UTF-16 <-> UTF-32 相互変換 クラス
struct UTFConvert
{
public:
    THIS(const char8_t*  s) { u8s  = std::u8string(s);  MakeString(u8s);  }
    THIS(const char16_t* s) { u16s = std::u16string(s); MakeString(u16s); }
    THIS(const char32_t* s) { u32s = std::u32string(s); MakeString(u32s); }
    ~THIS() { }

public:
    const char8_t*  toUTF8()  { return u8s.data();  }
    const char16_t* toUTF16() { return u16s.data(); }
    const char32_t* toUTF32() { return u32s.data(); }

protected:
    void MakeString(std::u8string& s)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt16;
        u16s = cvt16.from_bytes((const char*)s.data());

        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt32;
        u32s = cvt32.from_bytes((const char*)s.data());
    }

    void MakeString(std::u16string& s)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt8;
        u8s = (const char8_t*)cvt8.to_bytes(s).data();

        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt32;
        u32s = cvt32.from_bytes((const char*)u8s.data());
    }

    void MakeString(std::u32string& s)
    {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt8;
        auto u8s = cvt8.to_bytes(s);

        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt16;
        auto tmp16 = cvt16.from_bytes(u8s);
    }

private:
    std::u8string  u8s;
    std::u16string u16s;
    std::u32string u32s;
};

#undef THIS

//---------------------------------------------------------------------------//

// UTFConvert.hpp