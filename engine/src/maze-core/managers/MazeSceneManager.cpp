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
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class SceneManager
    //
    //////////////////////////////////////////
    SceneManager* SceneManager::s_instance = nullptr;

    //////////////////////////////////////////
    SceneManager::SceneManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    SceneManager::~SceneManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void SceneManager::Initialize(SceneManagerPtr& _eventManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneManager, _eventManager, init());
    }

    //////////////////////////////////////////
    bool SceneManager::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void SceneManager::destroyScene(ECSScenePtr const& _scene)
    {
        if (_scene->getState() == ECSSceneState::Destroy)
            return;

        _scene->setState(ECSSceneState::Destroy);        
    }

    //////////////////////////////////////////
    void SceneManager::setMainScene(ECSScenePtr const& _value)
    {
        m_mainScene = _value;
    }
    
    //////////////////////////////////////////
    void SceneManager::update(F32 _dt)
    {
        m_deadScenes.clear();


        if (!m_newScenes.empty())
        {
            for (Size i = 0, in = m_scenes.size(); i < in; ++i)
            {
                ECSScenePtr const& scene = m_scenes[i];
                if (scene->getPausedInBackground() &&
                    scene->getState() == ECSSceneState::Active)
                {
                    m_scenes[i]->setState(ECSSceneState::Paused);
                }
            }

            m_scenes.insert(
                m_scenes.end(),
                m_newScenes.begin(),
                m_newScenes.end());

            m_newScenes.clear();
        }

        for (ScenesList::iterator   it = m_scenes.begin(),
                                    end = m_scenes.end();
                                    it != end;)
        {
            ECSScenePtr const& scene = (*it);

            if (ECSSceneState::None == scene->getState())
            {
                ++it;
                continue;
            }
            else
            if (ECSSceneState::Created == scene->getState())
            {
                scene->setState(ECSSceneState::Active);
            }
            else
            if (ECSSceneState::Destroy == scene->getState())
            {
                ECSScenePtr scenePointerCopy = scene;

                scene->processSceneWillBeDestroyed();

                m_deadScenes.push_back(scene);

                it = m_scenes.erase(it);
                end = m_scenes.end();

                if (scene == m_mainScene)
                {
                    if (!m_scenes.empty())
                        setMainScene(m_scenes.back());
                    else
                        setMainScene(nullptr);
                }

                if (!m_scenes.empty())
                {
                    if (m_scenes.back()->getState() == ECSSceneState::Paused)
                        m_scenes.back()->setState(ECSSceneState::Active);
                }

                continue;
            }

            (*it)->update(_dt);
            ++it;
        }
    }

} // namespace Maze
//////////////////////////////////////////
