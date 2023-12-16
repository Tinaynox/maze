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
#if (!defined(_MazeMetaPropertyDrawerDefault_hpp_))
#define _MazeMetaPropertyDrawerDefault_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(HorizontalLayout2D);


    //////////////////////////////////////////
    // Class MetaPropertyDrawerDefault
    //
    //////////////////////////////////////////
    template <typename TProperty, typename TPropertyDrawer>
    class MetaPropertyDrawerDefault
        : public GenericMetaPropertyDrawer<TProperty>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MetaPropertyDrawerDefault, MetaPropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_AND_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawerDefault);

    public:

        //////////////////////////////////////////
        virtual ~MetaPropertyDrawerDefault()
        {
            if (m_drawer)
            {
                m_drawer->eventUIData.unsubscribe(this);
                m_drawer.reset();
            }
        }

        //////////////////////////////////////////
        static SharedPtr<MetaPropertyDrawerDefault> Create(MetaProperty* _metaProperty)
        {
            SharedPtr<MetaPropertyDrawerDefault> object;
            MAZE_CREATE_AND_INIT_SHARED_PTR(MetaPropertyDrawerDefault, object, init(_metaProperty));
            return object;
        }


        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_OVERRIDE
        {
            TProperty value;
            bool isMultiValue;
            this->fetchPropertyValue(value, isMultiValue);

            m_drawer->setValue(value);
        }

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_OVERRIDE
        {
            TProperty value = m_drawer->getValue();

            for (MetaInstance const& metaInstance : this->m_metaInstances)
                this->m_metaProperty->setValue(metaInstance, &value);
        }

    protected:

        //////////////////////////////////////////
        MetaPropertyDrawerDefault()
        {

        }

        //////////////////////////////////////////
        virtual bool init(MetaProperty* _metaProperty) MAZE_OVERRIDE
        {
            if (!MetaPropertyDrawer::init(_metaProperty))
                return false;

            m_drawer = TPropertyDrawer::Create(static_cast<CString>(_metaProperty->getName()));
            m_drawer->eventUIData.subscribe(this, &MetaPropertyDrawerDefault::processDataFromUI);

            return true;
        }

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label) MAZE_OVERRIDE
        {
            m_drawer->buildUI(_parent, _label);
        }

    protected:
        SharedPtr<TPropertyDrawer> m_drawer;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMetaPropertyDrawerDefault_hpp_
//////////////////////////////////////////
