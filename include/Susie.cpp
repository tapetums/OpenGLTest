// Susie.cpp

//----------------------------------------------------------------------------//
//
// Susie プラグインのラッパークラス
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS Susie

//----------------------------------------------------------------------------//

#include "Susie.hpp"

//---------------------------------------------------------------------------//

#include <locale>

// マルチバイト文字列をワイド文字列に変換して
// 同じバッファ内に無理矢理つっこむ関数
void MBCSztoUTF16z(char* buf, size_t buflen)
{
    auto tmp = new char[buflen + 1];
    ::strcpy_s(tmp, buflen, str);
    tmp[cb_str] = '\0';

    //::setlocale(LC_CTYPE, "JPN");
    std::locale::global(std::locale(""));

    size_t cc = 0;
    ::mbstowcs_s(&cc, (wchar_t*)str, buflen * sizeof(char), (const char*)tmp, (buflen + 1) * sizeof(char));

    delete[] tmp;
    tmp = nullptr;
}

//---------------------------------------------------------------------------//

// ワイド文字列をマルチバイト文字列に変換して
// 同じバッファ内に無理矢理つっこむ関数
void UTF16ztoMBCSz(wchar_t* wstr)
{
    auto wstrlen = ::wcslen(wstr);
    auto tmp = new wchar_t[wstrlen + 1];
    ::lstrcpy(tmp, wstr);
    tmp[wstrlen] = '\0';

    //::setlocale(LC_CTYPE, "JPN");
    std::locale::global(std::locale(""));

    size_t cc = 0;
    ::wcstombs_s(&cc, (char*)wstr, wstrlen * sizeof(wchar_t), (const wchar_t*)tmp, (wstrlen + 1) * sizeof(wchar_t));

    delete[] tmp;
    tmp = nullptr;
}

//---------------------------------------------------------------------------//

Susie::THIS()
{
    m_spi_path[0] = '\0';
}

//---------------------------------------------------------------------------//

Susie::THIS(LPCWSTR spi_path) : CLASSNAME()
{
    this->Load(spi_path);
}

//---------------------------------------------------------------------------//

Susie::~THIS()
{
    this->Free();
}

//---------------------------------------------------------------------------//

bool __stdcall Susie::Load(LPCWSTR spi_path)
{
    if ( m_module )
    {
        return false;
    }

    // DLLの読み込み
    m_module = ::LoadLibraryExW
    (
        spi_path, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH
    );
    if ( nullptr == m_module )
    {
        goto FREE_SPI;
    }

    // DLLのフルパスを取得
    ::GetModuleFileNameW(m_module, m_spi_path, MAX_PATH);

    // GetPluginInfo
    m_GetPluginInfoA = (SPI_GetPluginInfoA)::GetProcAddress
    (
        m_module, "GetPluginInfo"
    );
    m_GetPluginInfoW = (SPI_GetPluginInfoW)::GetProcAddress
    (
        m_module, "GetPluginInfoW"
    );
    if ( nullptr == m_GetPluginInfoA && nullptr == m_GetPluginInfoW )
    {
        goto FREE_SPI;
    }
    // IsSupported
    m_IsSupportedA = (SPI_IsSupportedA)::GetProcAddress
    (
        m_module, "IsSupported"
    );
    m_IsSupportedW = (SPI_IsSupportedW)::GetProcAddress
    (
        m_module, "IsSupportedW"
    );
    if ( nullptr == m_IsSupportedA && nullptr == m_IsSupportedW )
    {
        goto FREE_SPI;
    }

    // GetPictureInfo
    m_GetPictureInfoA = (SPI_GetPictureInfoA)::GetProcAddress
    (
        m_module, "GetPictureInfo"
    );
    m_GetPictureInfoW = (SPI_GetPictureInfoW)::GetProcAddress
    (
        m_module, "GetPictureInfoW"
    );
    // GetPicture
    m_GetPictureA = (SPI_GetPictureA)::GetProcAddress
    (
        m_module, "GetPicture"
    );
    m_GetPictureW = (SPI_GetPictureW)::GetProcAddress
    (
        m_module, "GetPictureW"
    );
    // GetPreview
    m_GetPreviewA = (SPI_GetPreviewA)::GetProcAddress
    (
        m_module, "GetPreview"
    );
    m_GetPreviewW = (SPI_GetPreviewW)::GetProcAddress
    (
        m_module, "GetPreviewW"
    );

    // GetArchiveInfo
    m_GetArchiveInfoA = (SPI_GetArchiveInfoA)::GetProcAddress
    (
        m_module, "GetArchiveInfo"
    );
    m_GetArchiveInfoW = (SPI_GetArchiveInfoW)::GetProcAddress
    (
        m_module, "GetArchiveInfoW"
    );
    // GetFileInfo
    m_GetFileInfoA = (SPI_GetFileInfoA)::GetProcAddress
    (
        m_module, "GetFileInfo"
    );
    m_GetFileInfoW = (SPI_GetFileInfoW)::GetProcAddress
    (
        m_module, "GetFileInfoW"
    );
    // GetFile
    m_GetFileA = (SPI_GetFileA)::GetProcAddress
    (
        m_module, "GetFile"
    );
    m_GetFileW = (SPI_GetFileW)::GetProcAddress
    (
        m_module, "GetFileW"
    );

    // ConfigurationDlg
    m_ConfigurationDlg = (SPI_ConfigurationDlg)::GetProcAddress
    (
        m_module, "ConfigurationDlg"
    );

    return true;

FREE_SPI:

    this->Free();

    return false;
}

