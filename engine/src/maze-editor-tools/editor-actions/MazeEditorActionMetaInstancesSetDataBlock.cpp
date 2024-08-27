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
#include "maze-editor-tools/editor-actions/MazeEditorActionMetaInstancesSetDataBlock.hpp"
#include "maze-editor-tools/events/MazeEditorActionEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorActionMetaInstancesSetDataBlock
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorActionMetaInstancesSetDataBlock, EditorAction);
 
    //////////////////////////////////////////
    EditorActionMetaInstancesSetDataBlock::EditorActionMetaInstancesSetDataBlock()
    {

    }
     
    //////////////////////////////////////////
    EditorActionMetaInstancesSetDataBlock::~EditorActionMetaInstancesSetDataBlock()
    {

    }

    //////////////////////////////////////////
    EditorActionMetaInstancesSetDataBlockPtr EditorActionMetaInstancesSetDataBlock::Create(
        MetaProperty const* _metaProperty,
        MetaInstance const& _metaInstance,
        DataBlock const& _newValue)
    {
        EditorActionMetaInstancesSetDataBlockPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorActionMetaInstancesSetDataBlock, object, init(_metaProperty, _metaInstance, _newValue));
        return object;
    }

    //////////////////////////////////////////
    EditorActionMetaInstancesSetDataBlockPtr EditorActionMetaInstancesSetDataBlock::Create(
        MetaProperty const* _metaProperty,
        Vector<MetaInstance> const& _metaInstances,
        DataBlock const& _newValue)
    {
        EditorActionMetaInstancesSetDataBlockPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorActionMetaInstancesSetDataBlock, object, init(_metaProperty, _metaInstances, _newValue));
        return object;
    }

    //////////////////////////////////////////
    bool EditorActionMetaInstancesSetDataBlock::init(
        MetaProperty const* _metaProperty,
        MetaInstance const& _metaInstance,
        DataBlock const& _newValue)
    {
        m_metaProperty = _metaProperty;
        m_metaInstances.push_back(_metaInstance);
        m_prevValues.resize(m_metaInstances.size());
        m_metaProperty->toDataBlock(_metaInstance, m_prevValues[0]);
        m_newValue = _newValue;

        return true;
    }

    //////////////////////////////////////////
    bool EditorActionMetaInstancesSetDataBlock::init(
        MetaProperty const* _metaProperty,
        Vector<MetaInstance> const& _metaInstances,
        DataBlock const& _newValue)
    {
        m_metaProperty = _metaProperty;
        m_metaInstances = _metaInstances;
        m_prevValues.resize(m_metaInstances.size());
        for (Size i = 0, in = m_metaInstances.size(); i != in; ++i)
            m_metaProperty->toDataBlock(m_metaInstances[i], m_prevValues[i]);
        m_newValue = _newValue;

        return true;
    }

    //////////////////////////////////////////
    void EditorActionMetaInstancesSetDataBlock::applyImpl()
    {
        for (Size i = 0, in = m_metaInstances.size(); i != in; ++i)
            m_metaProperty->setDataBlock(m_metaInstances[i], m_newValue);
    }

    //////////////////////////////////////////
    void EditorActionMetaInstancesSetDataBlock::revertImpl()
    {
        for (Size i = 0, in = m_metaInstances.size(); i != in; ++i)
        {
            EventManager::GetInstancePtr()->broadcastEventImmediate<EditorActionMetaInstancePropertyWillBeReverted>(
                m_metaInstances[i], m_metaProperty);
            m_metaProperty->setDataBlock(m_metaInstances[i], m_prevValues[i]);
        }
    }


} // namespace Maze
//////////////////////////////////////////
