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
#include "maze-plugin-csharp-editor-tools/meta-property-drawers/MazeScriptFieldDrawer.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class ScriptFieldDrawer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ScriptFieldDrawer, MetaPropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ScriptFieldDrawer);

    //////////////////////////////////////////
    ScriptFieldDrawer::ScriptFieldDrawer()
    {
        
    }

    //////////////////////////////////////////
    ScriptFieldDrawer::~ScriptFieldDrawer()
    {
        if (m_drawer)
        {
            m_drawer->eventUIData.unsubscribe(this);
            m_drawer.reset();
        }
    }

    //////////////////////////////////////////
    ScriptFieldDrawerPtr ScriptFieldDrawer::Create(
        ScriptFieldPtr const& _scriptProperty,
        ScriptFieldDrawerCallbacks _callbacks)
    {
        ScriptFieldDrawerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ScriptFieldDrawer, object, init(_scriptProperty, _callbacks));
        return object;
    }

    //////////////////////////////////////////
    bool ScriptFieldDrawer::init(
        ScriptFieldPtr const& _scriptProperty,
        ScriptFieldDrawerCallbacks _callbacks)
    {
        if (!MetaPropertyDrawer::init(nullptr))
            return false;

        m_scriptProperty = _scriptProperty;
        m_callbacks = _callbacks;
        
        m_drawer = m_callbacks.createDrawerCb();
        m_drawer->eventUIData.subscribe(this, &ScriptFieldDrawer::processDataFromUI);

        return true;
    }

    //////////////////////////////////////////
    void ScriptFieldDrawer::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        if (m_drawer)
            m_drawer->buildUI(_parent, _label);
    }

    //////////////////////////////////////////
    void ScriptFieldDrawer::processDataToUI()
    {
        if (m_metaInstances.empty())
            return;

        if (m_metaInstances.begin()->getMetaClass() != MonoBehaviour::GetMetaClass())
            return;

        MonoBehaviour* monoBehaviour = m_metaInstances.begin()->reinterpretObjectCast<MonoBehaviour>();
        if (monoBehaviour->getMonoInstance())
            m_callbacks.processDataToUICb(*monoBehaviour->getMonoInstance(), m_scriptProperty, m_drawer);
    }

    //////////////////////////////////////////
    void ScriptFieldDrawer::processDataFromUI()
    {
        if (m_metaInstances.empty())
            return;

        if (m_metaInstances.begin()->getMetaClass() != MonoBehaviour::GetMetaClass())
            return;

        MonoBehaviour* monoBehaviour = m_metaInstances.begin()->reinterpretObjectCast<MonoBehaviour>();
        if (monoBehaviour->getMonoInstance())
            m_callbacks.processDataFromUICb(*monoBehaviour->getMonoInstance(), m_scriptProperty, m_drawer);
    }


} // namespace Maze
//////////////////////////////////////////
