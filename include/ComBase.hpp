// ComBase.hpp

#pragma once

//---------------------------------------------------------------------------//
//
// COM オブジェクトの基底クラス
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#include <windows.h>

//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS ComBase

class ComBase : public IUnknown
{
public:
    THIS();
    ~THIS();

    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

protected:
    ULONG m_cRef = 0;

private:
    THIS(const THIS&)             = delete;
    THIS(THIS&&)                  = delete;
    THIS& operator= (const THIS&) = delete;
    THIS& operator= (THIS&&)      = delete;
};

#undef THIS

//---------------------------------------------------------------------------//

// ComBase.hpp