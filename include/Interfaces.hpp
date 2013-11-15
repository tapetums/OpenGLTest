// Interfaces.hpp

#pragma once

//---------------------------------------------------------------------------//
//
// CubeMelon コンポーネント API ヘッダー
//   Copyright (C) 2011-2013 tapetums
//
//---------------------------------------------------------------------------//

#include <stdint.h>
#include <windows.h>

//---------------------------------------------------------------------------//

typedef unsigned char char8_t;

#define U8STR  char8_t*
#define U8CSTR const char8_t*

//---------------------------------------------------------------------------//
//
// インターフェイスID
//
//---------------------------------------------------------------------------//

static const IID IID_IData =
{ 0x4fe37727, 0x3644, 0x43bf, { 0x9f, 0xea, 0x3e, 0xd2, 0x48, 0x35, 0x3d, 0xc5 } };

static const IID IID_ICompProp =
{ 0x704e6fe9, 0xb308, 0x4bdf, { 0xb4, 0xa1, 0xd6, 0xaf, 0x95, 0xeb, 0x60, 0xd5 } };

static const IID IID_IComponent =
{ 0xa35dc0c3, 0xac5f, 0x447a, { 0xa4, 0x60, 0x9c, 0x52, 0x17, 0x72, 0x05, 0x7c } };

static const IID IID_IIOComponent=
{ 0xeefb9211, 0xfa4, 0x4c0c, { 0x99, 0xd6, 0xc0, 0xc, 0xc6, 0xc0, 0xe1, 0x1 } };

static const IID IID_IReaderComponent =
{ 0x6b132e6c, 0x4703, 0x4281, { 0x88, 0x8d, 0x36, 0x7c, 0x1b, 0x40, 0xbd, 0x6a } };

static const IID IID_IWriterComponent =
{ 0xba6ffb76, 0xa1cb, 0x439c, { 0x9f, 0x6d, 0xca, 0xd6, 0xd1, 0x8a, 0x8f, 0x3e } };

//---------------------------------------------------------------------------//

