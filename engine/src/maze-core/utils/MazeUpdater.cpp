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
#include "MazeCoreHeader.hpp"
#include "maze-core/utils/MazeUpdater.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Updatable
    //
    //////////////////////////////////////////
    Updatable::Updatable()
        : m_updater(nullptr)
    {
    
    }

    //////////////////////////////////////////
    Updatable::~Updatable()
    {
        if (m_updater)
            m_updater->removeUpdatable(this);
    }
    
    
    //////////////////////////////////////////
    // Class Updater
    //
    //////////////////////////////////////////
    Updater::Updater()
        : m_updatablesDirty(false)
    {
        
    }

    //////////////////////////////////////////
    Updater::~Updater()
    {
        for (Size i = 0, in = m_updatables.size(); i < in; ++i)
        {
            if (!m_updatables[i])
                continue;

            m_updatables[i]->m_updater = nullptr;
        }
        m_updatables.clear();
    }

    //////////////////////////////////////////
    void Updater::addUpdatable(Updatable* _updatable)
    {
        MAZE_DEBUG_BP_IF(std::find(m_updatables.begin(), m_updatables.end(), _updatable) != m_updatables.end());
        MAZE_DEBUG_BP_IF(_updatable->m_updater);

        m_updatables.push_back(_updatable);
        _updatable->m_updater = this;
    }

    //////////////////////////////////////////
    void Updater::removeUpdatable(Updatable* _updatable)
    {
        Vector<Updatable*>::iterator it = std::find(m_updatables.begin(), m_updatables.end(), _updatable);
        if (it == m_updatables.end())
            return;

        (*it)->m_updater = nullptr;
        (*it) = nullptr;

        m_updatablesDirty = true;
    }

    //////////////////////////////////////////
    void Updater::ensureUpdatable(Updatable* _updatable)
    {
        Vector<Updatable*>::iterator it = std::find(m_updatables.begin(), m_updatables.end(), _updatable);
        if (it != m_updatables.end())
            return;

        m_updatables.push_back(_updatable);
        _updatable->m_updater = this;
    }

    //////////////////////////////////////////
    void Updater::processUpdate(F32 _dt)
    {
        for (Size i = 0, in = m_updatables.size(); i < in; ++i)
        {
            if (!m_updatables[i])
                continue;

            m_updatables[i]->update(_dt);
        }

        eventUpdate(_dt);

        if (m_updatablesDirty)
        {
            // Clear null
            m_updatables.erase(
                std::remove_if(
                    m_updatables.begin(),
                    m_updatables.end(),
                    [](Updatable* _updatable)
                    {
                        return _updatable == nullptr;
                    }),
                m_updatables.end());

            m_updatablesDirty = false;
        }
    }


} // namespace Maze
//////////////////////////////////////////
