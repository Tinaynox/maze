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
#if (!defined(_MazeEntityManager_hpp_))
#define _MazeEntityManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/data/MazeDataBlock.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntityManager);
    MAZE_USING_SHARED_PTR(EcsWorld);
    MAZE_USING_SHARED_PTR(ComponentFactory);
    MAZE_USING_SHARED_PTR(EntitySerializationManager);


    //////////////////////////////////////////
    // Class EntityManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntityManager
        : public Updatable
    {
    public:

        //////////////////////////////////////////
        virtual ~EntityManager();

        //////////////////////////////////////////
        static void Initialize(
            EntityManagerPtr& _entityManager,
            DataBlock const& _config = DataBlock::c_empty);

    
        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        static inline EntityManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EntityManager& GetInstance() { return *s_instance; }

        //////////////////////////////////////////
        S32 getComponentPriority(ClassUID _componentUID);

        //////////////////////////////////////////
        template <typename TComponent>
        inline S32 getComponentPriority()
        {
            return this->getComponentPriority(ClassInfo<TComponent>::UID());
        }


        //////////////////////////////////////////
        inline EcsWorldPtr const& getDefaultWorld() const { return m_defaultWorld; }

        //////////////////////////////////////////
        inline EcsWorld* getDefaultWorldRaw() const { return m_defaultWorld.get(); }


        //////////////////////////////////////////
        inline EcsWorldPtr const& getLibraryWorld() const { return m_libraryWorld; }

        //////////////////////////////////////////
        inline EcsWorld* getLibraryWorldRaw() const { return m_libraryWorld.get(); }


        //////////////////////////////////////////
        ComponentFactoryPtr const& getComponentFactory() const { return m_componentFactory; }


        //////////////////////////////////////////
        virtual S32 getUpdatableOrder() const { return 20000; }

    protected:

        //////////////////////////////////////////
        EntityManager();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _config);


    private:
        static EntityManager* s_instance;

        EcsWorldPtr m_defaultWorld;
        EcsWorldPtr m_libraryWorld;

        ComponentFactoryPtr m_componentFactory;

        EntitySerializationManagerPtr m_entitySerializationManager;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEntityManager_hpp_
//////////////////////////////////////////
