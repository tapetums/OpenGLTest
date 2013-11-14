// SpiManager.cpp

//---------------------------------------------------------------------------//
//
// Susie プラグイン管理クラス
//   Copyright (C) 2013 tapetums
//
//---------------------------------------------------------------------------//

#ifdef THIS
#undef THIS
#endif

#define THIS SpiManager

//---------------------------------------------------------------------------//

#include <vector>

#include "Susie.hpp"

//---------------------------------------------------------------------------//

struct THIS::Impl
{
    std::vector<Susie*> plugins;

    ~Impl()
    {
        for ( auto plugin : plugins ) delete plugin;
    }
};

//---------------------------------------------------------------------------//

SpiManager::THIS()
{
    pimpl = new Impl;
}

//---------------------------------------------------------------------------//

SpiManager::~THIS()
{
    delete pimpl;
    pimpl = nullptr;
}

//---------------------------------------------------------------------------//

Susie* __stdcall SpiManager::Append(LPCWSTR spi_path)
{
    auto susie = new Susie;

    if ( false == susie->Load(spi_path) )
    {
        delete susie;
        susie = nullptr;
    }
    else
    {
        pimpl->plugins.push_back(susie);
    }

    return susie;
}

//---------------------------------------------------------------------------//

void __stdcall SpiManager::Remove(LPCWSTR spi_path)
{
    auto it  = pimpl->plugins.begin();
    auto end = pimpl->plugins.end();

    while ( it != end )
    {
        if ( lstrcmp((*it)->SpiPath(), spi_path) == 0 )
        {
            auto susie = *it;
            pimpl->plugins.erase(it);
            delete susie;
            break;
        }
        ++it;
    }
}

//---------------------------------------------------------------------------//

size_t __stdcall SpiManager::SpiCount() const
{
    return pimpl->plugins.size();
}

//---------------------------------------------------------------------------//

Susie* __stdcall SpiManager::GetAt(size_t index) const
{
    Susie* susie = nullptr;

    try
    {
        susie = pimpl->plugins.at(index);
    }
    catch ( std::out_of_range& e )
    {
        OutputDebugStringA(e.what());
        OutputDebugStringA("\n");
    }

    return susie;
}

//---------------------------------------------------------------------------//

Susie* __stdcall SpiManager::QueryAvailableSpi(LPCWSTR filename, void* dw) const
{
    Susie* susie = nullptr;

    for ( auto plugin : pimpl->plugins )
    {
        if ( nullptr == plugin ) continue;

        auto result = plugin->IsSupported(filename, dw);
        if ( result == SPI_SUPPORTED )
        {
            susie = plugin;
            break;
        }
    }

    return susie;
}

//---------------------------------------------------------------------------//

#undef THIS

//---------------------------------------------------------------------------//

// SpiManager.cpp