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
#if (!defined(_MazeSceneManager_hpp_))
#define _MazeSceneManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/services/MazeLogStream.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneManager);
    MAZE_USING_SHARED_PTR(EcsScene);


    //////////////////////////////////////////
    // Class SceneManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SceneManager
        : public Updatable
    {
    public:

        //////////////////////////////////////////
        using ScenesList = Vector<EcsScenePtr>;

        //////////////////////////////////////////
        struct SceneData
        {
            String name;
            EcsSceneId id;
            EcsScenePtr scene;
        };

    public:

        //////////////////////////////////////////
        virtual ~SceneManager();

        //////////////////////////////////////////
        static void Initialize(
            SceneManagerPtr& _sceneManager,
            DataBlock const& _config = DataBlock::c_empty);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void destroyScene(EcsScenePtr const& _scene);

        //////////////////////////////////////////
        void destroyScene(EcsSceneId _sceneId);


        //////////////////////////////////////////
        void setMainScene(EcsScenePtr const& _value);

        //////////////////////////////////////////
        EcsScenePtr const& getMainScene() const { return m_mainScene; }


        //////////////////////////////////////////
        inline void destroyNotSystemScenes()
        {
            for (Size i = 0; i < m_scenes.size(); ++i)
            {
                if (!m_scenes[i].scene || m_scenes[i].scene->getIsSystemScene())
                    continue;

                destroyScene(m_scenes[i].scene);
            }
        }

        //////////////////////////////////////////
        template <class TScene>
        inline void destroyNotSystemScenes()
        {
            ClassUID sceneUID = ClassInfo<TScene>::UID();

            for (Size i = 0; i < m_scenes.size(); ++i)
            {
                if (!m_scenes[i].scene || m_scenes[i].scene->getIsSystemScene() || m_scenes[i].scene->getClassUID() != sceneUID)
                    continue;

                destroyScene(m_scenes[i].scene);
            }
        }

        //////////////////////////////////////////
        template <class TScene, typename ...TArgs>
        inline SharedPtr<TScene> loadScene(
            bool _additive,
            TArgs... _args)
        {
            static SharedPtr<TScene> nullPointer;

            MAZE_PROFILE_EVENT("SceneManager::loadScene");

            if (!_additive)
                destroyNotSystemScenes();

            {
                Debug::log << "Creating Scene '" << ClassInfo<TScene>::Name() << "'..." << endl;

                EcsScenePtr scene = std::static_pointer_cast<EcsScene>(TScene::Create(_args...));
                MAZE_RETURN_VALUE_IF(!scene, nullPointer);

                MAZE_DEBUG_BP_IF(scene->getClassUID() != ClassInfo<TScene>::UID());

                EcsSceneId sceneId = generateNewEcsSceneId();
                MAZE_DEBUG_ASSERT(sceneId.getIndex() < (S32)m_scenes.size() && !m_scenes[sceneId.getIndex()].scene);
                m_scenes[sceneId.getIndex()].id = sceneId;
                m_scenes[sceneId.getIndex()].scene = scene;
                m_scenes[sceneId.getIndex()].name = ClassInfo<TScene>::Name();

                scene->setId(sceneId);
                scene->setState(EcsSceneState::Created);                

                if (!m_mainScene && isGoodMainScene(scene))
                    setMainScene(scene);

                m_newScenesWereAdded = true;
                return scene->cast<TScene>();
            }
        }

        //////////////////////////////////////////
        template <class TScene>
        SharedPtr<TScene> loadScene()
        {
            return loadScene<TScene>(true);
        }

        //////////////////////////////////////////
        bool loadScene(
            EcsScenePtr const& _scene,
            bool _additive = false);


        //////////////////////////////////////////
        EcsScenePtr const& getScene(EcsSceneId _sceneId);


        //////////////////////////////////////////
        template <class TScene>
        SharedPtr<TScene> getScene()
        {
            for (Size i = 0; i < m_scenes.size(); ++i)
                if (m_scenes[i].scene && m_scenes[i].scene->getClassUID() == ClassInfo<TScene>::UID())
                    return m_scenes[i].scene->cast<TScene>();

            return nullptr;
        }

        //////////////////////////////////////////
        template <class TScene>
        SharedPtr<TScene> getSceneInheritedFrom()
        {
            for (Size i = 0; i < m_scenes.size(); ++i)
                if (m_scenes[i].scene && m_scenes[i].scene->getMetaClass()->isInheritedFrom(ClassInfo<TScene>::UID()))
                    return m_scenes[i].scene->cast<TScene>();

            return nullptr;
        }

        //////////////////////////////////////////
        template <class TScene>
        Vector<SharedPtr<TScene>> getScenesInheritedFrom()
        {
            Vector<SharedPtr<TScene>> result;
            for (Size i = 0; i < m_scenes.size(); ++i)
                if (m_scenes[i].scene && m_scenes[i].scene->getMetaClass()->isInheritedFrom(ClassInfo<TScene>::UID()))
                    result.push_back(m_scenes[i].scene->cast<TScene>());

            return result;
        }

        //////////////////////////////////////////
        EcsSceneId getEcsSceneIdByClassUID(ClassUID _classUID)
        {
            for (Size i = 0; i < m_scenes.size(); ++i)
                if (m_scenes[i].scene && m_scenes[i].scene->getClassUID() == _classUID)
                    return m_scenes[i].id;

            return EcsSceneId();
        }

        //////////////////////////////////////////
        template <class TScene>
        SharedPtr<TScene> ensureScene()
        {
            SharedPtr<TScene> scene = getScene<TScene>();
            if (scene)
                return scene;
            return loadScene<TScene>(true);
        }

        //////////////////////////////////////////
        Vector<SceneData> const& getScenes() const { return m_scenes; }
    

        //////////////////////////////////////////
        template <class TScene>
        inline void unloadScene()
        {
            SharedPtr<TScene> const& scene = getScene<TScene>();
            if (scene)
                destroyScene(scene);
        }


        //////////////////////////////////////////
        static inline SceneManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline SceneManager& GetInstance() { return *s_instance; }

    protected:

        //////////////////////////////////////////
        SceneManager();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _config);


        //////////////////////////////////////////
        bool isGoodMainScene(EcsScenePtr const& _scene);

        //////////////////////////////////////////
        EcsScenePtr const& findNewMainScene();

        //////////////////////////////////////////
        EcsSceneId generateNewEcsSceneId();

    private:
        static SceneManager* s_instance;

        Vector<SceneData> m_scenes;
        Stack<S32> m_freeSceneIndices;
        bool m_newScenesWereAdded = false;
        bool m_newScenesWereDestroyed = false;

        ScenesList m_deadScenes;

        EcsScenePtr m_mainScene;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSceneManager_hpp_
//////////////////////////////////////////
