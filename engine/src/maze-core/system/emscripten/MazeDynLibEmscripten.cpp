//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeCoreHeader.hpp"
#include "maze-core/system/emscripten/MazeDynLibEmscripten.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include <dlfcn.h>
#include <emscripten/emscripten.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class DynLibEmscripten
    //
    //////////////////////////////////////////
    DynLibEmscripten::DynLibEmscripten()
        : m_handle(nullptr)
    {
    }

    //////////////////////////////////////////
    DynLibEmscripten::~DynLibEmscripten()
    {
    }


    //////////////////////////////////////////
    bool DynLibEmscripten::init(String const& _libraryFullPath)
    {
        if (!DynLib::init(_libraryFullPath))
            return false;


        return true;
    }
    
    //////////////////////////////////////////
    bool DynLibEmscripten::load()
    {
        Debug::log << "Loading dynamic library: " << m_libraryFullPath << "..." << endl;
        
        String libraryFullPath = m_libraryFullPath;
        
        if (libraryFullPath.find(".js") != String::npos)
        {
            libraryFullPath += ".js";
        }

        m_handle = dlopen(libraryFullPath.c_str(), RTLD_LAZY | RTLD_LOCAL);
        MAZE_ERROR_RETURN_VALUE_IF(m_handle == nullptr, false, "Failed to load dynamic library: %s", m_libraryFullPath.c_str());

        Debug::log << "Dynamic library loaded: " << m_libraryFullPath << endl;
        
        return true;
    }

    //////////////////////////////////////////
    void DynLibEmscripten::unload()
    {
        if (m_handle == nullptr)
            return;

        Debug::log << "Unloading dynamic library: " << m_libraryFullPath << "..." << endl;
        
        if (dlclose(m_handle))
        {
            MAZE_ERROR("Failed to unload dynamic library: %s", m_libraryFullPath.c_str());
        }
        else
        {
            Debug::log << "Dynamic library unloaded: " << m_libraryFullPath << endl;
        }
        
        m_handle = nullptr;
    }

    //////////////////////////////////////////
    void* DynLibEmscripten::getSymbol(String const& _symbolName)
    {
        return (void*)dlsym(m_handle, _symbolName.c_str());
    }
    
} // namespace Maze
//////////////////////////////////////////
