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
#include "maze-plugin-csharp-editor-tools/inspectors/entities/MazeComponentEditorMonoBehaviour.hpp"
#include "maze-plugin-csharp-editor-tools/meta-property-drawers/MazeScriptPropertyDrawer.hpp"
#include "maze-plugin-csharp-editor-tools/managers/MazeCSharpEditorToolsManager.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-plugin-csharp/MazeCSharpService.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class ComponentEditorMonoBehaviour
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ComponentEditorMonoBehaviour);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ComponentEditorMonoBehaviour);

    //////////////////////////////////////////
    ComponentEditorMonoBehaviour::ComponentEditorMonoBehaviour()
    {
        EventManager::GetInstancePtr()->subscribeEvent<MonoReloadEvent>(this, &ComponentEditorMonoBehaviour::notifyEvent);
    }

    //////////////////////////////////////////
    ComponentEditorMonoBehaviour::~ComponentEditorMonoBehaviour()
    {
        EventManager::GetInstancePtr()->unsubscribeEvent<MonoReloadEvent>(this);
    }

    //////////////////////////////////////////
    ComponentEditorMonoBehaviourPtr ComponentEditorMonoBehaviour::Create(ComponentId _componentId, MetaClass* _metaClass)
    {
        ComponentEditorMonoBehaviourPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ComponentEditorMonoBehaviour, object, init(_componentId, _metaClass));
        return object;
    }

    //////////////////////////////////////////
    bool ComponentEditorMonoBehaviour::init(ComponentId _componentId, MetaClass* _metaClass)
    {
        if (!ComponentEditor::init(_componentId, _metaClass))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ComponentEditorMonoBehaviour::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<MonoReloadEvent>::UID())
        {
            rebuildUI();
        }
    }

    //////////////////////////////////////////
    void ComponentEditorMonoBehaviour::buildUI()
    {
        ScriptClassPtr const& scriptClass = MonoEngine::GetMonoBehaviourSubClass(getComponentId());
        if (scriptClass)
        {

            MonoHelper::IterateSerializableFields(scriptClass,
                [&](ScriptFieldPtr const& _field)
                {
                    ScriptFieldDrawerCallbacks callbacks;

                    ScriptFieldDrawerCallbacks const* callbacksPtr = CSharpEditorToolsManager::GetInstancePtr()->getScriptFieldDrawerCallbacks(_field);
                    if (!callbacksPtr)
                        return;
                    callbacks = *callbacksPtr;

                    // #TODO: Slider mode
                    // if (typeName == MAZE_HCS("System.Single"))
                    // {
                    //      callbacks = CSharpEditorToolsManager::GetInstancePtr()->buildScriptFieldDrawerSliderF32Callbacks(0.0f, 1.0f);
                    // }

                    MonoClass* fieldTypeMonoClass = mono_class_from_mono_type(_field->getMonoType());
                    CString fieldTypeMonoClassName = mono_class_get_name(fieldTypeMonoClass);
                    CString fieldTypeMonoClassNamespace = mono_class_get_namespace(fieldTypeMonoClass);

                    DataBlock data;
                    
                    if (mono_class_is_subclass_of(
                        fieldTypeMonoClass,
                        MonoEngine::GetNativeComponentClass()->getMonoClass(),
                        false))
                    {
                        ComponentId componentId = GetComponentIdByName((String("Maze::") + fieldTypeMonoClassName).c_str());
                        data.setS32(MAZE_HCS("componentId"), componentId);
                    }
                    else
                    if (mono_class_is_subclass_of(
                        fieldTypeMonoClass,
                        MonoEngine::GetMonoBehaviourClass()->getMonoClass(),
                        false))
                    {
                        HashedString fullClassName = MonoHelper::BuildMonoClassFullName(fieldTypeMonoClassNamespace, fieldTypeMonoClassName);

                        ComponentId componentId = GetComponentIdByName(fullClassName.c_str());
                        data.setS32(MAZE_HCS("componentId"), componentId);
                    }

                    ScriptFieldDrawerPtr propertyDrawer = ScriptFieldDrawer::Create(_field, callbacks, data);
                    if (propertyDrawer)
                        addPropertyDrawer(propertyDrawer, _field->getName().c_str());
                });
            
            MonoHelper::IterateSerializableProperties(scriptClass,
                [&](ScriptPropertyPtr const& _prop)
                {
                    ScriptPropertyDrawerCallbacks callbacks;

                    ScriptPropertyDrawerCallbacks const* callbacksPtr = CSharpEditorToolsManager::GetInstancePtr()->getScriptPropertyDrawerCallbacks(_prop);
                    if (!callbacksPtr)
                        return;
                    callbacks = *callbacksPtr;

                    // #TODO: Slider mode
                    // if (typeName == MAZE_HCS("System.Single"))
                    // {
                    //      callbacks = CSharpEditorToolsManager::GetInstancePtr()->buildScriptPropertyDrawerSliderF32Callbacks(0.0f, 1.0f);
                    // }

                    MonoClass* fieldTypeMonoClass = mono_class_from_mono_type(_prop->getMonoType());
                    CString fieldTypeMonoClassName = mono_class_get_name(fieldTypeMonoClass);
                    CString fieldTypeMonoClassNamespace = mono_class_get_namespace(fieldTypeMonoClass);

                    DataBlock data;

                    if (mono_class_is_subclass_of(
                        fieldTypeMonoClass,
                        MonoEngine::GetNativeComponentClass()->getMonoClass(),
                        false))
                    {
                        ComponentId componentId = GetComponentIdByName((String("Maze::") + fieldTypeMonoClassName).c_str());
                        data.setS32(MAZE_HCS("componentId"), componentId);
                    }
                    else
                    if (mono_class_is_subclass_of(
                        fieldTypeMonoClass,
                        MonoEngine::GetMonoBehaviourClass()->getMonoClass(),
                        false))
                    {
                        HashedString fullClassName = MonoHelper::BuildMonoClassFullName(fieldTypeMonoClassNamespace, fieldTypeMonoClassName);

                        ComponentId componentId = GetComponentIdByName(fullClassName.c_str());
                        data.setS32(MAZE_HCS("componentId"), componentId);
                    }

                    ScriptPropertyDrawerPtr propertyDrawer = ScriptPropertyDrawer::Create(_prop, callbacks, data);
                    if (propertyDrawer)
                        addPropertyDrawer(propertyDrawer, _prop->getName().c_str());
                });

            
        }

        m_expandButtonSprite->getEntityRaw()->setActiveSelf(!m_propertyDrawers.empty());
    }

    //////////////////////////////////////////
    String ComponentEditorMonoBehaviour::buildEditorTitle()
    {
        ScriptClassPtr const& scriptClass = MonoEngine::GetMonoBehaviourSubClass(getComponentId());
        if (scriptClass)
            return scriptClass->getClassName() + " [MB]";
        else
            return "<Unknown script> [MB]";
    }

} // namespace Maze
//////////////////////////////////////////
