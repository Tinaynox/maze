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
#if (!defined(_MazeAudioListener_hpp_))
#define _MazeAudioListener_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeVec3.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AudioListener);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    // Class AudioListener
    //
    // Unity-like audio listener component. Exactly one AudioListener is
    // "active" at a time (matches Unity semantics) - its Transform3D position/
    // orientation is what 3D AudioSources are heard relative to.
    //
    //////////////////////////////////////////
    class MAZE_SOUND_API AudioListener
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AudioListener, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(AudioListener);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~AudioListener();

        //////////////////////////////////////////
        static AudioListenerPtr Create();


        //////////////////////////////////////////
        void setActive(bool _value);

        //////////////////////////////////////////
        inline bool getActive() const { return m_active; }


        //////////////////////////////////////////
        static inline AudioListener* GetActiveListenerRaw() { return s_activeListener; }


        //////////////////////////////////////////
        // Called once per frame by AudioListenerOnUpdate - pushes this
        // listener's Transform3D world position/orientation into the current
        // SoundSystem, if this is the active listener.
        void updateSpatial(F32 _dt);

    protected:

        //////////////////////////////////////////
        AudioListener();

        //////////////////////////////////////////
        using Component::init;

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        bool m_active = true;
        Transform3D* m_transform3D = nullptr;

        Vec3F m_lastWorldPosition = Vec3F::c_zero;
        bool m_hasLastWorldPosition = false;

        static AudioListener* s_activeListener;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAudioListener_hpp_
//////////////////////////////////////////
