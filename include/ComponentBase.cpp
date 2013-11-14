// ComponentBase.cpp

//---------------------------------------------------------------------------//
//
// コンポーネントの基底クラス
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS ComponentBase

//---------------------------------------------------------------------------//

#include "ComPtr.h"
#include "DebugPrint.h"

#include "ComponentBase.hpp"
#include "UTFConvert.hpp"

//---------------------------------------------------------------------------//

#if defined(_DEBUG) || (DEBUG)
#define COMP_NAMEtoUTF16 \
    UTFConvert cvt_name(this->name());\
    auto name = cvt_name.toUTF16();
#else
#define COMP_NAMEtoUTF16
#endif

//---------------------------------------------------------------------------//

extern CLSID CLSID_Component = GUID_NULL;

//---------------------------------------------------------------------------//

namespace CubeMelon {

//---------------------------------------------------------------------------//

ComponentBase::THIS(IUnknown* pUnkOuter)
{
    COMP_NAMEtoUTF16

    DebugPrintLn(TEXT("%s::ctor begin"), name);

    if ( pUnkOuter )
    {
        DebugPrintLn(TEXT("%s::GetOwnerInterface begin"), name);
        pUnkOuter->QueryInterface
        (
            IID_IComponent, (void**)&m_owner
        );
        DebugPrintLn(TEXT("%s::GetOwnerInterface end"), name);
    }

    this->AddRef();

    /*struct AFO : public IData
    {
        AFO()  { DebugPrintLn(TEXT("AFO()")); }
        ~AFO() { DebugPrintLn(TEXT("~AFO()")); }
        STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override { return E_NOTIMPL; }
        STDMETHODIMP_(ULONG) AddRef() override { return 1; }
        STDMETHODIMP_(ULONG) Release() override { delete this; return 0; }
        U8CSTR  __stdcall name() const { return (const char8_t*)"afo"; }
        size_t  __stdcall size() const { return 0; }
        void*   __stdcall get()            { return nullptr; }
        HRESULT __stdcall set(void* value) { return E_NOTIMPL; }
    };

    this->DetachMessage(MSG_COMP_START_DONE, this);
    this->AttachMessage(MSG_COMP_START_DONE, this);
    this->AttachMessage(MSG_COMP_START_DONE, this);
    this->NotifyMessage(MSG_COMP_START_DONE, nullptr, nullptr, new AFO);
    this->DetachMessage(MSG_COMP_START_DONE, this);*/

    DebugPrintLn(TEXT("%s::ctor end"), name);
}

//---------------------------------------------------------------------------//

ComponentBase::~THIS()
{
    COMP_NAMEtoUTF16

    DebugPrintLn(TEXT("%s::dtor begin"), name);

    m_state = STATE::UNKNOWN;
    {
        if ( m_property )
        {
            m_property->Release();
            m_property = nullptr;
        }
        if ( m_owner )
        {
            DebugPrintLn(TEXT("%s::ReleaseOwner begin"), name);
            {
                m_owner->Release();
                m_owner = nullptr;
            }
            DebugPrintLn(TEXT("%s::ReleaseOwner end"), name);
        }
    }
    m_state = STATE::IDLE;

    DebugPrintLn(TEXT("%s::dtor end"), name);
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ComponentBase::QueryInterface
(
    REFIID riid, void** ppvObject
)
{
    COMP_NAMEtoUTF16

    DebugPrintLn(TEXT("%s::QueryInterface() begin"), name);

    if ( nullptr == ppvObject )
    {
        return E_POINTER;
    }

    *ppvObject = nullptr;

    if ( IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IComponent) )
    {
        DebugPrintLn(TEXT("IID_IComponent"));
        *ppvObject = static_cast<IComponent*>(this);
    }
    else
    {
        DebugPrintLn(TEXT("No such an interface"));
        DebugPrintLn(TEXT("%s::QueryInterface() end"), name);
        return E_NOINTERFACE;
    }

    this->AddRef();

    DebugPrintLn(TEXT("%s::QueryInterface() end"), name);

    return S_OK;
}

//---------------------------------------------------------------------------//

ULONG __stdcall ComponentBase::AddRef()
{
    COMP_NAMEtoUTF16

    LONG cRef = ::InterlockedIncrement(&m_cRef);

    DebugPrintLn(TEXT("%s::AddRef(): %d -> %d"), name, cRef - 1, cRef);

    return static_cast<ULONG>(cRef);
}

//---------------------------------------------------------------------------//

ULONG __stdcall ComponentBase::Release()
{
    COMP_NAMEtoUTF16

    if ( m_cRef < 1 )
    {
        DebugPrintLn(TEXT("%s::Release() %d"), name, m_cRef);
        return m_cRef;
    }

    LONG cRef = ::InterlockedDecrement(&m_cRef);

    DebugPrintLn(TEXT("%s::Release(): %d -> %d"), name, cRef + 1, cRef);

    if ( cRef == 0 )
    {
        DebugPrintLn(TEXT("%s::delete begin"), name);
        {
            delete this;
        }
        DebugPrintLn(TEXT("%s::delete end"), name);
    }

    return static_cast<ULONG>(cRef);
}

//---------------------------------------------------------------------------//

U8CSTR __stdcall ComponentBase::api_version() const
{
    return API_VERSION;
}

//---------------------------------------------------------------------------//

REFCLSID __stdcall ComponentBase::clsid() const
{
    return CLSID_Component;
}

//---------------------------------------------------------------------------//

U8CSTR __stdcall ComponentBase::name() const
{
    return (const char8_t*)"Unknown";
}

//---------------------------------------------------------------------------//

U8CSTR __stdcall ComponentBase::description() const
{
    return (const char8_t*)"Component Description Here";
}

//---------------------------------------------------------------------------//

CompVerInfo* __stdcall ComponentBase::version() const
{
    static CompVerInfo version = { 1, 0, 0, 'a' };

    return &version;
}

//---------------------------------------------------------------------------//

IComponent* __stdcall ComponentBase::owner() const
{
    if ( m_owner )
    {
        m_owner->AddRef();
    }

    return m_owner;
}

//---------------------------------------------------------------------------//

ICompProp* __stdcall ComponentBase::property() const
{
    if ( m_property )
    {
        m_property->AddRef();
    }

    return m_property;
}

//---------------------------------------------------------------------------//

STATE __stdcall ComponentBase::status() const
{
    return m_state;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ComponentBase::AttachMessage
(
    U8CSTR msg, IComponent* listener
)
{
    COMP_NAMEtoUTF16

    DebugPrintLn(TEXT("%s::AttachMessage() begin"), name);

    if ( nullptr == listener )
    {
        return E_POINTER;
    }

    DebugPrintLn(TEXT("msg: %s"),      UTFConvert(msg).toUTF16());
    DebugPrintLn(TEXT("listener: %s"), UTFConvert(listener->name()).toUTF16());

    const auto range = m_msg_map.equal_range(std::u8string(msg));
    const auto end = range.second;
    auto it  = range.first;
    while ( it != end )
    {
        if ( it->second == listener )
        {
            DebugPrintLn(TEXT("This message has been already attached to the component"));
            DebugPrintLn(TEXT("%s::AttachMessage() end"), name);
            return S_FALSE;
        }
        ++it;
    }

    m_msg_map.emplace(msg, listener);

    DebugPrintLn(TEXT("%s::AttachMessage() end"), name);

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ComponentBase::DetachMessage
(
    U8CSTR msg, IComponent* listener
)
{
    COMP_NAMEtoUTF16

    DebugPrintLn(TEXT("%s::DetachMessage() begin"), name);

    if ( nullptr == listener )
    {
        return E_POINTER;
    }

    DebugPrintLn(TEXT("msg: %s"),      UTFConvert(msg).toUTF16());
    DebugPrintLn(TEXT("listener: %s"), UTFConvert(listener->name()).toUTF16());

    size_t count = 0;
    const auto range = m_msg_map.equal_range(std::u8string(msg));
    const auto end = range.second;
    auto it  = range.first;
     while ( it != end )
    {
        if ( it->second == listener )
        {
            m_msg_map.erase(it);
            ++count;
            break;
        }
        ++it;
    }
    if ( count == 0 )
    {
        DebugPrintLn(TEXT("This message was not attached to the component"));
        DebugPrintLn(TEXT("%s::DetachMessage() end"), name);
        return S_FALSE;
    }

    DebugPrintLn(TEXT("%s::DetachMessage() end"), name);

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ComponentBase::NotifyMessage
(
    U8CSTR msg, IComponent* sender, IComponent* listener, IData* data
)
{
    COMP_NAMEtoUTF16

    DebugPrintLn(TEXT("%s::NotifyMessage() begin"), name);

    // スマートポインタにオブジェクトを代入 ... Release() 忘れ防止
    ComPtr<IComponent> s(sender,   false);
    ComPtr<IComponent> l(listener, false);
    ComPtr<IData>      d(data,     false);

    // 以下、デバッグ情報を吐き出すだけ
    #if defined(_DEBUG) || (DEBUG)
    {
        DebugPrintLn(TEXT("msg: %s"), UTFConvert(msg).toUTF16());

        if ( sender )
        {
            DebugPrintLn(TEXT("sender: %s"), UTFConvert(sender->name()).toUTF16());
        }
        if ( listener )
        {
            DebugPrintLn(TEXT("listener: %s"), UTFConvert(listener->name()).toUTF16());
        }
        if ( data )
        {
            DebugPrintLn(TEXT("data: %s"), UTFConvert(data->name()).toUTF16());
        }
    }
    #endif

    DebugPrintLn(TEXT("%s::NotifyMessage() end"), name);

    return E_NOTIMPL;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ComponentBase::CreateInstance
(
    REFCLSID rclsid, REFIID riid, void** ppvObject
)
{
    COMP_NAMEtoUTF16

    DebugPrintLn(TEXT("%s::ComponentBase() begin"), name);

    HRESULT hr;

    if ( IsEqualCLSID(rclsid, CLSID_Component) )
    {
        hr = this->QueryInterface(riid, ppvObject);
    }
    else
    {
        if ( m_owner )
        {
            hr = m_owner->CreateInstance(rclsid, riid, ppvObject);
        }
        else
        {
            hr = E_NOTIMPL;
        }
    }

    DebugPrintLn(TEXT("%s::ComponentBase() end"), name);

    return hr;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ComponentBase::Start
(
    void* args, IComponent* listener
)
{
    COMP_NAMEtoUTF16

    DebugPrintLn(TEXT("%s:Start(): Not Implemented"), name);
    return E_NOTIMPL;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ComponentBase::Stop
(
    void* args, IComponent* listener
)
{
    COMP_NAMEtoUTF16

    DebugPrintLn(TEXT("%s:Stop(): Not Implemented"), name);
    return E_NOTIMPL;
}

//---------------------------------------------------------------------------//

} // namespace CubeMelon

//---------------------------------------------------------------------------//

#undef THIS

//---------------------------------------------------------------------------//

// ComponentBase.cpp