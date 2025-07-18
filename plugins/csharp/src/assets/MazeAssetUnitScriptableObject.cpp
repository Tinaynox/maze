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
#include "MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/assets/MazeAssetUnitScriptableObject.hpp"
#include "maze-plugin-csharp/mono/MazeScriptableObject.hpp"
#include "maze-plugin-csharp/managers/MazeScriptableObjectManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitScriptableObject
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitScriptableObject, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitScriptableObject::AssetUnitScriptableObject()
    {

    }

    //////////////////////////////////////////
    AssetUnitScriptableObject::~AssetUnitScriptableObject()
    {
        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->unsubscribeEvent<MonoShutdownEvent>(this);
    }

    //////////////////////////////////////////
    AssetUnitScriptableObjectPtr AssetUnitScriptableObject::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitScriptableObjectPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(
            AssetUnitScriptableObject,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitScriptableObject::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
        
        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &AssetUnitScriptableObject::notifyEvent);

        return true;
    }

    //////////////////////////////////////////
    ScriptableObjectPtr const& AssetUnitScriptableObject::loadScriptableObject(bool _syncLoad)
    {
        if (!isLoaded())
        {
            initScriptableObject();
            _syncLoad ? loadNow() : load();
        }

        return m_scriptableObject;
    }

    //////////////////////////////////////////
    bool AssetUnitScriptableObject::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initScriptableObject();
        if (!m_scriptableObject)
            return false;

        m_scriptableObject->loadFromAssetFile(assetFile);
        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitScriptableObject::unloadNowImpl()
    {
        if (m_scriptableObject)
        {
            if (m_scriptableObject->getScriptInstance())
                m_scriptableObject->clearScriptInstance();
        }

        return true;
    }

    //////////////////////////////////////////
    ScriptableObjectPtr const& AssetUnitScriptableObject::initScriptableObject()
    {
        if (m_scriptableObject)
            return m_scriptableObject;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_scriptableObject;

        m_scriptableObject = ScriptableObject::Create();
        m_scriptableObject->setName(HashedString(m_data.getString(MAZE_HCS("name"), assetFile->getFileName().toUTF8())));

        if (ScriptableObjectManager::GetInstancePtr())
        {
            ScriptableObjectLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitScriptableObjectWPtr)cast<AssetUnitScriptableObject>()](bool _syncLoad)
                {
                    if (AssetUnitScriptableObjectPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitScriptableObjectWPtr)cast<AssetUnitScriptableObject>()] (bool _syncLoad)
                {
                    if (AssetUnitScriptableObjectPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitScriptableObjectWPtr)cast<AssetUnitScriptableObject>()](bool _syncLoad)
                {
                    if (AssetUnitScriptableObjectPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitScriptableObjectWPtr)cast<AssetUnitScriptableObject>()](Set<String> const& _tags)
                {
                    if (AssetUnitScriptableObjectPtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            DataBlock info;
            info.setU32(MAZE_HCS("auid"), getAssetUnitId());

            ScriptableObjectManager::GetInstancePtr()->addScriptableObjectToLibrary(
                m_scriptableObject,
                callbacks,
                info);
        }

        return m_scriptableObject;
    }

    //////////////////////////////////////////
    void AssetUnitScriptableObject::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<MonoShutdownEvent>::UID())
        {
            unloadNow();
        }
    }


} // namespace Maze
//////////////////////////////////////////
