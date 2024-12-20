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
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-core/helpers/MazeMetaClassHelper.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-editor-tools/events/MazeEditorActionEvents.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class MetaPropertyDrawer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MetaPropertyDrawer);

    //////////////////////////////////////////
    MetaPropertyDrawer::MetaPropertyDrawer()
        : m_metaProperty(nullptr)
    {
        
    }

    //////////////////////////////////////////
    MetaPropertyDrawer::~MetaPropertyDrawer()
    {
        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->unsubscribeEvent<EditorActionMetaInstancePropertyWillBeReverted>(this);
    }

    //////////////////////////////////////////
    bool MetaPropertyDrawer::init(MetaProperty* _metaProperty)
    {
        m_metaProperty = _metaProperty;

        EventManager::GetInstancePtr()->subscribeEvent<EditorActionMetaInstancePropertyWillBeReverted>(this, &MetaPropertyDrawer::notifyEvent);

        return true;
    }

    //////////////////////////////////////////
    void MetaPropertyDrawer::linkMetaInstances(
        Set<MetaInstance> const& _metaInstances)
    {
        m_metaInstances = _metaInstances;

        processMetaInstancesChanged();
    }

    //////////////////////////////////////////
    void MetaPropertyDrawer::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<EditorActionMetaInstancePropertyWillBeReverted>::UID())
        {
            EditorActionMetaInstancePropertyWillBeReverted* evt = _event->castRaw<EditorActionMetaInstancePropertyWillBeReverted>();
            if (m_metaProperty != evt->getChangedMetaProperty())
                return;

            for (MetaInstance metaInstance : m_metaInstances)
                if (metaInstance == evt->getChangedMetaInstance())
                {
                    unselectUI();
                    break;
                }
        }
    }


} // namespace Maze
//////////////////////////////////////////
