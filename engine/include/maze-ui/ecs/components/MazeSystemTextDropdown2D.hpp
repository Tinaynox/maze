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
#if (!defined(_MazeSystemTextDropdown2D_hpp_))
#define _MazeSystemTextDropdown2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-ui/ecs/components/MazeSystemTextDropdown2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(SystemTextDropdown2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(Canvas);


    //////////////////////////////////////////
    // Class SystemTextDropdown2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API SystemTextDropdown2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SystemTextDropdown2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SystemTextDropdown2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        struct OptionData
        {
            String text;
        };

    public:

        //////////////////////////////////////////
        virtual ~SystemTextDropdown2D();

        //////////////////////////////////////////
        static SystemTextDropdown2DPtr Create();


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        UIElement2DPtr const& getUIElement() const { return m_UIElement2D; }
            
        //////////////////////////////////////////
        inline bool getFocused() const { return m_UIElement2D->getFocused(); }


        //////////////////////////////////////////
        inline bool getSelected() const { return m_selected; }

        //////////////////////////////////////////
        void setSelected(bool _selected);


        //////////////////////////////////////////
        inline S32 getValue() const { return m_value; }

        //////////////////////////////////////////
        String getValueString();

        //////////////////////////////////////////
        void setValue(S32 _value);

        //////////////////////////////////////////
        void setValue(String const& _value);

        //////////////////////////////////////////
        S32 getOptionIndex(String const& _value);


        //////////////////////////////////////////
        inline void setSystemTextRenderer(SystemTextRenderer2DPtr const& _systemTextRenderer)
        {
            if (m_systemTextRenderer == _systemTextRenderer)
                return;

            m_systemTextRenderer = _systemTextRenderer;

            updateSystemTextRenderer();
        }

        //////////////////////////////////////////
        inline SystemTextRenderer2DPtr const& getSystemTextRenderer() const { return m_systemTextRenderer; }

        //////////////////////////////////////////
        inline void setSystemTextRenderer(ComponentPtr _avatar) { setSystemTextRenderer(_avatar ? _avatar->cast<SystemTextRenderer2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getSystemTextRendererComponent() const { return m_systemTextRenderer; }


        //////////////////////////////////////////
        inline void setListCanvas(CanvasPtr const& _value)
        {
            if (m_listCanvas == _value)
                return;

            m_listCanvas = _value;

            updateSystemTextRenderer();
        }

        //////////////////////////////////////////
        inline CanvasPtr const& getListCanvas() const { return m_listCanvas; }

        //////////////////////////////////////////
        inline void setListCanvas(ComponentPtr _value) { setListCanvas(_value ? _value->safeCast<Canvas>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getListCanvasComponent() const { return m_listCanvas; }


        //////////////////////////////////////////
        inline void setItemPrefabTransform(Transform2DPtr const& _value)
        {
            if (m_itemPrefabTransform == _value)
                return;

            m_itemPrefabTransform = _value;

            rebuildOptions();
        }

        //////////////////////////////////////////
        inline Transform2DPtr const& getItemPrefabTransform() const { return m_itemPrefabTransform; }

        //////////////////////////////////////////
        inline void setItemPrefabTransform(ComponentPtr _value) { setItemPrefabTransform(_value ? _value->safeCast<Transform2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getItemPrefabTransformComponent() const { return m_itemPrefabTransform; }


        //////////////////////////////////////////
        void addOption(OptionData const& _optionData);

        //////////////////////////////////////////
        inline void addOption(String const& _optionData)
        {
            addOption(OptionData{ _optionData });
        }

        //////////////////////////////////////////
        void addOptions(Vector<OptionData> const& _options);


        //////////////////////////////////////////
        void addOptions(Vector<String> const& _options);


        //////////////////////////////////////////
        void clearOptions();

    public:

        MultiDelegate<SystemTextDropdown2D*, CursorInputEvent const&> eventClick;
        MultiDelegate<SystemTextDropdown2D*, bool> eventFocusChanged;
        MultiDelegate<SystemTextDropdown2D*, bool> eventPressedChanged;
        MultiDelegate<SystemTextDropdown2D*, bool> eventSelectedChanged;
        MultiDelegate<SystemTextDropdown2D*, S32> eventValueChanged;
        MultiDelegate<SystemTextDropdown2D*> eventTextInput;

    protected:

        //////////////////////////////////////////
        SystemTextDropdown2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
            
        //////////////////////////////////////////
        void notifyCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyCursorReleaseOut(CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyFocusChanged(bool _focused);

        //////////////////////////////////////////
        void notifyPressedChanged(bool _pressed);

        //////////////////////////////////////////
        void notifyClick(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void updateSystemTextRenderer();


        //////////////////////////////////////////
        void rebuildOptions();


        //////////////////////////////////////////
        void updateCaption();

        //////////////////////////////////////////
        void updateSelectedOption();

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;
        SystemTextRenderer2DPtr m_systemTextRenderer;

        CanvasPtr m_listCanvas;
        Transform2DPtr m_itemPrefabTransform;

        bool m_selected;

        Vector<OptionData> m_options;

        S32 m_value;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemTextDropdown2D_hpp_
//////////////////////////////////////////
