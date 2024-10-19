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
#if (!defined(_MazeEcsScene_hpp_))
#define _MazeEcsScene_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/utils/MazeIdWithGeneration.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneManager);


    //////////////////////////////////////////
    using EcsSceneId = IdWithGeneration<S32, 8>;


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(MAZE_CORE_API, EcsSceneState,
        Created,
        Active,
        Destroy);


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EcsScene);
    MAZE_USING_SHARED_PTR(EcsWorld);
    MAZE_USING_SHARED_PTR(ComponentEcsSceneLinker);


    //////////////////////////////////////////
    // Class EcsScene
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EcsScene
        : public SharedObject<EcsScene>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(EcsScene);

        //////////////////////////////////////////
        friend SceneManager;

        //////////////////////////////////////////
        friend void Entity::setEcsScene(EcsScene*);

    protected:

        //////////////////////////////////////////
        enum Flags
        {
            IsSystemScene = MAZE_BIT(0)
        };

    public:

        //////////////////////////////////////////
        virtual ~EcsScene();
        
        //////////////////////////////////////////
        virtual HashedCString getSceneName() const { return getMetaClass()->getName(); }

        //////////////////////////////////////////
        virtual void update(F32 _dt);


        //////////////////////////////////////////
        inline EcsSceneId getId() const { return m_id; }

        //////////////////////////////////////////
        inline EcsSceneState getState() const { return m_state; }



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
        inline EcsWorld* getWorld() const { return m_world; }


        //////////////////////////////////////////
        Set<Entity*> const& getEntities() const { return m_entities; }


        //////////////////////////////////////////
        template <typename TComponent, typename ...TArgs>
        inline SharedPtr<TComponent> createAndAddEntityWithComponent(
            CString _name,
            TArgs... _args)
        {
            EntityPtr entity = createEntity(_name);
            return entity->createComponent<TComponent, TArgs...>(_args...);
        }

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



        //////////////////////////////////////////
        virtual void serializeSceneCommonInfo(DataBlock& _info) {}

        //////////////////////////////////////////
        virtual void deserializeSceneCommonInfo(DataBlock const& _info) {}

    protected:

        //////////////////////////////////////////
        EcsScene();

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

        //////////////////////////////////////////
        void notifyEcsWorldOnDestroy(EcsWorld* _world);

    private:

        //////////////////////////////////////////
        void setState(EcsSceneState _state);

        //////////////////////////////////////////
        inline void setId(EcsSceneId _id) { m_id = _id; }

        //////////////////////////////////////////
        virtual void processSceneStateChanged(EcsSceneState _state) {}

        //////////////////////////////////////////
        void processSceneWillBeDestroyed();

        //////////////////////////////////////////
        virtual void load();


        //////////////////////////////////////////
        void processEntityAdded(Entity* _entity);

        //////////////////////////////////////////
        void processEntityRemoved(Entity* _entity);


        //////////////////////////////////////////
        virtual EcsWorld* assignWorld();

    protected:
        S32 m_flags = 0;

        Set<Entity*> m_entities;

        EcsWorld* m_world = nullptr;

    private:
        EcsSceneId m_id;
        EcsSceneState m_state = EcsSceneState::None;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsScene_hpp_
//////////////////////////////////////////