//---------------------------------------------------------------------------//

bool __stdcall Susie::Free()
{
    BOOL result = 0;

    if ( m_module )
    {
        result = ::FreeLibrary(m_module);
    }

    if ( result )
    {
        return false;
    }
    else
    {
        m_spi_path[0]      = '\0';
        m_module           = nullptr;
        m_GetPluginInfoA   = nullptr;
        m_GetPluginInfoW   = nullptr;
        m_IsSupportedA     = nullptr;
        m_IsSupportedW     = nullptr;
        m_GetPictureInfoA  = nullptr;
        m_GetPictureInfoW  = nullptr;
        m_GetPictureA      = nullptr;
        m_GetPictureW      = nullptr;
        m_GetPreviewA      = nullptr;
        m_GetPreviewW      = nullptr;
        m_GetArchiveInfoA  = nullptr;
        m_GetArchiveInfoW  = nullptr;
        m_GetFileInfoA     = nullptr;
        m_GetFileInfoW     = nullptr;
        m_GetFileA         = nullptr;
        m_GetFileW         = nullptr;
        m_ConfigurationDlg = nullptr;

        return true;
    }
}

//---------------------------------------------------------------------------//

LPCWSTR __stdcall Susie::SpiPath() const
{
    return m_spi_path;
}

//---------------------------------------------------------------------------//

SUSIE_API Susie::GetPluginInfo(int32_t infono, LPWSTR buf, int32_t buflen)
{
    if ( m_GetPluginInfoW )
    {
        return m_GetPluginInfoW(infono, buf, buflen);
    }
    else if ( m_GetPluginInfoA )
    {
        auto result = m_GetPluginInfoA(infono, (LPSTR)buf, buflen);
        if ( result == SPI_ALL_RIGHT )
        {
            MBCSztoUTF16z((LPSTR)buf, buflen);
        }
        return result;
    }
    else
    {
        return SPI_NO_FUNCTION;
    }
}

//---------------------------------------------------------------------------//

SUSIE_API Susie::IsSupported(LPCWSTR filename, void* dw)
{
    if ( m_IsSupportedW )
    {
        return m_IsSupportedW(filename, dw);
    }
    else if ( m_IsSupportedA )
    {
        UTF16ztoMBCSz((LPWSTR)filename);

        return m_IsSupportedA((LPCSTR)filename, dw);
    }
    else
    {
        return SPI_NO_FUNCTION;
    }
}

//---------------------------------------------------------------------------//

SUSIE_API Susie::GetPictureInfo(LPCWSTR buf, size_t len, SPI_FLAG flag, SusiePictureInfo* lpInfo)
{
    if ( m_GetPictureInfoW )
    {
        return m_GetPictureInfoW(buf, len, flag, lpInfo);
    }
    else if ( m_GetPictureInfoA )
    {
        if ( !(flag & SPI_INPUT_MEMORY) )
        {
            UTF16ztoMBCSz((LPWSTR)buf);
        }
        return m_GetPictureInfoA((LPCSTR)buf, len, flag, lpInfo);
    }
    else
    {
        return SPI_NO_FUNCTION;
    }
}

//---------------------------------------------------------------------------//

SUSIE_API Susie::GetPicture(LPCWSTR buf, size_t len, SPI_FLAG flag, HANDLE* pHBInfo, HANDLE* pHBm, SUSIE_PROGRESS progressCallback, intptr_t lData)
{
    if ( m_GetPictureW )
    {
        return m_GetPictureW(buf, len, flag, pHBInfo, pHBm, progressCallback, lData);
    }
    else if ( m_GetPictureA )
    {
    
        if ( !(flag & SPI_INPUT_MEMORY) )
        {
            UTF16ztoMBCSz((LPWSTR)buf);
        }
        return m_GetPictureA((LPCSTR)buf, len, flag, pHBInfo, pHBm, progressCallback, lData);
    }
    else
    {
        return SPI_NO_FUNCTION;
    }
}

