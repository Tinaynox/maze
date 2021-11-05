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
#include "maze-sound-system-openal/MazeSoundGroupOpenAL.hpp"
#include "maze-sound-system-openal/MazeFunctionsOpenAL.hpp"
#include "maze-sound/MazeSoundSystem.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SoundGroupOpenAL
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SoundGroupOpenAL, SoundGroup);

    //////////////////////////////////////////
    SoundGroupOpenAL::SoundGroupOpenAL()
    {
    }

    //////////////////////////////////////////
    SoundGroupOpenAL::~SoundGroupOpenAL()
    {
        deleteALObjects();
    }

    //////////////////////////////////////////
    SoundGroupOpenALPtr SoundGroupOpenAL::Create(SoundSystem* _soundSystem)
    {
        if (!_soundSystem)
            _soundSystem = SoundSystem::GetCurrentInstancePtr();

        SoundGroupOpenALPtr sound;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SoundGroupOpenAL, sound, init(_soundSystem));
        return sound;
    }


    //////////////////////////////////////////
    bool SoundGroupOpenAL::init(SoundSystem* _soundSystem)
    {
        if (!SoundGroup::init(_soundSystem))
            return false;
        
        generateALObjects();

        return true;
    }

    //////////////////////////////////////////
    void SoundGroupOpenAL::generateALObjects()
    {
        /*
        if (m_bufferID != 0)
        {
            deleteALObjects();
        }

        MAZE_AL_CALL(mzalGenBuffers(1, &m_bufferID));
        */
    }

    //////////////////////////////////////////
    void SoundGroupOpenAL::deleteALObjects()
    {
        /*
        if (m_bufferID == 0)
            return;

        MAZE_AL_CALL(mzalDeleteBuffers(1, &m_bufferID));
        m_bufferID = 0;
        */
    }

} // namespace Maze
//////////////////////////////////////////
