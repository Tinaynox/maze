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
#include "MazeSoundSystemOpenALHeader.hpp"
#include "maze-sound-system-openal/win/MazeContextOpenALWin.hpp"
#include "maze-sound-system-openal/MazeSoundOpenAL.hpp"
#include "maze-sound-system-openal/MazeSoundSystemOpenAL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ContextOpenALWin
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ContextOpenALWin, ContextOpenAL);

    //////////////////////////////////////////
    ContextOpenALWin::ContextOpenALWin()
    {
    }

    //////////////////////////////////////////
    ContextOpenALWin::~ContextOpenALWin()
    {
        unloadOALLibrary();

        if (m_soundSystemRaw)
            m_soundSystemRaw->eventFunctionsAssigned.unsubscribe(this);
    }

    //////////////////////////////////////////
    ContextOpenALWinPtr ContextOpenALWin::Create(
        SoundSystemOpenALPtr const& _soundSystem,
        S32 _deviceIndex)
    {
        ContextOpenALWinPtr soundSystem;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextOpenALWin, soundSystem, init(_soundSystem, _deviceIndex));
        return soundSystem;
    }

    //////////////////////////////////////////
    bool ContextOpenALWin::init(
        SoundSystemOpenALPtr const& _soundSystem,
        S32 _deviceIndex)
    {
        if (!ContextOpenAL::init(_soundSystem, _deviceIndex))
            return false;
        
        if (!loadOALLibrary())
            return false;

        if (_soundSystem->getFunctionsAssigned())
            setup();
        else
            _soundSystem->eventFunctionsAssigned.subscribe(this, &ContextOpenALWin::notifyFunctionsAssigned);

        return true;
    }

    //////////////////////////////////////////
    void ContextOpenALWin::setup()
    {
        if (m_deviceIndex < 0)
            return;

        createALContext();
    }

    //////////////////////////////////////////
    ContextOpenAL::ALFunctionPointer ContextOpenALWin::getFunction(CString _name)
    {
        if (!m_openALDLL)
            return nullptr;

        return reinterpret_cast<ALFunctionPointer>(GetProcAddress(m_openALDLL, reinterpret_cast<LPCSTR>(_name)));
    }

    //////////////////////////////////////////
    bool ContextOpenALWin::loadOALLibrary(Path const& _libFullPath)
    {
        if (!_libFullPath.empty())
            m_openALDLL = LoadLibraryW(_libFullPath.c_str());
        else
            m_openALDLL = LoadLibraryW(L"openal32.dll");

        if (!m_openALDLL)
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ContextOpenALWin::unloadOALLibrary()
    {
        if (!m_openALDLL)
            return;

        FreeLibrary(m_openALDLL);
        m_openALDLL = NULL;
    }

    //////////////////////////////////////////
    void ContextOpenALWin::notifyFunctionsAssigned()
    {
        setup();
    }

} // namespace Maze
//////////////////////////////////////////
