// ComBase.cpp

//---------------------------------------------------------------------------//
//
// COM オブジェクトの基底クラス
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS ComBase

//---------------------------------------------------------------------------//

#include "ComBase.hpp"

//---------------------------------------------------------------------------//

ComBase::THIS()
{
    this->AddRef();
}

//---------------------------------------------------------------------------//

ComBase::~THIS()
{
}

//---------------------------------------------------------------------------//

STDMETHODIMP ComBase::QueryInterface(REFIID riid, void** ppvObject)
{
    if ( nullptr == ppvObject )
    {
        return E_POINTER;
    }

    *ppvObject = nullptr;

    if ( IsEqualIID(riid, IID_IUnknown) )
    {
        *ppvObject = static_cast<IComponent*>(this);
    }
    else
    {
        return E_NOINTERFACE;
    }

    this->AddRef();

    return S_OK;
}

//---------------------------------------------------------------------------//

STDMETHODIMP_(ULONG) ComBase::AddRef()
{
    LONG cRef = ::InterlockedIncrement(&m_cRef);

    return static_cast<ULONG>(cRef);
}

//---------------------------------------------------------------------------//

STDMETHODIMP_(ULONG) ComBase::Release()
{
    LONG cRef = ::InterlockedDecrement(&m_cRef);
    if ( cRef == 0 )
    {
        delete this;
    }

    return static_cast<ULONG>(cRef);
}

//---------------------------------------------------------------------------//

#undef THIS

//---------------------------------------------------------------------------//

// ComBase.cpp