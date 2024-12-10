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
#if (!defined(_MazeEnginePlayer_hpp_))
#define _MazeEnginePlayer_hpp_


//////////////////////////////////////////
#include "maze-engine/MazeEngineHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EnginePlayer);
    MAZE_USING_SHARED_PTR(Camera3D);


    //////////////////////////////////////////
    // Class EnginePlayerCurrentCameraChangedEvent
    //
    //////////////////////////////////////////
    class MAZE_ENGINE_API EnginePlayerCurrentCameraChangedEvent
        : public GenericEvent<EnginePlayerCurrentCameraChangedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EnginePlayerCurrentCameraChangedEvent, Event);

        //////////////////////////////////////////
        EnginePlayerCurrentCameraChangedEvent(EntityId _cameraEid = c_invalidEntityId)
            : cameraEid(_cameraEid)
        {}

        EntityId cameraEid;
    };


    //////////////////////////////////////////
    // Class EnginePlayer
    //
    //////////////////////////////////////////
    class MAZE_ENGINE_API EnginePlayer
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EnginePlayer, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(EnginePlayer);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~EnginePlayer();

        //////////////////////////////////////////
        static EnginePlayerPtr Create();


        //////////////////////////////////////////
        void setCurrentCamera3D(Camera3DPtr const& _camera);

        //////////////////////////////////////////
        inline Camera3DPtr const& getCurrentCamera3D() const { return m_currentCamera3D; }



        //////////////////////////////////////////
        void processOnCreate();

        //////////////////////////////////////////
        void processUpdate(F32 _dt);

    protected:

        //////////////////////////////////////////
        EnginePlayer();

        //////////////////////////////////////////
        using Component::init;

        //////////////////////////////////////////
        bool init();


    protected:
        Camera3DPtr m_currentCamera3D;
        SharedPtr<GenericInclusiveEntitiesSample<Camera3D>> m_cameras3DSample;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEnginePlayer_hpp_
//////////////////////////////////////////
