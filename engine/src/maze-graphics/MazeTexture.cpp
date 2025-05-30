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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeTexture.hpp"
#include "maze-graphics/events/MazeGraphicsEvents.hpp"
#include "maze-core/managers/MazeEventManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Texture
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(TextureFilter);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(TextureWrap);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(TextureType);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Texture, Object);

    //////////////////////////////////////////
    Texture* Texture::s_instancesList = nullptr;

    //////////////////////////////////////////
    Texture::Texture()
        : m_renderSystem(nullptr)
    {
        if (s_instancesList)
            s_instancesList->m_instancesListNext = this;
        m_instancesListPrev = s_instancesList;
        s_instancesList = this;
    }

    //////////////////////////////////////////
    Texture::~Texture()
    {
        if (m_instancesListPrev)
            m_instancesListPrev->m_instancesListNext = m_instancesListNext;
        if (m_instancesListNext)
            m_instancesListNext->m_instancesListPrev = m_instancesListPrev;
        else
        if (s_instancesList == this)
            s_instancesList = m_instancesListPrev;

        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->broadcastEventImmediate<TextureDestroyedEvent>(this);
    }

    //////////////////////////////////////////
    bool Texture::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    void Texture::IterateTextures(std::function<bool(Texture*)> _cb)
    {
        Texture* instance = s_instancesList;
        while (instance)
        {
            if (!_cb(instance))
                break;

            instance = instance->m_instancesListPrev;
        }
    }


} // namespace Maze
//////////////////////////////////////////
