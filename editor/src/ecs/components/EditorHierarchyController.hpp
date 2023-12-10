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
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "editor/EditorSceneMode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(EditorHierarchyController);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(HierarchyLinePool);
    MAZE_USING_SHARED_PTR(HierarchyLine);
    MAZE_USING_SHARED_PTR(ECSScene);


    //////////////////////////////////////////
    struct EditorHierarchyLineEntityData
    {
        //////////////////////////////////////////
        EditorHierarchyLineEntityData()
            : expanded(false)
        {
        }

        HierarchyLinePtr line;
        bool expanded;
    };


    //////////////////////////////////////////
    struct EditorHierarchyLineSceneData
    {
        //////////////////////////////////////////
        EditorHierarchyLineSceneData()
            : expanded(false)
        {
        }

        HierarchyLinePtr line;
        bool expanded;
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
        inline void addIgnoreScene(ClassUID _classUID) { m_ignoreScenes.insert(_classUID); }


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
        void updateHierarchy();

        //////////////////////////////////////////
        void updateHierarchyScenes();

        //////////////////////////////////////////
        void updateHierarchyPrefab();

        //////////////////////////////////////////
        void updateHierarchyElement(Transform2D* _transform2D, F32 _x, F32& _y);

        //////////////////////////////////////////
        void updateHierarchyElement(Transform3D* _transform3D, F32 _x, F32& _y);

        //////////////////////////////////////////
        HierarchyLinePtr createHierarchyElement(
            Transform2D* _transform,
            CString _name,
            Vec2F32 const& _position);

        //////////////////////////////////////////
        HierarchyLinePtr createHierarchyElement(
            Transform3D* _transform,
            CString _name,
            Vec2F32 const& _position);

        //////////////////////////////////////////
        HierarchyLinePtr createHierarchyElement(
            Entity* _entity,
            CString _name,
            Vec2F32 const& _position);

        //////////////////////////////////////////
        HierarchyLinePtr createHierarchyLine(EntityId _entityId);

        //////////////////////////////////////////
        HierarchyLinePtr createHierarchyLine(ECSScenePtr const& _scene);

        //////////////////////////////////////////
        void notifyHierarchyLineDropDownClick(HierarchyLine* _hierarchyLine);

        //////////////////////////////////////////
        void notifyHierarchyLineClick(HierarchyLine* _hierarchyLine);

        //////////////////////////////////////////
        void notifyHierarchyLineDoubleClick(HierarchyLine* _hierarchyLine);

        //////////////////////////////////////////
        void notifyEntityRemoved(EntityPtr const& _entity);

        //////////////////////////////////////////
        void notifyEntityChanged(Entity* _entity);


        //////////////////////////////////////////
        void removeHierarchyLine(EntityPtr const& _entity);


        //////////////////////////////////////////
        void notifyEditorSceneModeChanged(EditorSceneMode _mode);

        //////////////////////////////////////////
        void notifyPlaytestModeEnabled(bool _value);

        //////////////////////////////////////////
        void notifyPrefabEntityChanged(EntityPtr const& _entity);

        //////////////////////////////////////////
        void notifySelectionChanged();

        //////////////////////////////////////////
        void setECSWorld(ECSWorld* _world);

        //////////////////////////////////////////
        void expandEntity(EntityPtr const& _entity);

    protected:
        Canvas* m_canvas;

        Transform2DPtr m_transform;

        SpriteRenderer2DPtr m_bodyBackground;
        Transform2DPtr m_layoutTransform;

        HierarchyLinePoolPtr m_hierarchyLinePool;

        Transform2DPtr m_titleTransform;
        SpriteRenderer2DPtr m_titleBackground;

        ECSWorld* m_world = nullptr;

        Map<EntityId, EditorHierarchyLineEntityData> m_hierarchyLinesPerEntity;
        UnorderedMap<String, EditorHierarchyLineSceneData> m_hierarchyLinesPerScene;

        Set<ClassUID> m_ignoreScenes;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorHierarchyController_hpp_
//////////////////////////////////////////