//---------------------------------------------------------------------------//

SUSIE_API Susie::GetPreview(LPCWSTR buf, size_t len, SPI_FLAG flag, HANDLE* pHBInfo, HANDLE* pHBm, SUSIE_PROGRESS progressCallback, intptr_t lData)
{
    if ( m_GetPreviewW )
    {
        return m_GetPreviewW(buf, len, flag, pHBInfo, pHBm, progressCallback, lData);
    }
    else if ( m_GetPreviewA )
    {
        if ( !(flag & SPI_INPUT_MEMORY) )
        {
            UTF16ztoMBCSz((LPWSTR)buf);
        }
        return m_GetPreviewA((LPCSTR)buf, len, flag, pHBInfo, pHBm, progressCallback, lData);
    }
    else
    {
        return SPI_NO_FUNCTION;
    }
}

//---------------------------------------------------------------------------//

SUSIE_API Susie::GetArchiveInfo(LPCWSTR buf, size_t len, SPI_FLAG flag, HLOCAL* lphInf)
{
    if ( m_GetArchiveInfoW )
    {
        return m_GetArchiveInfoW(buf, len, flag, lphInf);
    }
    else if ( m_GetArchiveInfoA )
    {
        if ( !(flag & SPI_INPUT_MEMORY) )
        {
            UTF16ztoMBCSz((LPWSTR)buf);
        }
        return m_GetArchiveInfoA((LPCSTR)buf, len, flag, lphInf);
    }
    else
    {
        return SPI_NO_FUNCTION;
    }
}

//---------------------------------------------------------------------------//

SUSIE_API Susie::GetFileInfo(LPCWSTR buf, size_t len, LPCWSTR filename, SPI_FLAG flag, SusieFileInfoW* lpInfo)
{
    if ( m_GetFileInfoW )
    {
        return m_GetFileInfoW(buf, len, filename, flag, lpInfo);
    }
    else if ( m_GetFileInfoA )
    {
        UTF16ztoMBCSz((LPWSTR)filename);
        if ( !(flag & SPI_INPUT_MEMORY) )
        {
            UTF16ztoMBCSz((LPWSTR)buf);
        }

        SusieFileInfoA info;
        auto result =  m_GetFileInfoA((LPCSTR)buf, len, (LPCSTR)filename, flag, &info);
        if ( result == SPI_ALL_RIGHT )
        {
            ::CopyMemory(lpInfo,           &info,          sizeof(uint8_t)*8 + sizeof(size_t)*3 + sizeof(susie_time_t));
            ::CopyMemory(lpInfo->path,     &info.path,     sizeof(char)*SPI_MAX_PATH);
            ::CopyMemory(lpInfo->filename, &info.filename, sizeof(char)*SPI_MAX_PATH);
            lpInfo->crc = info.crc;

            MBCSztoUTF16z((LPSTR)lpInfo->path,     sizeof(wchar_t)*SPI_MAX_PATH);
            MBCSztoUTF16z((LPSTR)lpInfo->filename, sizeof(wchar_t)*MAX_PATH);
        }
        return result;
    }
    else
    {
        return SPI_NO_FUNCTION;
    }
}

//---------------------------------------------------------------------------//

SUSIE_API Susie::GetFile(LPCWSTR src, size_t len, LPWSTR dest, SPI_FLAG flag, SUSIE_PROGRESS progressCallback, intptr_t lData)
{
    if ( m_GetFileW )
    {
        return m_GetFileW(src, len, dest, flag, progressCallback, lData);
    }
    else if ( m_GetFileA )
    {
        if ( !(flag & SPI_INPUT_MEMORY) )
        {
            UTF16ztoMBCSz((LPWSTR)src);
        }
        if ( !(flag & SPI_OUTPUT_MEMORY) )
        {
            UTF16ztoMBCSz((LPWSTR)dest);
        }
        return m_GetFileA((LPCSTR)src, len, (LPSTR)dest, flag, progressCallback, lData);
    }
    else
    {
        return SPI_NO_FUNCTION;
    }
}

//---------------------------------------------------------------------------//

SUSIE_API Susie::ConfigurationDlg(HWND parent, SPI_FNC_CODE fnc)
{
    if ( m_ConfigurationDlg )
    {
        return m_ConfigurationDlg(parent, fnc);
    }
    else
    {
        return SPI_NO_FUNCTION;
    }
}

//---------------------------------------------------------------------------//

#undef THIS

//---------------------------------------------------------------------------//

// Susie.cpp
