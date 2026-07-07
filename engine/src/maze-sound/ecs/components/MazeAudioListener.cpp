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
#include "MazeSoundHeader.hpp"
#include "maze-sound/ecs/components/MazeAudioListener.hpp"
#include "maze-sound/MazeSoundSystem.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class AudioListener
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AudioListener, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, active, true, getActive, setActive));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(AudioListener);

    //////////////////////////////////////////
    AudioListener* AudioListener::s_activeListener = nullptr;

    //////////////////////////////////////////
    AudioListener::AudioListener()
    {
    }

    //////////////////////////////////////////
    AudioListener::~AudioListener()
    {
        if (s_activeListener == this)
            s_activeListener = nullptr;
    }

    //////////////////////////////////////////
    AudioListenerPtr AudioListener::Create()
    {
        AudioListenerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AudioListener, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool AudioListener::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void AudioListener::processEntityAwakened()
    {
        m_transform3D = getEntityRaw()->getComponentRaw<Transform3D>();

        if (m_active)
        {
            if (s_activeListener && s_activeListener != this)
                s_activeListener->m_active = false;

            s_activeListener = this;
        }
    }

    //////////////////////////////////////////
    void AudioListener::setActive(bool _value)
    {
        if (m_active == _value)
            return;

        m_active = _value;

        if (m_active)
        {
            if (s_activeListener && s_activeListener != this)
                s_activeListener->m_active = false;

            s_activeListener = this;
        }
        else
        if (s_activeListener == this)
        {
            s_activeListener = nullptr;
        }
    }

    //////////////////////////////////////////
    void AudioListener::updateSpatial(F32 _dt)
    {
        if (s_activeListener != this)
            return;

        SoundSystem* soundSystem = SoundManager::GetInstancePtr()
            ? SoundManager::GetInstancePtr()->getDefaultSoundSystemRaw()
            : nullptr;
        if (!soundSystem)
            return;

        if (!m_transform3D)
        {
            if (!m_hasLastWorldPosition)
            {
                soundSystem->setListenerPosition(Vec3F::c_zero);
                soundSystem->setListenerVelocity(Vec3F::c_zero);
                soundSystem->setListenerOrientation(Vec3F(0.0f, 0.0f, 1.0f), Vec3F(0.0f, 1.0f, 0.0f));
                m_hasLastWorldPosition = true;
            }
            return;
        }

        if (!m_transform3D->isWorldTransformChanged() && m_hasLastWorldPosition)
            return;

        Vec3F worldPosition = m_transform3D->getWorldPosition();
        Vec3F velocity = (m_hasLastWorldPosition && _dt > 1e-5f)
            ? (worldPosition - m_lastWorldPosition) / _dt
            : Vec3F::c_zero;

        soundSystem->setListenerPosition(worldPosition);
        soundSystem->setListenerVelocity(velocity);
        soundSystem->setListenerOrientation(m_transform3D->getWorldForwardDirection(), m_transform3D->getWorldUpDirection());

        m_lastWorldPosition = worldPosition;
        m_hasLastWorldPosition = true;
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(AudioListenerOnUpdate,
        MAZE_ECS_TAGS(MAZE_HS("default")),
        {},
        UpdateEvent const& _event,
        Entity* _entity,
        AudioListener* _audioListener)
    {
        _audioListener->updateSpatial(_event.getDt());
    }

} // namespace Maze
//////////////////////////////////////////
