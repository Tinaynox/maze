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
#if (!defined(_EntitySerializationManager_hpp_))
#define _EntitySerializationManager_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include <tinyxml2/tinyxml2.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntitySerializationManager);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(PrefabInstance);
    

    //////////////////////////////////////////
    // Class EntitySerializationManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntitySerializationManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        ~EntitySerializationManager();

        //////////////////////////////////////////
        static void Initialize(EntitySerializationManagerPtr& _manager);
        

        //////////////////////////////////////////
        static inline EntitySerializationManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EntitySerializationManager& GetInstance() { return *s_instance; }



        //////////////////////////////////////////
        bool savePrefabToDataBlockFile(EntityPtr const& _entity, Path const& _fileFullPath) const;

        //////////////////////////////////////////
        bool saveSceneToDataBlockFile(EcsScenePtr const& _entity, Path const& _fileFullPath) const;


        //////////////////////////////////////////
        EntityPtr loadPrefab(
            AssetFilePtr const& _assetFile,
            EcsWorld* _world = nullptr,
            EcsScene* _scene = nullptr) const;

        //////////////////////////////////////////
        EntityPtr loadPrefab(
            Path const& _assetFileName,
            EcsWorld* _world = nullptr,
            EcsScene* _scene = nullptr) const;

        //////////////////////////////////////////
        EntityPtr loadPrefabOBSOLETE(
            DataBlock const& _dataBlock,
            EcsWorld* _world = nullptr,
            EcsScene* _scene = nullptr) const;


        //////////////////////////////////////////
        EntityPtr loadPrefab(
            DataBlock const& _dataBlock,
            EcsWorld* _world = nullptr,
            EcsScene* _scene = nullptr) const;


        //////////////////////////////////////////
        void collectAllChildrenEntity(
            EntityPtr const& _entity,
            Vector<EntityPtr>& _entities,
            Vector<PrefabInstance*>& _prefabInstances) const;

        //////////////////////////////////////////
        void collectEntityComponentsMap(
            EntityPtr const& _entity,
            Map<EntityPtr, Vector<ComponentPtr>>& _entities,
            Vector<PrefabInstance*>& _prefabs) const;

    protected:

        //////////////////////////////////////////
        EntitySerializationManager();

        //////////////////////////////////////////
        bool init();


    protected:
        static EntitySerializationManager* s_instance;


    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EntitySerializationManager_hpp_
//////////////////////////////////////////
