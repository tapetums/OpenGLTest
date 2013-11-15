// ComponentBase.Readert.cpp

//---------------------------------------------------------------------------//
//
// 入力コンポーネントの基底クラス
//   Copyright (C) 2013 tapetums
//
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

namespace CubeMelon {

//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS ReaderComponent

//---------------------------------------------------------------------------//

ReaderComponent::THIS(IUnknown* pUnkOuter)
{
    COMP_NAMEtoUTF16

    console_out(TEXT("%s::ctor begin"), name);

    if ( pUnkOuter )
    {
        console_out(TEXT("%s::GetOwnerInterface begin"), name);
        pUnkOuter->QueryInterface
        (
            IID_IComponent, (void**)&m_owner
        );
        console_out(TEXT("%s::GetOwnerInterface end"), name);
    }

    this->AddRef();

    console_out(TEXT("%s::ctor end"), name);
}

//---------------------------------------------------------------------------//

ReaderComponent::~THIS()
{
    COMP_NAMEtoUTF16

    console_out(TEXT("%s::dtor begin"), name);

    m_state = STATE::UNKNOWN;
    {
        if ( m_property )
        {
            m_property->Release();
            m_property = nullptr;
        }
        if ( m_owner )
        {
            console_out(TEXT("%s::ReleaseOwner begin"), name);
            {
                m_owner->Release();
                m_owner = nullptr;
            }
            console_out(TEXT("%s::ReleaseOwner end"), name);
        }
    }
    m_state = STATE::IDLE;

    console_out(TEXT("%s::dtor end"), name);
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ReaderComponent::QueryInterface
(
    REFIID riid, void** ppvObject
)
{
    COMP_NAMEtoUTF16

    console_out(TEXT("%s::QueryInterface() begin"), name);

    if ( nullptr == ppvObject )
    {
        return E_POINTER;
    }

    *ppvObject = nullptr;

    if ( IsEqualIID(riid, IID_IUnknown) )
    {
        console_out(TEXT("IID_IUnknown"));
        *ppvObject = static_cast<IUnknown*>(this);
    }
    else if ( IsEqualIID(riid, IID_IComponent) )
    {
        console_out(TEXT("IID_IComponent"));
        *ppvObject = static_cast<IComponent*>(this);
    }
    else if ( IsEqualIID(riid, IID_IIOComponent) )
    {
        console_out(TEXT("IID_IIOComponent"));
        *ppvObject = static_cast<IIOComponent*>(this);
    }
    else if ( IsEqualIID(riid, IID_IReaderComponent) )
    {
        console_out(TEXT("IID_IReaderComponent"));
        *ppvObject = static_cast<IReaderComponent*>(this);
    }
    else
    {
        console_out(TEXT("No such an interface"));
        console_out(TEXT("%s::QueryInterface() end"), name);
        return E_NOINTERFACE;
    }

    this->AddRef();

    console_out(TEXT("%s::QueryInterface() end"), name);

    return S_OK;
}

//---------------------------------------------------------------------------//

ULONG __stdcall ReaderComponent::AddRef()
{
    COMP_NAMEtoUTF16

    LONG cRef = ::InterlockedIncrement(&m_cRef);

    console_out(TEXT("%s::AddRef(): %d -> %d"), name, cRef - 1, cRef);

    return static_cast<ULONG>(cRef);
}

//---------------------------------------------------------------------------//

ULONG __stdcall ReaderComponent::Release()
{
    COMP_NAMEtoUTF16

    if ( m_cRef < 1 )
    {
        console_out(TEXT("%s::Release() %d"), name, m_cRef);
        return m_cRef;
    }

    LONG cRef = ::InterlockedDecrement(&m_cRef);

    console_out(TEXT("%s::Release(): %d -> %d"), name, cRef + 1, cRef);

    if ( cRef == 0 )
    {
        console_out(TEXT("%s::delete begin"), name);
        {
            delete this;
        }
        console_out(TEXT("%s::delete end"), name);
    }

    return static_cast<ULONG>(cRef);
}

//---------------------------------------------------------------------------//

U8CSTR __stdcall ReaderComponent::api_version() const
{
    return API_VERSION;
}

//---------------------------------------------------------------------------//

REFCLSID __stdcall ReaderComponent::clsid() const
{
    return CLSID_Component;
}

//---------------------------------------------------------------------------//

U8CSTR __stdcall ReaderComponent::name() const
{
    return (const char8_t*)"ReaderComponent";
}

//---------------------------------------------------------------------------//

U8CSTR __stdcall ReaderComponent::description() const
{
    return (const char8_t*)"Component Description Here";
}

//---------------------------------------------------------------------------//

CompVerInfo* __stdcall ReaderComponent::version() const
{
    static CompVerInfo version = { 0, 0, 0, 'a' };

    return &version;
}

//---------------------------------------------------------------------------//

IComponent* __stdcall ReaderComponent::owner() const
{
    if ( m_owner )
    {
        m_owner->AddRef();
    }

    return m_owner;
}

//---------------------------------------------------------------------------//

ICompProp* __stdcall ReaderComponent::property() const
{
    if ( m_property )
    {
        m_property->AddRef();
    }

    return m_property;
}

//---------------------------------------------------------------------------//

STATE __stdcall ReaderComponent::status() const
{
    return m_state;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ReaderComponent::AttachMessage
(
    U8CSTR msg, IComponent* listener
)
{
    COMP_NAMEtoUTF16

    console_out(TEXT("%s::AttachMessage() begin"), name);

    if ( nullptr == listener )
    {
        return E_POINTER;
    }

    console_out(TEXT("msg: %s"),      UTFConvert(msg).toUTF16());
    console_out(TEXT("listener: %s"), UTFConvert(listener->name()).toUTF16());

    const auto range = m_msg_map.equal_range(std::u8string(msg));
    const auto end = range.second;
    auto it = range.first;
    while ( it != end )
    {
        if ( it->second == listener )
        {
            console_out(TEXT("This message has been already attached to the component"));
            console_out(TEXT("%s::AttachMessage() end"), name);
            return S_FALSE;
        }
        ++it;
    }

    m_msg_map.emplace(msg, listener);

    console_out(TEXT("%s::AttachMessage() end"), name);

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ReaderComponent::DetachMessage
(
    U8CSTR msg, IComponent* listener
)
{
    COMP_NAMEtoUTF16

    console_out(TEXT("%s::DetachMessage() begin"), name);

    if ( nullptr == listener )
    {
        return E_POINTER;
    }

    console_out(TEXT("msg: %s"),      UTFConvert(msg).toUTF16());
    console_out(TEXT("listener: %s"), UTFConvert(listener->name()).toUTF16());

    size_t count = 0;
    const auto range = m_msg_map.equal_range(std::u8string(msg));
    const auto end = range.second;
    auto it = range.first;
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
        console_out(TEXT("This message was not attached to the component"));
        console_out(TEXT("%s::DetachMessage() end"), name);
        return S_FALSE;
    }

    console_out(TEXT("%s::DetachMessage() end"), name);

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ReaderComponent::NotifyMessage
(
    U8CSTR msg, IComponent* sender, IComponent* listener, IData* data
)
{
    COMP_NAMEtoUTF16

    console_out(TEXT("%s::NotifyMessage() begin"), name);

    // スマートポインタにオブジェクトを代入 ... Release() 忘れ防止
    ComPtr<IComponent> s(sender);
    ComPtr<IComponent> l(listener);
    ComPtr<IData>      d(data);

    // 以下、デバッグ情報を吐き出すだけ
    #if defined(_DEBUG) || (DEBUG)
    {
        console_out(TEXT("msg: %s"), UTFConvert(msg).toUTF16());

        if ( sender )
        {
            console_out(TEXT("sender: %s"), UTFConvert(sender->name()).toUTF16());
        }
        if ( listener )
        {
            console_out(TEXT("listener: %s"), UTFConvert(listener->name()).toUTF16());
        }
        if ( data )
        {
            console_out(TEXT("data: %s"), UTFConvert(data->name()).toUTF16());
        }
    }
    #endif

    console_out(TEXT("%s::NotifyMessage() end"), name);

    return E_NOTIMPL;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ReaderComponent::CreateInstance
(
    REFCLSID rclsid, REFIID riid, void** ppvObject
)
{
    COMP_NAMEtoUTF16

    console_out(TEXT("%s::CreateInstance() begin"), name);

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

    console_out(TEXT("%s::CreateInstance() end"), name);

    return hr;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ReaderComponent::Start
(
    void* args, IComponent* listener
)
{
    COMP_NAMEtoUTF16

    console_out(TEXT("%s:Start(): Not Implemented"), name);
    return E_NOTIMPL;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ReaderComponent::Stop
(
    void* args, IComponent* listener
)
{
    COMP_NAMEtoUTF16

    console_out(TEXT("%s:Stop(): Not Implemented"), name);
    return E_NOTIMPL;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ReaderComponent::Close
(
    IComponent* listener
)
{
    return E_NOTIMPL;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ReaderComponent::Open
(
    U8CSTR path, U8CSTR format_as, IComponent* listener
)
{
    return E_NOTIMPL;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ReaderComponent::QuerySupport
(
    U8CSTR path, U8CSTR format_as
)
{
    return E_NOTIMPL;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ReaderComponent::Seek
(
    int64_t offset, uint32_t origin, uint64_t* new_pos
)
{
    return E_NOTIMPL;
}

//---------------------------------------------------------------------------//

HRESULT __stdcall ReaderComponent::Read
(
    void* buffer, size_t buf_size, size_t* cb_data, IComponent* listener
)
{
    return E_NOTIMPL;
}

//---------------------------------------------------------------------------//

} // namespace CubeMelon

//---------------------------------------------------------------------------//

// ComponentBase.Readert.cpp