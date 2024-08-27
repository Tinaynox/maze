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
#include "maze-editor-tools/meta-property-drawers/MazeS32.hpp"
#include "maze-core/helpers/MazeMetaClassHelper.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorActionHelper.hpp"
#include "maze-editor-tools/managers/MazeEditorActionManager.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MetaPropertyDrawerS32
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MetaPropertyDrawerS32, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawerS32);

    //////////////////////////////////////////
    MetaPropertyDrawerS32::MetaPropertyDrawerS32()
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawerS32::~MetaPropertyDrawerS32()
    {
        if (m_drawer)
        {
            m_drawer->eventUIData.unsubscribe(this);
            m_drawer.reset();
        }
    }

    //////////////////////////////////////////
    MetaPropertyDrawerS32Ptr MetaPropertyDrawerS32::Create(MetaProperty* _metaProperty)
    {
        MetaPropertyDrawerS32Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MetaPropertyDrawerS32, object, init(_metaProperty));
        return object;
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerS32::init(MetaProperty* _metaProperty)
    {
        if (!MetaPropertyDrawer::init(_metaProperty))
            return false;

        m_drawer = PropertyDrawerS32::Create(static_cast<CString>(_metaProperty->getName()));
        m_drawer->eventUIData.subscribe(this, &MetaPropertyDrawerS32::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerS32::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        m_drawer->buildUI(_parent, _label);
    }

    ////////////////////////////////////////////
    void MetaPropertyDrawerS32::unselectUI()
    {
        m_drawer->unselectUI();
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerS32::processDataToUI()
    {
        S32 value;
        bool isMultiValue;
        fetchPropertyValue(value, isMultiValue);

        m_processingDataToUI = true;
        {
            m_drawer->setValue(value);
        }
        m_processingDataToUI = false;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerS32::processDataFromUI()
    {
        if (m_processingDataToUI)
            return;

        S32 value = m_drawer->getValue();

        if (MetaClassHelper::IsValueEqual(value, m_metaProperty, m_metaInstances))
            return;

        if (m_useEditorActions && EditorActionManager::GetInstancePtr())
            EditorActionHelper::SetValue(value, m_metaProperty, m_metaInstances);
        else
            MetaClassHelper::SetValue(value, m_metaProperty, m_metaInstances);
    }

} // namespace Maze
//////////////////////////////////////////
