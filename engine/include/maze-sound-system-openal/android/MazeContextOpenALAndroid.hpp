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
// subject to the folloAndroidg restrictions:
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
#if (!defined(_MazeContextOpenALAndroid_hpp_))
#define _MazeContextOpenALAndroid_hpp_


//////////////////////////////////////////
#include "maze-sound-system-openal/MazeSoundSystemOpenALHeader.hpp"
#include "maze-sound-system-openal/MazeContextOpenAL.hpp"
#include "maze-sound-system-openal/MazeHeaderOpenAL.hpp"
#include "maze-sound/MazeSoundSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ContextOpenALAndroid);


    //////////////////////////////////////////
    // Class ContextOpenALAndroid
    //
    //////////////////////////////////////////
    class MAZE_SOUND_SYSTEM_OPENAL_API ContextOpenALAndroid
        : public ContextOpenAL
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ContextOpenALAndroid, ContextOpenAL);

    public:

        //////////////////////////////////////////
        virtual ~ContextOpenALAndroid();

        //////////////////////////////////////////
        static ContextOpenALAndroidPtr Create(
            SoundSystemOpenALPtr const& _soundSystem,
            S32 _deviceIndex = -1);


        //////////////////////////////////////////
        virtual ALFunctionPointer getFunction(CString _name) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ContextOpenALAndroid();

        //////////////////////////////////////////
        virtual bool init(
            SoundSystemOpenALPtr const& _soundSystem,
            S32 _deviceIndex = -1) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void setup();


        //////////////////////////////////////////
        void notifyFunctionsAssigned();

    protected:
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeContextOpenALAndroid_hpp_
//////////////////////////////////////////
