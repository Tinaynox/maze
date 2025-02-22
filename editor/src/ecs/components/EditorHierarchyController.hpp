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
#if (!defined(_EditorHierarchyController_hpp_))
#define _EditorHierarchyController_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLine.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "editor/EditorSceneMode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(EditorHierarchyController);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(HierarchyLinePool);
    MAZE_USING_SHARED_PTR(HierarchyLine);
    MAZE_USING_SHARED_PTR(EcsAssetScene);
    MAZE_USING_SHARED_PTR(EditorHierarchyControllerAgent);


    //////////////////////////////////////////
    enum class EditorHierarchyMode
    {
        None = 0,
        Scene,
        Prefab
    };


    //////////////////////////////////////////
    // Class EditorHierarchyController
    //
    //////////////////////////////////////////
    class EditorHierarchyController
        : public Component
        , public Updatable
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorHierarchyController, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(EditorHierarchyController);

        //////////////////////////////////////////
        friend class Entity;


    public:

        //////////////////////////////////////////
        virtual ~EditorHierarchyController();

        //////////////////////////////////////////
        static EditorHierarchyControllerPtr Create(Canvas* _canvas);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }


        //////////////////////////////////////////
        void addIgnoreScene(ClassUID _classUID);


        //////////////////////////////////////////
        void setEcsWorld(EcsWorld* _world);


        //////////////////////////////////////////
        void processEcsSceneStateChanged(
            EcsSceneId _sceneId,
            EcsSceneState _state);


        //////////////////////////////////////////
        void processEntityParentChanged(
            EntityId _entityId,
            EntityId _prevParentEntityId,
            EntityId _newParentEntityId);

    protected:

        //////////////////////////////////////////
        EditorHierarchyController();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(Canvas* _canvas);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        

        //////////////////////////////////////////
        HierarchyLinePtr addEcsScene(EcsScenePtr const& _scene);

        //////////////////////////////////////////
        void removeEcsScene(EcsSceneId _sceneId);



        //////////////////////////////////////////
        HierarchyLinePtr addEntity(EntityPtr const& _entity);

        //////////////////////////////////////////
        void removeEntity(EntityId _entityId);

        //////////////////////////////////////////
        void updateEntity(EntityPtr const& _entity);

        //////////////////////////////////////////
        void updateEntity(HierarchyLinePtr const& hierarchyLine, EntityPtr const& _entity);

        //////////////////////////////////////////
        void updateEntityName(EntityPtr const& _entity);


        //////////////////////////////////////////
        HierarchyLinePtr createHierarchyLine(HierarchyLineType _type);

        //////////////////////////////////////////
        void subscribeHierarchyLine(HierarchyLine* _line);

        //////////////////////////////////////////
        void unsubscribeHierarchyLine(HierarchyLine* _line);

        //////////////////////////////////////////
        void removeHierarchyLine(HierarchyLine* _line);


        //////////////////////////////////////////
        void notifyHierarchyLineDoubleClick(HierarchyLine* _hierarchyLine);

        //////////////////////////////////////////
        void notifyHierarchyLineClick(HierarchyLine* _hierarchyLine);

        //////////////////////////////////////////
        void notifyHierarchyLineCursorPressIn(HierarchyLine* _hierarchyLine);

        //////////////////////////////////////////
        void notifyEventManagerEvent(
            ClassUID _classUID,
            Event* _event);

        //////////////////////////////////////////
        void notifySelectionChanged();


        //////////////////////////////////////////
        void notifyEntityAdded(EntityPtr const& _entity);

        //////////////////////////////////////////
        void notifyEntityRemoved(EntityPtr const& _entity);

        //////////////////////////////////////////
        void notifyEntityChanged(EntityPtr const& _entity);

        //////////////////////////////////////////
        void notifyWorldDestroyed(EcsWorld* _world);


        //////////////////////////////////////////
        void notifyHierarchyLineRelease(HierarchyLine* _hierarchyLine);


        //////////////////////////////////////////
        void notifyEditorSceneModeChanged(EditorSceneMode _mode);

        //////////////////////////////////////////
        void notifyPlaytestModeEnabled(bool _value);


        //////////////////////////////////////////
        void notifyPrefabEntityChanged(EntityPtr const& _entity);

        //////////////////////////////////////////
        void notifySceneChanged(EcsAssetScenePtr const& _scene);


        //////////////////////////////////////////
        void updateMode();

        //////////////////////////////////////////
        void setHierarchyMode(EditorHierarchyMode _mode);

    protected:
        EditorHierarchyMode m_hierarchyMode = EditorHierarchyMode::None;

        Canvas* m_canvas = nullptr;

        Transform2DPtr m_transform;

        SpriteRenderer2DPtr m_bodyBackground;

        HierarchyLinePoolPtr m_hierarchyLinePool;

        Transform2DPtr m_titleTransform;
        SpriteRenderer2DPtr m_titleBackground;

        Transform2DPtr m_hierarchyMainLayoutEntity;

        EcsWorld* m_world = nullptr;

        Set<ClassUID> m_ignoreScenes;

        EditorHierarchyControllerAgentPtr m_agent;


        UnorderedMap<EcsSceneId, HierarchyLinePtr> m_sceneLines;
        UnorderedMap<EntityId, HierarchyLinePtr> m_entityLines;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorHierarchyController_hpp_
//////////////////////////////////////////
