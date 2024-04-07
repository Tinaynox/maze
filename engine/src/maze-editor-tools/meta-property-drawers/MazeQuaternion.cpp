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
#include "maze-editor-tools/meta-property-drawers/MazeQuaternion.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MetaPropertyDrawerQuaternion
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MetaPropertyDrawerQuaternion, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawerQuaternion);

    //////////////////////////////////////////
    MetaPropertyDrawerQuaternion::MetaPropertyDrawerQuaternion()
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawerQuaternion::~MetaPropertyDrawerQuaternion()
    {
        if (m_drawer)
        {
            m_drawer->eventUIData.unsubscribe(this);
            m_drawer.reset();
        }
    }

    //////////////////////////////////////////
    MetaPropertyDrawerQuaternionPtr MetaPropertyDrawerQuaternion::Create(MetaProperty* _metaProperty)
    {
        MetaPropertyDrawerQuaternionPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MetaPropertyDrawerQuaternion, object, init(_metaProperty));
        return object;
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerQuaternion::init(MetaProperty* _metaProperty)
    {
        if (!MetaPropertyDrawer::init(_metaProperty))
            return false;

        m_drawer = PropertyDrawerQuaternion::Create(static_cast<CString>(_metaProperty->getName()));
        m_drawer->eventUIData.subscribe(this, &MetaPropertyDrawerQuaternion::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerQuaternion::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        m_drawer->buildUI(_parent, _label);
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerQuaternion::processDataToUI()
    {
        Quaternion value;
        bool isMultiValue;
        fetchPropertyValue(value, isMultiValue);

        m_processingDataToUI = true;
        {
            m_drawer->setValue(value);
        }
        m_processingDataToUI = false;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerQuaternion::processDataFromUI()
    {
        if (m_processingDataToUI)
            return;

        Quaternion q = m_drawer->getValue();

        for (MetaInstance const& metaInstance : m_metaInstances)
            m_metaProperty->setValue(metaInstance, &q);
    }

} // namespace Maze
//////////////////////////////////////////
