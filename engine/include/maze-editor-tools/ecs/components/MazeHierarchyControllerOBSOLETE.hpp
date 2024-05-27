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
#if (!defined(_MazeHierarchyControllerOBSOLETE_hpp_))
#define _MazeHierarchyControllerOBSOLETE_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(HierarchyControllerOBSOLETE);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(HierarchyLinePoolOBSOLETE);
    MAZE_USING_SHARED_PTR(HierarchyLineOBSOLETE);
    MAZE_USING_SHARED_PTR(EcsScene);


    //////////////////////////////////////////
    struct HierarchyLineOBSOLETEEntityDataOBSOLETE
    {
        //////////////////////////////////////////
        HierarchyLineOBSOLETEEntityDataOBSOLETE()
            : expanded(false)
        {
        }

        HierarchyLineOBSOLETEPtr line;
        bool expanded;
    };


    //////////////////////////////////////////
    struct HierarchyLineSceneDataOBSOLETE
    {
        //////////////////////////////////////////
        HierarchyLineSceneDataOBSOLETE()
            : expanded(false)
        {
        }

        HierarchyLineOBSOLETEPtr line;
        bool expanded;
    };


    //////////////////////////////////////////
    // Class HierarchyControllerOBSOLETE
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API HierarchyControllerOBSOLETE
        : public Component
        , public Updatable
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(HierarchyControllerOBSOLETE, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(HierarchyControllerOBSOLETE);

        //////////////////////////////////////////
        friend class Entity;


    public:

        //////////////////////////////////////////
        virtual ~HierarchyControllerOBSOLETE();

        //////////////////////////////////////////
        static HierarchyControllerOBSOLETEPtr Create(Canvas* _canvas);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }


        //////////////////////////////////////////
        inline void addIgnoreScene(ClassUID _classUID) { m_ignoreScenes.insert(_classUID); }


        //////////////////////////////////////////
        void setEcsWorld(EcsWorld* _world);

    protected:

        //////////////////////////////////////////
        HierarchyControllerOBSOLETE();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(Canvas* _canvas);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        void updateHierarchy();

        //////////////////////////////////////////
        void updateHierarchyElement(Transform2D* _transform2D, F32 _x, F32& _y);

        //////////////////////////////////////////
        void updateHierarchyElement(Transform3D* _transform3D, F32 _x, F32& _y);

        //////////////////////////////////////////
        HierarchyLineOBSOLETEPtr createHierarchyElement(
            Transform2D* _transform,
            CString _name,
            Vec2F const& _position);

        //////////////////////////////////////////
        HierarchyLineOBSOLETEPtr createHierarchyElement(
            Transform3D* _transform,
            CString _name,
            Vec2F const& _position);

        //////////////////////////////////////////
        HierarchyLineOBSOLETEPtr createHierarchyElement(
            Entity* _entity,
            CString _name,
            Vec2F const& _position);

        //////////////////////////////////////////
        HierarchyLineOBSOLETEPtr createHierarchyLineOBSOLETE(EntityId _entityId);

        //////////////////////////////////////////
        HierarchyLineOBSOLETEPtr createHierarchyLineOBSOLETE(EcsScenePtr const& _scene);

        //////////////////////////////////////////
        void notifyHierarchyLineOBSOLETEDropDownClick(HierarchyLineOBSOLETE* _hierarchyLine);

        //////////////////////////////////////////
        void notifyHierarchyLineOBSOLETEClick(HierarchyLineOBSOLETE* _hierarchyLine);

        //////////////////////////////////////////
        void notifyEntityRemoved(EntityPtr const& _entity);

        //////////////////////////////////////////
        void notifyEntityChanged(EntityPtr const& _entity);


        //////////////////////////////////////////
        void removeHierarchyLineOBSOLETE(EntityPtr const& _entity);

    protected:
        Canvas* m_canvas = nullptr;

        Transform2DPtr m_transform;

        SpriteRenderer2DPtr m_bodyBackground;
        Transform2DPtr m_layoutTransform;

        HierarchyLinePoolOBSOLETEPtr m_hierarchyLinePool;

        Transform2DPtr m_titleTransform;
        SpriteRenderer2DPtr m_titleBackground;

        EcsWorld* m_world = nullptr;

        Map<EntityId, HierarchyLineOBSOLETEEntityDataOBSOLETE> m_hierarchyLinesPerEntity;
        UnorderedMap<EcsScene*, HierarchyLineSceneDataOBSOLETE> m_hierarchyLinesPerScene;

        Set<ClassUID> m_ignoreScenes;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeHierarchyControllerOBSOLETE_hpp_
//////////////////////////////////////////
