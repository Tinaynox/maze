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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeEnumClass.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeDropdown2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MetaPropertyDrawerEnumClass
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MetaPropertyDrawerEnumClass, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawerEnumClass);

    //////////////////////////////////////////
    MetaPropertyDrawerEnumClass::MetaPropertyDrawerEnumClass()
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawerEnumClass::~MetaPropertyDrawerEnumClass()
    {
        if (m_drawer)
        {
            m_drawer->eventUIData.unsubscribe(this);
            m_drawer.reset();
        }
    }

    //////////////////////////////////////////
    MetaPropertyDrawerEnumClassPtr MetaPropertyDrawerEnumClass::Create(
        MetaProperty* _metaProperty,
        Vector<String> const& _enumValues)
    {
        MetaPropertyDrawerEnumClassPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MetaPropertyDrawerEnumClass, object, init(_metaProperty, _enumValues));
        return object;
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerEnumClass::init(
        MetaProperty* _metaProperty,
        Vector<String> const& _enumValues)
    {
        if (!MetaPropertyDrawer::init(_metaProperty))
            return false;

        m_drawer = PropertyDrawerEnumClass::Create(static_cast<CString>(_metaProperty->getName()), _enumValues);
        m_drawer->eventUIData.subscribe(this, &MetaPropertyDrawerEnumClass::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerEnumClass::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        m_drawer->buildUI(_parent, _label);
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerEnumClass::processDataToUI()
    {
        String value;
        bool isMultiValue;
        fetchPropertyValue(value, isMultiValue);

        m_processingDataToUI = true;
        {
            m_drawer->setValue(value);
        }
        m_processingDataToUI = false;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerEnumClass::processDataFromUI()
    {
        if (m_processingDataToUI)
            return;

        String value = m_drawer->getValue();

        for (MetaInstance const& metaInstance : m_metaInstances)
            m_metaProperty->setString(metaInstance, value);
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerEnumClass::fetchPropertyValue(
        String& _value,
        bool& _isMultiValue)
    {
        _isMultiValue = false;

        if (!m_metaInstances.empty())
        {
            m_metaProperty->toString(*m_metaInstances.begin(), _value);
            for (Set<MetaInstance>::const_iterator    it = ++m_metaInstances.begin(),
                end = m_metaInstances.end();
                it != end;
                ++it)
            {
                String value;
                m_metaProperty->toString(*it, value);
                if (value != _value)
                {
                    _isMultiValue = true;
                    return true;
                }
            }
        }

        return false;
    }

} // namespace Maze
//////////////////////////////////////////
