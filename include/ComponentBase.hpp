// ComponentBase.hpp

#pragma once

//---------------------------------------------------------------------------//
//
// コンポーネントの基底クラス
//   Copyright (C) 2013 tapetums
//
//  C++ は 直線継承以外の形でインターフェイスを実装すると
//  (e.g. class Hoge : public Interface, public ClassBase {}; )
//  「定義があいまい」コンパイルエラーになるので、
//   仕方なく重複するコードを複数の .cpp ファイルに書いています。
//  解決策が見つかれば改善する予定
//
//---------------------------------------------------------------------------//

#include <string>
#include <unordered_map>

#include "Interfaces.hpp"

//---------------------------------------------------------------------------//

template <class T> struct ComPtr;
namespace std
{
    typedef basic_string<char8_t> u8string;
}

//---------------------------------------------------------------------------//

extern const CLSID CLSID_Component;

//---------------------------------------------------------------------------//

namespace CubeMelon {

//---------------------------------------------------------------------------//

typedef std::unordered_multimap<std::u8string, ComPtr<CubeMelon::IComponent>> MessageMap;

//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS Component

// コンポーネントの基底クラス
class Component : public IComponent
{
public:
    explicit THIS(IUnknown* pUnkOuter = nullptr);
     ~THIS();

public:
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

public:
    U8CSTR       __stdcall api_version() const override;
    REFCLSID     __stdcall clsid()       const override;
    U8CSTR       __stdcall name()        const override;
    U8CSTR       __stdcall description() const override;
    CompVerInfo* __stdcall version()     const override;
    IComponent*  __stdcall owner()       const override;
    ICompProp*   __stdcall property()    const override;
    STATE        __stdcall status()      const override;

    HRESULT __stdcall AttachMessage(U8CSTR msg, IComponent* listener) override;
    HRESULT __stdcall DetachMessage(U8CSTR msg, IComponent* listener) override;
    HRESULT __stdcall NotifyMessage(U8CSTR msg, IComponent* sender, IComponent* listener, IData* data) override;
    HRESULT __stdcall CreateInstance(REFCLSID rclsid, REFIID riid, void** ppvObject) override;
    HRESULT __stdcall Start(void* args, IComponent* listener = nullptr) override;
    HRESULT __stdcall Stop (void* args, IComponent* listener = nullptr) override;

protected:
    ULONG       m_cRef       = 0;
    IComponent* m_owner      = nullptr;
    ICompProp*  m_property   = nullptr;
    STATE       m_state      = STATE::UNKNOWN;
    MessageMap  m_msg_map;

private:
    struct Impl;
    Impl* pimpl;

private:
    THIS(const THIS&)             = delete;
    THIS(THIS&&)                  = delete;
    THIS& operator =(const THIS&) = delete;
    THIS& operator =(THIS&&)      = delete;
};

#undef THIS

//---------------------------------------------------------------------------//

#define THIS ReaderComponent

// 入力コンポーネントの基底クラス
class ReaderComponent : public IReaderComponent
{
public:
    explicit THIS(IUnknown* pUnkOuter = nullptr);
     ~THIS();

public:
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

public:
    U8CSTR       __stdcall api_version() const override;
    REFCLSID     __stdcall clsid()       const override;
    U8CSTR       __stdcall name()        const override;
    U8CSTR       __stdcall description() const override;
    CompVerInfo* __stdcall version()     const override;
    IComponent*  __stdcall owner()       const override;
    ICompProp*   __stdcall property()    const override;
    STATE        __stdcall status()      const override;

    HRESULT __stdcall AttachMessage(U8CSTR msg, IComponent* listener) override;
    HRESULT __stdcall DetachMessage(U8CSTR msg, IComponent* listener) override;
    HRESULT __stdcall NotifyMessage(U8CSTR msg, IComponent* sender, IComponent* listener, IData* data) override;
    HRESULT __stdcall CreateInstance(REFCLSID rclsid, REFIID riid, void** ppvObject) override;
    HRESULT __stdcall Start(void* args, IComponent* listener = nullptr) override;
    HRESULT __stdcall Stop (void* args, IComponent* listener = nullptr) override;

public:
    HRESULT __stdcall Close(IComponent* listener = nullptr) override;
    HRESULT __stdcall Open(U8CSTR path, U8CSTR format_as, IComponent* listener = nullptr) override;
    HRESULT __stdcall QuerySupport(U8CSTR path, U8CSTR format_as) override;
    HRESULT __stdcall Seek(int64_t offset, uint32_t origin, uint64_t* new_pos) override;

public:
    HRESULT __stdcall Read(void* buffer, size_t buf_size, size_t* cb_data, IComponent* listener = nullptr) override;

protected:
    ULONG       m_cRef       = 0;
    IComponent* m_owner      = nullptr;
    ICompProp*  m_property   = nullptr;
    STATE       m_state      = STATE::UNKNOWN;
    MessageMap  m_msg_map;

protected:
    char8_t* m_path     = nullptr;
    char8_t* m_format   = nullptr;
    uint64_t m_position = 0;

private:
    struct Impl;
    Impl* pimpl;
};

#undef THIS

//---------------------------------------------------------------------------//

#define THIS WriterComponent

// 出力コンポーネントの基底クラス
class WriterComponent : public IWriterComponent
{
public:
    explicit THIS(IUnknown* pUnkOuter = nullptr);
     ~THIS();

public:
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

public:
    U8CSTR       __stdcall api_version() const override;
    REFCLSID     __stdcall clsid()       const override;
    U8CSTR       __stdcall name()        const override;
    U8CSTR       __stdcall description() const override;
    CompVerInfo* __stdcall version()     const override;
    IComponent*  __stdcall owner()       const override;
    ICompProp*   __stdcall property()    const override;
    STATE        __stdcall status()      const override;

    HRESULT __stdcall AttachMessage(U8CSTR msg, IComponent* listener) override;
    HRESULT __stdcall DetachMessage(U8CSTR msg, IComponent* listener) override;
    HRESULT __stdcall NotifyMessage(U8CSTR msg, IComponent* sender, IComponent* listener, IData* data) override;
    HRESULT __stdcall CreateInstance(REFCLSID rclsid, REFIID riid, void** ppvObject) override;
    HRESULT __stdcall Start(void* args, IComponent* listener = nullptr) override;
    HRESULT __stdcall Stop (void* args, IComponent* listener = nullptr) override;

public:
    HRESULT __stdcall Close(IComponent* listener = nullptr) override;
    HRESULT __stdcall Open(U8CSTR path, U8CSTR format_as, IComponent* listener = nullptr) override;
    HRESULT __stdcall QuerySupport(U8CSTR path, U8CSTR format_as) override;
    HRESULT __stdcall Seek(int64_t offset, uint32_t origin, uint64_t* new_pos) override;

public:
    HRESULT __stdcall Write(void* buffer, size_t buf_size, size_t* cb_data, IComponent* listener = nullptr) override;

protected:
    ULONG       m_cRef       = 0;
    IComponent* m_owner      = nullptr;
    ICompProp*  m_property   = nullptr;
    STATE       m_state      = STATE::UNKNOWN;
    MessageMap  m_msg_map;

protected:
    char8_t* m_path     = nullptr;
    char8_t* m_format   = nullptr;
    uint64_t m_position = 0;

private:
    struct Impl;
    Impl* pimpl;
};

#undef THIS

//---------------------------------------------------------------------------//

} // namespace CubeMelon

//---------------------------------------------------------------------------//

// ComponentBase.hpp