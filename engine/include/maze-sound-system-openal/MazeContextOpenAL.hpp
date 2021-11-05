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
#if (!defined(_MazeContextOpenAL_hpp_))
#define _MazeContextOpenAL_hpp_


//////////////////////////////////////////
#include "maze-sound-system-openal/MazeSoundSystemOpenALHeader.hpp"
#include "maze-sound-system-openal/MazeHeaderOpenAL.hpp"
#include "maze-sound/MazeSoundSystem.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ContextOpenAL);
    MAZE_USING_SHARED_PTR(SoundSystemOpenAL);


    //////////////////////////////////////////
    // Class ContextOpenAL
    //
    //////////////////////////////////////////
    class MAZE_SOUND_SYSTEM_OPENAL_API ContextOpenAL
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ContextOpenAL);

        //////////////////////////////////////////
        using ALFunctionPointer = void (*)();

    public:

        //////////////////////////////////////////
        virtual ~ContextOpenAL();

        //////////////////////////////////////////
        static ContextOpenALPtr Create(SoundSystemOpenALPtr const& _soundSystem = nullptr, S32 _deviceIndex = -1);


        //////////////////////////////////////////
        virtual ALFunctionPointer getFunction(CString _name) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool makeCurrent();


        //////////////////////////////////////////
        inline S32 getDeviceIndex() const { return m_deviceIndex; }

        //////////////////////////////////////////
        inline ALCdevice* getDevice() const { return m_device; }

        //////////////////////////////////////////
        inline ALCcontext* getContext() const { return m_context; }


    protected:

        //////////////////////////////////////////
        ContextOpenAL();

        //////////////////////////////////////////
        virtual bool init(SoundSystemOpenALPtr const& _soundSystem, S32 _deviceIndex = -1);

        //////////////////////////////////////////
        bool createALContext();

        //////////////////////////////////////////
        void destroyALContext();

    protected:

        SoundSystemOpenALWPtr m_soundSystemWeak;
        SoundSystemOpenAL* m_soundSystemRaw = nullptr;

        S32 m_deviceIndex = -1;

        ALCdevice* m_device = nullptr;
        ALCcontext* m_context = nullptr;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeContextOpenAL_hpp_
//////////////////////////////////////////
