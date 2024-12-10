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
#include "MazeEngineHeader.hpp"
#include "maze-engine/ecs/components/MazeEnginePlayer.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EnginePlayerCurrentCameraChangedEvent, Event);


    //////////////////////////////////////////
    // Class EnginePlayer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EnginePlayer, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(EnginePlayer);

    //////////////////////////////////////////
    EnginePlayer::EnginePlayer()
    {
    }

    //////////////////////////////////////////
    EnginePlayer::~EnginePlayer()
    {

    }

    //////////////////////////////////////////
    EnginePlayerPtr EnginePlayer::Create()
    {
        EnginePlayerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EnginePlayer, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool EnginePlayer::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void EnginePlayer::processOnCreate()
    {
        m_cameras3DSample = getEntityRaw()->getEcsWorld()->requestInclusiveSample<Camera3D>();
    }

    //////////////////////////////////////////
    void EnginePlayer::processUpdate(F32 _dt)
    {
        if (!m_currentCamera3D)
        {
            m_cameras3DSample->findQuery(
                [&](Entity* _entity, Camera3D* _camera)
                {
                    if (_camera->getRenderMask() & (S32)DefaultRenderMask::Gizmos)
                        return false;

                    setCurrentCamera3D(_camera->cast<Camera3D>());
                    return true;
                });
        }
    }

    //////////////////////////////////////////
    void EnginePlayer::setCurrentCamera3D(Camera3DPtr const& _camera)
    {
        if (m_currentCamera3D == _camera)
            return;

        m_currentCamera3D = _camera;

        m_entityRaw->getEcsWorld()->broadcastEvent<EnginePlayerCurrentCameraChangedEvent>(
            m_currentCamera3D ? m_currentCamera3D->getEntityId() : c_invalidEntityId);
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(EnginePlayerOnCreate,
        {},
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        EnginePlayer* _enginePlayer)
    {
        _enginePlayer->processOnCreate();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(EnginePlayerUpdate,
        {},
        {},
        UpdateEvent const& _event,
        Entity* _entity,
        EnginePlayer* _enginePlayer)
    {
        _enginePlayer->processUpdate(_event.getDt());
    }


} // namespace Maze
//////////////////////////////////////////
