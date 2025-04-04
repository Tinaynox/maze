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
#include "MazeCSharpEditorToolsHeader.hpp"
#include "maze-plugin-csharp-editor-tools/meta-property-drawers/MazeScriptPropertyDrawer.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class ScriptPropertyDrawer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ScriptPropertyDrawer, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ScriptPropertyDrawer);

    //////////////////////////////////////////
    ScriptPropertyDrawer::ScriptPropertyDrawer()
    {
    }

    //////////////////////////////////////////
    ScriptPropertyDrawer::~ScriptPropertyDrawer()
    {
        if (m_drawer)
        {
            m_drawer->eventUIData.unsubscribe(this);
            m_drawer.reset();
        }
    }

    //////////////////////////////////////////
    ScriptPropertyDrawerPtr ScriptPropertyDrawer::Create(
        ScriptPropertyPtr const& _scriptProperty,
        ScriptPropertyDrawerCallbacks _callbacks,
        DataBlock const& _data)
    {
        ScriptPropertyDrawerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ScriptPropertyDrawer, object, init(_scriptProperty, _callbacks, _data));
        return object;
    }

    //////////////////////////////////////////
    bool ScriptPropertyDrawer::init(
        ScriptPropertyPtr const& _scriptProperty,
        ScriptPropertyDrawerCallbacks _callbacks,
        DataBlock const& _data)
    {
        if (!MetaPropertyDrawer::init(nullptr))
            return false;

        m_scriptProperty = _scriptProperty;
        m_callbacks = _callbacks;
        
        m_drawer = m_callbacks.createDrawerCb(_data);
        m_drawer->eventUIData.subscribe(this, &ScriptPropertyDrawer::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void ScriptPropertyDrawer::notifyEvent(ClassUID _eventUID, Event* _event)
    {

    }

    //////////////////////////////////////////
    void ScriptPropertyDrawer::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        if (m_drawer)
            m_drawer->buildUI(_parent, _label);
    }

    //////////////////////////////////////////
    void ScriptPropertyDrawer::processDataToUI()
    {
        if (m_metaInstances.empty())
            return;

        if (m_metaInstances.begin()->getMetaClass() != MonoBehaviour::GetMetaClass())
            return;

        MonoBehaviour* monoBehaviour = m_metaInstances.begin()->reinterpretObjectCast<MonoBehaviour>();
        if (monoBehaviour->getEntityRaw() && monoBehaviour->getMonoInstance())
            m_callbacks.processDataToUICb(
                monoBehaviour->getEntityRaw()->getEcsWorld(),
                *monoBehaviour->getMonoInstance(),
                m_scriptProperty,
                m_drawer);
    }

    //////////////////////////////////////////
    void ScriptPropertyDrawer::processDataFromUI()
    {
        if (m_metaInstances.empty())
            return;

        if (m_metaInstances.begin()->getMetaClass() != MonoBehaviour::GetMetaClass())
            return;

        MonoBehaviour* monoBehaviour = m_metaInstances.begin()->reinterpretObjectCast<MonoBehaviour>();
        if (monoBehaviour->getEntityRaw() && monoBehaviour->getEntityRaw() && monoBehaviour->getMonoInstance())
            m_callbacks.processDataFromUICb(
                monoBehaviour->getEntityRaw()->getEcsWorld(),
                *monoBehaviour->getMonoInstance(),
                m_scriptProperty,
                m_drawer);
    }


} // namespace Maze
//////////////////////////////////////////
