// ComponentBase.hpp

#pragma once

//---------------------------------------------------------------------------//
//
// コンポーネントの基底クラス
//   Copyright (C) 2013 tapetums
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

namespace CubeMelon {

//---------------------------------------------------------------------------//

typedef std::unordered_multimap<std::u8string, ComPtr<CubeMelon::IComponent>> MessageMap;

//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS ComponentBase

class ComponentBase : public IComponent
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

} // namespace CubeMelon

//---------------------------------------------------------------------------//

// ComponentBase.hpp