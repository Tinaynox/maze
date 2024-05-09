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
#include "maze-core/services/MazeLogStream.hpp"


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
        if (!_scene || _scene->getState() == ECSSceneState::Destroy)
            return;

        Debug::log << "Destroying Scene '" << static_cast<CString>(_scene->getMetaClass()->getName()) << "'..." << endl;
        _scene->setState(ECSSceneState::Destroy);        
    }

    //////////////////////////////////////////
    void SceneManager::destroyScene(EcsSceneId _sceneId)
    {
        if (_sceneId.getIndex() < m_scenes.size())
            destroyScene(m_scenes[_sceneId.getIndex()].scene);
    }

    //////////////////////////////////////////
    void SceneManager::setMainScene(ECSScenePtr const& _value)
    {
        m_mainScene = _value;
    }
    
    //////////////////////////////////////////
    void SceneManager::update(F32 _dt)
    {
        MAZE_PROFILE_EVENT("SceneManager::update");

        m_deadScenes.clear();

        if (m_newScenesWereAdded)
        {
            for (Size i = 0, in = m_scenes.size(); i < in; ++i)
            {
                auto& sceneData = m_scenes[i];
                if (sceneData.scene && sceneData.scene->getState() == ECSSceneState::Created)
                    m_scenes[i].scene->setState(ECSSceneState::Active);
            }
        }

        Size scenesCount = m_scenes.size();
        for (Size i = 0; i < scenesCount; ++i)
        {
            SceneData& sceneData = m_scenes[i];
            if (!sceneData.scene)
                continue;

            if (ECSSceneState::None == sceneData.scene->getState())
            {
                continue;
            }
            else
            if (ECSSceneState::Active == sceneData.scene->getState())
            {
                sceneData.scene->update(_dt);
            }
            else
            if (ECSSceneState::Destroy == sceneData.scene->getState())
            {
                ECSScenePtr scenePointerCopy = sceneData.scene;

                scenePointerCopy->processSceneWillBeDestroyed();

                m_deadScenes.push_back(scenePointerCopy);
                m_freeSceneIndices.push(sceneData.id.getIndex());
                sceneData.id.incrementGeneration();
                sceneData.scene.reset();

                if (scenePointerCopy == m_mainScene)
                    setMainScene(findNewMainScene());

                continue;
            }
        }
    }

    //////////////////////////////////////////
    bool SceneManager::isGoodMainScene(ECSScenePtr const& _scene)
    {
        if (!_scene)
            return false;

        if (_scene->getState() == ECSSceneState::Destroy)
            return false;

        if (_scene->getIsSystemScene())
            return false;

        return true;
    }

    //////////////////////////////////////////
    ECSScenePtr const& SceneManager::findNewMainScene()
    {
        static ECSScenePtr const nullPointer;

        for (Vector<SceneData>::reverse_iterator it = m_scenes.rbegin(), end = m_scenes.rend(); it != end; ++it)
        {
            ECSScenePtr const& scene = it->scene;
            if (!isGoodMainScene(scene))
                continue;

            return scene;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    EcsSceneId SceneManager::generateNewEcsSceneId()
    {
        if (!m_freeSceneIndices.empty())
        {
            S32 index = m_freeSceneIndices.top();
            m_freeSceneIndices.pop();
            MAZE_ASSERT(index < m_scenes.size() && !m_scenes[index].scene);
            return m_scenes[index].id;
        }

        SceneData sceneData;
        sceneData.id = EcsSceneId((S32)m_scenes.size(), 0);
        m_scenes.push_back(sceneData);
        return sceneData.id;
    }

} // namespace Maze
//////////////////////////////////////////
