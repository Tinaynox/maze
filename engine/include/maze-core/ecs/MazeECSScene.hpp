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
#if (!defined(_MazeECSScene_hpp_))
#define _MazeECSScene_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/ecs/MazeECSTypes.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneManager);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_4_API(MAZE_CORE_API, ECSSceneState,
        Created,
        Active,
        Paused,
        Destroy);


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ECSScene);
    MAZE_USING_SHARED_PTR(ECSWorld);
    MAZE_USING_SHARED_PTR(ComponentECSSceneLinker);
    MAZE_USING_SHARED_PTR(ComponentSystem);


    //////////////////////////////////////////
    // Class ECSScene
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ECSScene
        : public SharedObject<ECSScene>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ECSScene);

        //////////////////////////////////////////
        friend SceneManager;

        //////////////////////////////////////////
        friend void Entity::setECSScene(ECSScene*);

    protected:

        //////////////////////////////////////////
        enum Flags
        {
            PausedInBackground = MAZE_BIT(0),
            IsSystemScene = MAZE_BIT(1)
        };

    public:

        //////////////////////////////////////////
        virtual ~ECSScene();
        
        //////////////////////////////////////////
        virtual void update(F32 _dt);

        //////////////////////////////////////////
        ECSSceneState getState() const { return m_state; }


        //////////////////////////////////////////
        inline bool getPausedInBackground() const { return m_flags & Flags::PausedInBackground; }

        //////////////////////////////////////////
        inline void setPausedInBackground() { setFlag(Flags::PausedInBackground, true); }


        //////////////////////////////////////////
        inline bool getIsSystemScene() const { return m_flags & Flags::IsSystemScene; }

        //////////////////////////////////////////
        inline void setIsSystemScene() { setFlag(Flags::IsSystemScene, true); }


        //////////////////////////////////////////
        EntityPtr createEntity();

        //////////////////////////////////////////
        EntityPtr createEntity(String const& _name);

        //////////////////////////////////////////
        EntityPtr createEntity(CString _name);

        //////////////////////////////////////////
        EntityPtr ensureEntity(String const& _name);

        //////////////////////////////////////////
        EntityPtr ensureEntity(CString _name);

        //////////////////////////////////////////
        EntityPtr findEntity(CString _name);

        //////////////////////////////////////////
        void destroyAllEntities();

        //////////////////////////////////////////
        void destroyAllEntitiesExcept(EntityPtr const& _value);


        //////////////////////////////////////////
        void addSystem(ComponentSystemPtr const& _system);

        //////////////////////////////////////////
        void removeSystem(ComponentSystemPtr const& _system);

        //////////////////////////////////////////
        void removeAllSystems();

        //////////////////////////////////////////
        inline ECSWorld* getWorld() const { return m_world; }


        //////////////////////////////////////////
        Set<Entity*> const getEntities() const { return m_entities; }

        //////////////////////////////////////////
        template <typename TComponent>
        TComponent* findEntityByComponentRaw()
        {
            for (auto it = m_entities.begin(), end = m_entities.end(); it != end; ++it)
            {
                TComponent* component = (*it)->getComponentRaw<TComponent>();
                if (component)
                    return component;
            }

            return nullptr;
        }

        //////////////////////////////////////////
        template <typename TComponent>
        SharedPtr<TComponent> findEntityByComponent()
        {
            for (auto it = m_entities.begin(), end = m_entities.end(); it != end; ++it)
            {
                SharedPtr<TComponent> component = (*it)->getComponent<TComponent>();
                if (component)
                    return component;
            }

            return nullptr;
        }

        //////////////////////////////////////////
        void makeMainScene();

    protected:

        //////////////////////////////////////////
        ECSScene();

        //////////////////////////////////////////
        virtual bool init();


        //////////////////////////////////////////
        inline void setFlag(Flags _flag, bool _value)
        {
            if (_value)
                m_flags |= _flag;
            else
                m_flags &= ~_flag;
        }

    private:

        //////////////////////////////////////////
        void setState(ECSSceneState _state);

        //////////////////////////////////////////
        virtual void processSceneStateChanged(ECSSceneState _state) {}

        //////////////////////////////////////////
        void processSceneWillBeDestroyed();

        //////////////////////////////////////////
        virtual void load();


        //////////////////////////////////////////
        void processEntityAdded(Entity* _entity);

        //////////////////////////////////////////
        void processEntityRemoved(Entity* _entity);


        //////////////////////////////////////////
        virtual ECSWorld* assignWorld();

    protected:
        ECSSceneState m_state;

        S32 m_flags;

        Set<Entity*> m_entities;
        Vector<ComponentSystemPtr> m_systems;

        ECSWorld* m_world = nullptr;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeECSScene_hpp_
//////////////////////////////////////////
