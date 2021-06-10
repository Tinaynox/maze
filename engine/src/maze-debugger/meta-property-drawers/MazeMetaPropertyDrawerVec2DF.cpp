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
#include "MazeDebuggerHeader.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawerVec2DF.hpp"
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
#include "maze-debugger/layout/MazeDebuggerLayout.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MetaPropertyDrawerVec2DF
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MetaPropertyDrawerVec2DF, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawerVec2DF);

    //////////////////////////////////////////
    MetaPropertyDrawerVec2DF::MetaPropertyDrawerVec2DF()
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawerVec2DF::~MetaPropertyDrawerVec2DF()
    {
        if (m_drawer)
        {
            m_drawer->eventUIData.unsubscribe(this);
            m_drawer.reset();
        }
    }

    //////////////////////////////////////////
    MetaPropertyDrawerVec2DFPtr MetaPropertyDrawerVec2DF::Create(MetaProperty* _metaProperty)
    {
        MetaPropertyDrawerVec2DFPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MetaPropertyDrawerVec2DF, object, init(_metaProperty));
        return object;
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawerVec2DF::init(MetaProperty* _metaProperty)
    {
        if (!MetaPropertyDrawer::init(_metaProperty))
            return false;

        m_drawer = PropertyDrawerVec2DF::Create(_metaProperty->getName());
        m_drawer->eventUIData.subscribe(this, &MetaPropertyDrawerVec2DF::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerVec2DF::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        m_drawer->buildUI(_parent, _label);
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerVec2DF::processDataToUI()
    {
        Vec2DF value;
        bool isMultiValue;
        fetchPropertyValue(value, isMultiValue);

        m_drawer->setValue(value);
    }

    //////////////////////////////////////////
    void MetaPropertyDrawerVec2DF::processDataFromUI()
    {
        Vec2DF value = m_drawer->getValue();

        for (MetaInstance const& metaInstance : m_metaInstances)
            m_metaProperty->setValue(metaInstance, &value);
    }

} // namespace Maze
//////////////////////////////////////////
