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
#if (!defined(_MazeMenuListItem2D_hpp_))
#define _MazeMenuListItem2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(MenuListItem2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(MeshRenderer);


    //////////////////////////////////////////
    // Class MenuListItem2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API MenuListItem2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MenuListItem2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MenuListItem2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~MenuListItem2D();

        //////////////////////////////////////////
        static MenuListItem2DPtr Create();


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        UIElement2DPtr const& getUIElement() const { return m_UIElement2D; }

        //////////////////////////////////////////
        ClickButton2DPtr const& getButton() const { return m_button; }
            
        //////////////////////////////////////////
        inline bool getFocused() const { return m_UIElement2D->getFocused(); }

        //////////////////////////////////////////
        inline bool getPressed() const { return m_UIElement2D->getPressed(); }


        //////////////////////////////////////////
        inline void setCheckedNode(EntityPtr const& _checkedNode)
        {
            if (m_checkedNode == _checkedNode)
                return;

            m_checkedNode = _checkedNode;

            updateCheckedUI();
        }

        //////////////////////////////////////////
        inline EntityPtr const& getCheckedNode() const { return m_checkedNode; }


        //////////////////////////////////////////
        inline void setChecked(bool _checked)
        {
            if (m_checked == _checked)
                return;

            m_checked = _checked;

            updateCheckedUI();
        }

        //////////////////////////////////////////
        inline bool getChecked() const { return m_checked; }


        //////////////////////////////////////////
        inline void setSubMenuNode(EntityPtr const& _subMenuNode)
        {
            if (m_subMenuNode == _subMenuNode)
                return;

            m_subMenuNode = _subMenuNode;

            updateSubMenuUI();
        }

        //////////////////////////////////////////
        inline EntityPtr const& getSubMenuNode() const { return m_subMenuNode; }


        //////////////////////////////////////////
        inline void setIsSubMenu(bool _isSubMenu)
        {
            if (m_isSubMenu == _isSubMenu)
                return;

            m_isSubMenu = _isSubMenu;

            updateSubMenuUI();
        }

        //////////////////////////////////////////
        inline bool getIsSubMenu() const { return m_isSubMenu; }

    public:

        //////////////////////////////////////////
        MultiDelegate<MenuListItem2D*> eventItemFocused;

    protected:

        //////////////////////////////////////////
        MenuListItem2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        void updateCheckedUI();

        //////////////////////////////////////////
        void updateSubMenuUI();

        //////////////////////////////////////////
        void notifyFocusChanged(bool _focus);

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;
        ClickButton2DPtr m_button;
        EntityPtr m_checkedNode;
        EntityPtr m_subMenuNode;

        bool m_checked;
        bool m_isSubMenu;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMenuListItem2D_hpp_
//////////////////////////////////////////
