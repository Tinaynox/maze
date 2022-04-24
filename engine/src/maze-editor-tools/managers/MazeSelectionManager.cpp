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
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-graphics/ecs/systems/MazeGizmosSystem.hpp"
#include "maze-graphics/ecs/components/gizmos/MazeComponentGizmos.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SelectionManager
    //
    //////////////////////////////////////////
    SelectionManager* SelectionManager::s_instance = nullptr;

    //////////////////////////////////////////
    SelectionManager::SelectionManager()
        : m_selectionType(SelectionType::None)
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    SelectionManager::~SelectionManager()
    {
        if (EntityManager::GetInstancePtr())
        {
            if (EntityManager::GetInstancePtr()->getDefaultWorldRaw())
            {
                EntityManager::GetInstancePtr()->getDefaultWorldRaw()->eventEntityRemoved.unsubscribe(this);
            }
        }
        
        if (AssetManager::GetInstancePtr())
            AssetManager::GetInstancePtr()->eventAssetFileRemoved.unsubscribe(this);

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void SelectionManager::Initialize(SelectionManagerPtr& _gamepadManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(SelectionManager, _gamepadManager, init());
    }

    //////////////////////////////////////////
    bool SelectionManager::init()
    {
        EntityManager::GetInstancePtr()->getDefaultWorldRaw()->eventEntityRemoved.subscribe(this, &SelectionManager::notifyEntityRemoved);
        AssetManager::GetInstancePtr()->eventAssetFileRemoved.subscribe(this, &SelectionManager::notifyAssetFileRemoved);

        return true;
    }

    //////////////////////////////////////////
    void SelectionManager::update(F32 _dt)
    {
    }

    //////////////////////////////////////////
    void SelectionManager::unselectObjects()
    {
        selectObject(EntityPtr());
    }

    //////////////////////////////////////////
    Set<AssetFilePtr> SelectionManager::getSelectedAssetFiles() const
    {
        Set<AssetFilePtr> assetFiles;

        if (!m_selectedObjects.empty())
        {
            MetaClass* objectsMetaClass = (*m_selectedObjects.begin())->getMetaClass();
            for (auto it = ++m_selectedObjects.begin(); it != m_selectedObjects.end(); ++it)
            {
                if ((*it)->getMetaClass() != objectsMetaClass)
                {
                    objectsMetaClass = nullptr;
                    break;
                }
            }

            if (objectsMetaClass != nullptr)
            {
                // Asset files
                if (objectsMetaClass->isInheritedFrom<AssetFile>())
                {
                    for (ObjectPtr const& object : m_selectedObjects)
                        assetFiles.insert(std::static_pointer_cast<AssetFile>(object));
                }
            }
        }

        return assetFiles;
    }

    //////////////////////////////////////////
    bool SelectionManager::isObjectSelected(EntityPtr const& _object) const
    {
        Set<EntityPtr>::const_iterator it = std::find(
            m_selectedEntities.begin(),
            m_selectedEntities.end(),
            _object);

        return it != m_selectedEntities.end();
    }

    //////////////////////////////////////////
    void SelectionManager::selectObject(
        EntityPtr const& _object,
        bool _clearSelectionList,
        bool _throwEvent)
    {
        if (_clearSelectionList)
            m_selectedEntities.clear();

        if (!_object)
        {
            if (_throwEvent)
                eventSelectionChanged();

            return;
        }

        if (isObjectSelected(_object))
            return;

        m_selectedEntities.insert(_object);

        setSelectionType(SelectionType::Entities);

        if (_throwEvent)
            eventSelectionChanged();

    }

    //////////////////////////////////////////
    void SelectionManager::selectObjects(
        Vector<EntityPtr> const& _objects,
        bool _clearSelectionList)
    {
        if (_clearSelectionList)
            m_selectedEntities.clear();

        for (Size i = 0, in = m_selectedEntities.size(); i != in; ++i)
            selectObject(_objects[i], false, false);

        eventSelectionChanged();
    }

    //////////////////////////////////////////
    void SelectionManager::unselectObject(
        EntityPtr const& _object,
        bool _throwEvent)
    {
        Set<EntityPtr>::const_iterator it = std::find(
            m_selectedEntities.begin(),
            m_selectedEntities.end(),
            _object);
        if (it == m_selectedEntities.end())
            return;

        m_selectedEntities.erase(it);

        setSelectionType(SelectionType::Entities);

        if (_throwEvent)
            eventSelectionChanged();
    }

    //////////////////////////////////////////
    bool SelectionManager::isSelectedObjectsEqual(Vector<EntityPtr> const& _objects)
    {
        if (m_selectedEntities.size() != _objects.size())
            return false;

        for (Size i = 0, in = _objects.size(); i < in; ++i)
            if (!isObjectSelected(_objects[i]))
                return false;

        return true;
    }

    //////////////////////////////////////////
    bool SelectionManager::isObjectSelected(ObjectPtr const& _object) const
    {
        Set<ObjectPtr>::const_iterator it = std::find(
            m_selectedObjects.begin(),
            m_selectedObjects.end(),
            _object);

        return it != m_selectedObjects.end();
    }

    //////////////////////////////////////////
    void SelectionManager::selectObject(
        ObjectPtr const& _object,
        bool _clearSelectionList,
        bool _throwEvent)
    {
        if (_clearSelectionList)
            m_selectedObjects.clear();

        if (!_object)
        {
            if (_throwEvent)
                eventSelectionChanged();

            return;
        }

        if (isObjectSelected(_object))
            return;

        m_selectedObjects.insert(_object);

        setSelectionType(SelectionType::Objects);

        if (_throwEvent)
            eventSelectionChanged();
    }

    //////////////////////////////////////////
    void SelectionManager::unselectObject(
        ObjectPtr const& _object,
        bool _throwEvent)
    {
        Set<ObjectPtr>::const_iterator it = std::find(
            m_selectedObjects.begin(),
            m_selectedObjects.end(),
            _object);
        if (it == m_selectedObjects.end())
            return;

        m_selectedObjects.erase(it);

        setSelectionType(SelectionType::Objects);

        if (_throwEvent)
            eventSelectionChanged();
    }

    //////////////////////////////////////////
    void SelectionManager::notifyEntityRemoved(EntityPtr const& _entity)
    {
        unselectObject(_entity);
    }

    //////////////////////////////////////////
    void SelectionManager::setSelectionType(SelectionType _value)
    {
        if (m_selectionType == _value)
            return;

        switch (m_selectionType)
        {
            case SelectionType::Entities:
            {
                selectObject(EntityPtr());
                break;
            }
            case SelectionType::Objects:
            {
                selectObject(ObjectPtr());
                break;
            }
            default:
            {
                break;
            }
        }

        m_selectionType = _value;
    }

    //////////////////////////////////////////
    void SelectionManager::notifyAssetFileRemoved(AssetFilePtr const& _assetFile)
    {
        unselectObject(_assetFile);
    }
    
} // namespace Maze
//////////////////////////////////////////
