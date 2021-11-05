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
#if (!defined(_MazeSoundSource_hpp_))
#define _MazeSoundSource_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-sound/MazeSoundData.hpp"
#include "maze-sound/MazeSoundSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundSource);


    //////////////////////////////////////////
    // Class SoundSource
    //
    //////////////////////////////////////////
    class MAZE_SOUND_API SoundSource
        : public SharedObject<SoundSource>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(SoundSource);

    public:

        //////////////////////////////////////////
        virtual ~SoundSource();

        //////////////////////////////////////////
        inline void setCycled(bool _cycled) { m_cycled = _cycled; }

        //////////////////////////////////////////
        inline bool getCycled() const { return m_cycled; }


        //////////////////////////////////////////
        virtual void play() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void stop() MAZE_ABSTRACT;

    protected:

        //////////////////////////////////////////
        SoundSource();

        //////////////////////////////////////////
        bool init(SoundPtr const& _sound, SoundSystem* _soundSystem);

    protected:
        SoundSystem* m_soundSystem = nullptr;
        SoundPtr m_sound;

        bool m_cycled = false;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundSource_hpp_
//////////////////////////////////////////
