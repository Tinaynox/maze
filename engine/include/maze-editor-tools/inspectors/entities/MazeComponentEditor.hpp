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
#if (!defined(_MazeComponentEditor_hpp_))
#define _MazeComponentEditor_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ComponentEditor);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_SHARED_PTR(MetaPropertyDrawer);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(ContextMenu2D);


    //////////////////////////////////////////
    // Class ComponentEditor
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API ComponentEditor
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ComponentEditor);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ComponentEditor);

    public:

        //////////////////////////////////////////
        virtual ~ComponentEditor();


        //////////////////////////////////////////
        virtual void buildEditor(Transform2DPtr const& _parent);

        //////////////////////////////////////////
        virtual void removeEditor();

        //////////////////////////////////////////
        virtual ComponentId getComponentId() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual MetaClass* getComponentMetaClass() MAZE_ABSTRACT;


        //////////////////////////////////////////
        void updatePropertyValues(Set<ComponentPtr> const& _components);


        //////////////////////////////////////////
        template <typename TPropertyDrawer>
        inline void registerCustomPropertyDrawer(MetaProperty* _metaProperty)
        {
            registerCustomPropertyDrawer(
                _metaProperty,
                [](MetaProperty* _metaProperty)
                {
                    return TPropertyDrawer::Create(_metaProperty);
                });
        }

        //////////////////////////////////////////
        inline void registerCustomPropertyDrawer(
            MetaProperty* _metaProperty,
            std::function<MetaPropertyDrawerPtr(MetaProperty*)> _func)
        {
            m_customPropertyDrawers.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(_metaProperty),
                std::forward_as_tuple(_func));
        }

    public:

        //////////////////////////////////////////
        MultiDelegate<ComponentId> eventRemoveComponentPressed;

    protected:

        //////////////////////////////////////////
        ComponentEditor();

        //////////////////////////////////////////
        virtual bool init();


        //////////////////////////////////////////
        Transform2DPtr createDefaultEditorRoot(Transform2DPtr const& _parent);

        //////////////////////////////////////////
        Transform2DPtr createDefaultTitle(Transform2DPtr const& _parent);

        //////////////////////////////////////////
        Transform2DPtr createDefaultBody(Transform2DPtr const& _parent);

        //////////////////////////////////////////
        virtual void buildUI();

        //////////////////////////////////////////
        MetaPropertyDrawerPtr buildPropertyDrawer(
            MetaProperty* _metaProperty,
            CString _label = nullptr);

        //////////////////////////////////////////
        void notifyExpandButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void updateExpandButtonSprite();

    protected:
        
        Transform2DPtr m_editorRoot;
        Transform2DPtr m_titleRoot;
        Transform2DPtr m_bodyRoot;
        Vector<MetaPropertyDrawerPtr> m_propertyDrawers;

        SpriteRenderer2DPtr m_expandButtonSprite;
        ClickButton2DPtr m_expandButton;
        ContextMenu2DPtr m_contextMenu;

        Map<MetaProperty*, std::function<MetaPropertyDrawerPtr(MetaProperty*)>> m_customPropertyDrawers;
    };


    //////////////////////////////////////////
    // Class GenericComponentEditor
    //
    //////////////////////////////////////////
    template <typename TComponent>
    class GenericComponentEditor
        : public ComponentEditor
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_AND_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(GenericComponentEditor<TComponent>);

    protected:

        //////////////////////////////////////////
        virtual ComponentId getComponentId() MAZE_OVERRIDE
        {
            return GetStaticComponentId<TComponent>();
        }

        //////////////////////////////////////////
        virtual MetaClass* getComponentMetaClass() MAZE_OVERRIDE
        {
            return TComponent::GetMetaClass();
        }
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeComponentEditor_hpp_
//////////////////////////////////////////
