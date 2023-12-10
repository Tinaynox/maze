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
#if (!defined(_MazeContextMenuCanvas2D_hpp_))
#define _MazeContextMenuCanvas2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-ui/ecs/components/MazeMenuListTree2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(ContextMenuCanvas2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(ECSScene);


    //////////////////////////////////////////
    // Class ContextMenuCanvas2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API ContextMenuCanvas2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ContextMenuCanvas2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ContextMenuCanvas2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        static ContextMenuCanvas2DPtr EnsureContextMenuCanvas(ECSScene* _scene);

    public:

        //////////////////////////////////////////
        virtual ~ContextMenuCanvas2D();

        //////////////////////////////////////////
        static ContextMenuCanvas2DPtr Create();


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }



        //////////////////////////////////////////
        inline void setMenuListPrefab(MenuListTree2DPtr const& _value)
        {
            if (m_menuListTreePrefab == _value)
                return;

            m_menuListTreePrefab = _value;
        }

        //////////////////////////////////////////
        inline MenuListTree2DPtr const& getMenuListTreePrefab() const { return m_menuListTreePrefab; }

        //////////////////////////////////////////
        inline void setMenuListTreePrefab(ComponentPtr _value) { setMenuListPrefab(_value ? _value->safeCast<MenuListTree2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getMenuListTreePrefabComponent() const { return m_menuListTreePrefab; }


        //////////////////////////////////////////
        void closeContextMenu();

        //////////////////////////////////////////
        MenuListTree2DPtr const& openContextMenu(
            void* _contextMenuHolder,
            Vec2F const& _positionWS,
            std::function<void(MenuListTree2DPtr const&)> _initMenuFunc);

        //////////////////////////////////////////
        bool isContextMenuOpened() const;

        //////////////////////////////////////////
        inline void* getContextMenuHolder() const { return m_contextMenuHolder; }

    public:

        //////////////////////////////////////////
        MultiDelegate<ContextMenuCanvas2D*, MenuListTree2DPtr const&> eventContextMenuOpened;
        MultiDelegate<ContextMenuCanvas2D*, MenuListTree2DPtr const&> eventContextMenuClosed;

    protected:

        //////////////////////////////////////////
        ContextMenuCanvas2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyEnabledInHierarchyChanged(MenuListTree2D* _tree, bool _enabled);

    
    protected:
        Transform2DPtr m_transform;

        MenuListTree2DPtr m_menuListTreePrefab;
        CanvasPtr m_canvas;

        void* m_contextMenuHolder;

        MenuListTree2DPtr m_menuListTree;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeContextMenuCanvas2D_hpp_
//////////////////////////////////////////