namespace CubeMelon {

//---------------------------------------------------------------------------//
//
// 文字列定数
//
//---------------------------------------------------------------------------//

// コンポーネント API のバージョン
static U8CSTR API_VERSION = (U8CSTR)"1.0.0";

// NotifyMessage()メッソドで使われる主なメッセージ
static U8CSTR MSG_NULL = nullptr;

static U8CSTR MSG_COMP_START_DONE   = (U8CSTR)"Comp.Start.Done";
static U8CSTR MSG_COMP_START_FAILED = (U8CSTR)"Comp.Start.Failed";
static U8CSTR MSG_COMP_STOP_DONE    = (U8CSTR)"Comp.Stop.Done";
static U8CSTR MSG_COMP_STOP_FAILED  = (U8CSTR)"Comp.Stop.Failed";

static U8CSTR MSG_IO_CLOSE_DONE     = (U8CSTR)"IO.Close.Done";
static U8CSTR MSG_IO_CLOSE_FAILED   = (U8CSTR)"IO.Close.Failed";
static U8CSTR MSG_IO_OPEN_DONE      = (U8CSTR)"IO.Open.Done";
static U8CSTR MSG_IO_OPEN_FAILED    = (U8CSTR)"IO.Open.Failed";
static U8CSTR MSG_IO_READ_DONE      = (U8CSTR)"IO.Read.Done";
static U8CSTR MSG_IO_READ_FAILED    = (U8CSTR)"IO.Read.Failed";
static U8CSTR MSG_IO_WRITE_DONE     = (U8CSTR)"IO.Write.Done";
static U8CSTR MSG_IO_WRITE_FAILED   = (U8CSTR)"IO.Write.Failed";

static const HRESULT S_OK_ASYNC  = 0x00000002;
static const HRESULT E_BUSY      = 0x8FFFFFFF;

//---------------------------------------------------------------------------//
//
// 列挙体
//
//---------------------------------------------------------------------------//

enum class STATE : uint32_t
{
    UNKNOWN  = (uint32_t)-1,
    IDLE     = 0,
    ACTIVE   = 1,
    OPEN     = 1 << 1,
    STARTING = 1 << 2,
    STOPPING = 1 << 3,
    CLOSING  = 1 << 4,
    OPENING  = 1 << 5,
    SEEKING  = 1 << 6,
    READING  = 1 << 7,
    WRITING  = 1 << 8,
};

//---------------------------------------------------------------------------//
//
// 構造体
//
//---------------------------------------------------------------------------//

#pragma pack(push, 1)

struct CompVerInfo
{
    uint8_t major;
    uint8_t minor;
    uint8_t revision;
    char8_t stage;
};

#pragma pack(pop)

//---------------------------------------------------------------------------//
//
// インターフェイス
//
//---------------------------------------------------------------------------//

interface IData : public IUnknown
{
    virtual U8CSTR  __stdcall name()     const = 0;
    virtual size_t  __stdcall size()     const = 0;
    virtual void*   __stdcall get()            = 0;
    virtual HRESULT __stdcall set(void* value) = 0;
};

//---------------------------------------------------------------------------//

interface ICompProp : public IUnknown
{
    virtual size_t __stdcall data_count()           const = 0;
    virtual IData* __stdcall data(size_t index = 0) const = 0;
};

//---------------------------------------------------------------------------//

interface IComponent : public IUnknown
{
    virtual U8CSTR       __stdcall api_version() const = 0;
    virtual REFCLSID     __stdcall clsid()       const = 0;
    virtual U8CSTR       __stdcall name()        const = 0;
    virtual U8CSTR       __stdcall description() const = 0;
    virtual CompVerInfo* __stdcall version()     const = 0;
    virtual IComponent*  __stdcall owner()       const = 0;
    virtual ICompProp*   __stdcall property()    const = 0;
    virtual STATE        __stdcall status()      const = 0;

    virtual HRESULT __stdcall AttachMessage(U8CSTR msg, IComponent* listener) = 0;
    virtual HRESULT __stdcall DetachMessage(U8CSTR msg, IComponent* listener) = 0;
    virtual HRESULT __stdcall NotifyMessage(U8CSTR msg, IComponent* sender, IComponent* listener, IData* data) = 0;
    virtual HRESULT __stdcall CreateInstance(REFCLSID rclsid, REFIID riid, void** ppvObject) = 0;
    virtual HRESULT __stdcall Start(void* args, IComponent* listener = nullptr) = 0;
    virtual HRESULT __stdcall Stop (void* args, IComponent* listener = nullptr) = 0;
};

//---------------------------------------------------------------------------//

interface IIOComponent : public IComponent
{
    virtual HRESULT __stdcall Close(IComponent* listener = nullptr)                               = 0;
    virtual HRESULT __stdcall Open(U8CSTR path, U8CSTR format_as, IComponent* listener = nullptr) = 0;
    virtual HRESULT __stdcall QuerySupport(U8CSTR path, U8CSTR format_as)                         = 0;
    virtual HRESULT __stdcall Seek(int64_t offset, uint32_t origin, uint64_t* new_pos)            = 0;
};

//---------------------------------------------------------------------------//

interface IReaderComponent : public IIOComponent
{
    virtual HRESULT __stdcall Read(void* buffer, size_t buf_size, size_t* cb_data, IComponent* listener = nullptr) = 0;
};

//---------------------------------------------------------------------------//

interface IWriterComponent : public IIOComponent
{
    virtual HRESULT __stdcall Write(void* buffer, size_t buf_size, size_t* cb_data, IComponent* listener = nullptr) = 0;
};

//---------------------------------------------------------------------------//

} // namespace CubeMelon

//---------------------------------------------------------------------------//

// Interfaces.hpp