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
#pragma once
#if (!defined(_MazeSoundManager_hpp_))
#define _MazeSoundManager_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-sound/MazeSoundSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundManager);
    MAZE_USING_SHARED_PTR(SoundSystem);

    //////////////////////////////////////////
    // Class SoundManager
    //
    //////////////////////////////////////////
    class MAZE_SOUND_API SoundManager
    {
    public:

        //////////////////////////////////////////
        virtual ~SoundManager();

        //////////////////////////////////////////
        static void Initialize(SoundManagerPtr& _soundManager);


        //////////////////////////////////////////
        static inline SoundManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline SoundManager& GetInstance() { return *s_instance; }       


        //////////////////////////////////////////
        void addSoundSystem(SoundSystemPtr const& _soundSystem);

        //////////////////////////////////////////
        void removeSoundSystem(SoundSystemPtr const& _soundSystem);


        //////////////////////////////////////////
        inline UnorderedMap<String, SoundSystemPtr> const& getSoundSystems() const { return m_soundSystems; }


        //////////////////////////////////////////
        inline SoundSystemPtr const& getDefaultSoundSystem() const { return m_defaultSoundSystem; }

        //////////////////////////////////////////
        inline SoundSystem* getDefaultSoundSystemRaw() const { return m_defaultSoundSystem.get(); }

        //////////////////////////////////////////
        void setDefaultSoundSystem(SoundSystemPtr const& _soundSystem);

    protected:

        //////////////////////////////////////////
        SoundManager();

        //////////////////////////////////////////
        bool init();

    protected:
        static SoundManager* s_instance;

        UnorderedMap<String, SoundSystemPtr> m_soundSystems;
        SoundSystemPtr m_defaultSoundSystem;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundManager_hpp_
//////////////////////////////////////////
