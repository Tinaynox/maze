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
#if (!defined(_MazeMenuBar2D_hpp_))
#define _MazeMenuBar2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/components/MazeMenuListTree2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(MenuBar2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(FontMaterial);
    MAZE_USING_SHARED_PTR(MenuListTree2D);
    MAZE_USING_SHARED_PTR(ContextMenuCanvas2D);


    //////////////////////////////////////////
    // Class MenuBar2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API MenuBar2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MenuBar2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MenuBar2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        struct OptionData
        {
            //////////////////////////////////////////
            struct CallbackData
            {
                String text;
                MenuListTree2D::ItemCallback callback;
                MenuListTree2D::ItemValidateCallback validate;
            };

            ToggleButton2DPtr button;
            Vector<CallbackData> callbacks;
        };

    public:

        //////////////////////////////////////////
        virtual ~MenuBar2D();

        //////////////////////////////////////////
        static MenuBar2DPtr Create(FontMaterialPtr const& _fontMaterial = FontMaterialPtr());


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        UIElement2DPtr const& getUIElement() const { return m_UIElement2D; }
            
        //////////////////////////////////////////
        inline bool getFocused() const { return m_UIElement2D->getFocused(); }

        //////////////////////////////////////////
        inline bool getPressed() const { return m_UIElement2D->getPressed(); }



        //////////////////////////////////////////
        inline void setLayoutTransform(Transform2DPtr const& _value)
        {
            if (m_layoutTransform == _value)
                return;

            m_layoutTransform = _value;
        }

        //////////////////////////////////////////
        inline Transform2DPtr const& getLayoutTransform() const { return m_layoutTransform; }

        //////////////////////////////////////////
        inline void setLayoutTransform(ComponentPtr _value) { setLayoutTransform(_value ? _value->safeCast<Transform2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getLayoutTransformComponent() const { return m_layoutTransform; }


        //////////////////////////////////////////
        inline void setMenuButtonPrefab(ToggleButton2DPtr const& _value)
        {
            if (m_menuButtonPrefab == _value)
                return;

            m_menuButtonPrefab = _value;
        }

        //////////////////////////////////////////
        inline ToggleButton2DPtr const& getMenuButtonPrefab() const { return m_menuButtonPrefab; }

        //////////////////////////////////////////
        inline void setMenuButtonPrefab(ComponentPtr _value) { setMenuButtonPrefab(_value ? _value->safeCast<ToggleButton2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getMenuButtonPrefabComponent() const { return m_menuButtonPrefab; }


        /*
        //////////////////////////////////////////
        inline void setMenuListTreePrefab(MenuListTree2DPtr const& _value)
        {
            if (m_menuListTreePrefab == _value)
                return;

            m_menuListTreePrefab = _value;
        }

        //////////////////////////////////////////
        inline MenuListTree2DPtr const& getMenuListTreePrefab() const { return m_menuListTreePrefab; }

        //////////////////////////////////////////
        inline void setMenuListTreePrefab(ComponentPtr _value) { setMenuListTreePrefab(_value ? _value->safeCast<MenuListTree2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getMenuListTreePrefabComponent() const { return m_menuListTreePrefab; }
        */



        

        //////////////////////////////////////////
        void openMenu(String const& _menuName);

        //////////////////////////////////////////
        void closeMenu();

        //////////////////////////////////////////
        String const& getCurrentMenu() const { return m_currentMenu; }

    
        //////////////////////////////////////////
        void addOption(
            String const& _menuName,
            String const& _option,
            MenuListTree2D::ItemCallback _callback,
            MenuListTree2D::ItemValidateCallback _validate = nullptr);

    protected:

        //////////////////////////////////////////
        MenuBar2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(FontMaterialPtr const& _fontMaterial);


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        // void notifyContextMenuOpened(ContextMenuCanvas2D* _menuCanvas, MenuListTree2DPtr const& _tree);

        //////////////////////////////////////////
        void notifyContextMenuClosed(ContextMenuCanvas2D* _menuCanvas, MenuListTree2DPtr const& _tree);

        //////////////////////////////////////////
        OptionData* ensureMenu(String const& _menuName);

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;

        Transform2DPtr m_layoutTransform;
        ToggleButton2DPtr m_menuButtonPrefab;
        // MenuListTree2DPtr m_menuListTreePrefab;

        // Map<String, MenuListTree2DPtr> m_items;
        Map<String, OptionData> m_options;
        String m_currentMenu;

        FontMaterialPtr m_fontMaterial;

        ContextMenuCanvas2DPtr m_contextMenuCanvas;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMenuBar2D_hpp_
//////////////////////////////////////////
