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
#include "maze-plugin-csharp/managers/MazeScriptableObjectManager.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/assets/MazeAssetUnit.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"
#include "maze-plugin-csharp/assets/MazeAssetUnitScriptableObject.hpp"
#include "maze-plugin-csharp/mono/MazeScriptableObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScriptableObjectManager
    //
    //////////////////////////////////////////
    ScriptableObjectManager* ScriptableObjectManager::s_instance = nullptr;

    //////////////////////////////////////////
    ScriptableObjectManager::ScriptableObjectManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    ScriptableObjectManager::~ScriptableObjectManager()
    {
        s_instance = nullptr;

        if (EventManager::GetInstancePtr())
        {
            EventManager::GetInstancePtr()->unsubscribeEvent<CSharpCoreAssemblyLoadedEvent>(this);
            EventManager::GetInstancePtr()->unsubscribeEvent<MonoShutdownEvent>(this);
        }
    }

    //////////////////////////////////////////
    void ScriptableObjectManager::Initialize(
        ScriptableObjectManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(ScriptableObjectManager, _manager, init());
    }

    //////////////////////////////////////////
    bool ScriptableObjectManager::init()
    {
        EventManager::GetInstancePtr()->subscribeEvent<CSharpCoreAssemblyLoadedEvent>(this, &ScriptableObjectManager::notifyEvent);
        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &ScriptableObjectManager::notifyEvent);


        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                AssetUnitScriptableObject::GetDataBlockId(),
                [](AssetFilePtr const& _file, DataBlock const& _data)
            {
                return AssetUnitScriptableObject::Create(_file, _data);
            });

            AssetUnitManager::GetInstancePtr()->eventAssetUnitAdded.subscribe(
                [](AssetUnitPtr const& _assetUnit)
            {
                if (_assetUnit->getClassUID() == ClassInfo<AssetUnitScriptableObject>::UID())
                    _assetUnit->castRaw<AssetUnitScriptableObject>()->initScriptableObject();
            });
        }

        if (AssetManager::GetInstancePtr())
        {
            AssetManager::GetInstancePtr()->eventAssetFileAdded.subscribe(
                [](AssetFilePtr const& _assetFile, HashedString const& _extension)
            {
                if (_extension == MAZE_HCS("mzso"))
                {
                    if (!_assetFile->getAssetUnit<AssetUnitScriptableObject>())
                        _assetFile->addAssetUnit(AssetUnitScriptableObject::Create(_assetFile));
                }
            });

            for (AssetFilePtr const& assetFile : AssetManager::GetInstancePtr()->getAssetFilesWithExtension("mzso"))
            {
                assetFile->updateAssetUnitsFromMetaData();
            }

            AssetManager::GetInstancePtr()->eventAssetFileMoved.subscribe(
                [](AssetFilePtr const& _assetFile, Path const& _prevPath)
            {
                if (_assetFile->getExtension() == Path("mzso"))
                {
                    if (!ScriptableObjectManager::GetInstancePtr())
                        return;

                    StringKeyMap<SharedPtr<ScriptableObjectLibraryData>>& scriptableObjectsLibrary = ScriptableObjectManager::GetInstancePtr()->m_scriptableObjectsLibrary;
                    String prevScriptableObjectName = FileHelper::GetFileNameInPath(_prevPath).toUTF8();
                    StringKeyMap<SharedPtr<ScriptableObjectLibraryData>>::iterator it = scriptableObjectsLibrary.find(prevScriptableObjectName);
                    if (it != scriptableObjectsLibrary.end())
                    {
                        String newAssetName = _assetFile->getFileName().toUTF8();
                        it->second->scriptableObject->setName(HashedString(newAssetName));
                        scriptableObjectsLibrary.insert(newAssetName, it->second);
                        scriptableObjectsLibrary.erase(it);
                    }
                }
            });
        }


        return true;
    }

    //////////////////////////////////////////
    void ScriptableObjectManager::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<CSharpCoreAssemblyLoadedEvent>::UID())
        {

        }
        else
        if (_eventUID == ClassInfo<MonoShutdownEvent>::UID())
        {
            
        }
    }

    //////////////////////////////////////////
    ScriptableObjectLibraryData const* ScriptableObjectManager::getScriptableObjectLibraryData(HashedCString _scriptableObjectName)
    {
        StringKeyMap<SharedPtr<ScriptableObjectLibraryData>>::const_iterator it = m_scriptableObjectsLibrary.find(_scriptableObjectName);
        if (it != m_scriptableObjectsLibrary.end())
            return it->second.get();
        return nullptr;
    }

    //////////////////////////////////////////
    ScriptableObjectLibraryData const* ScriptableObjectManager::getScriptableObjectLibraryData(AssetUnitId _auid)
    {
        auto it = m_scriptableObjectsByAssetUnitId.find(_auid);
        if (it != m_scriptableObjectsByAssetUnitId.end())
            return it->second.get();
        return nullptr;
    }

    //////////////////////////////////////////
    ScriptableObjectPtr const& ScriptableObjectManager::getOrLoadScriptableObject(
        HashedCString _scriptableObjectName,
        bool _syncLoad)
    {
        static ScriptableObjectPtr const nullPointer;

        if (_scriptableObjectName.empty())
            return nullPointer;

        ScriptableObjectLibraryData const* libraryData = getScriptableObjectLibraryData(_scriptableObjectName);
        if (libraryData != nullptr)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->scriptableObject;
        }

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_scriptableObjectName);
        if (!assetFile)
        {
            MAZE_ERROR("Undefined scriptableObject: %s!", _scriptableObjectName.str);
            return nullPointer;
        }

        return getOrLoadScriptableObject(assetFile, _syncLoad);
    }

    //////////////////////////////////////////
    ScriptableObjectPtr const& ScriptableObjectManager::getOrLoadScriptableObject(AssetUnitId _auid, bool _syncLoad)
    {
        static ScriptableObjectPtr const nullPointer;

        ScriptableObjectLibraryData const* libraryData = getScriptableObjectLibraryData(_auid);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->scriptableObject;
        }

        AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(_auid);
        if (!assetUnit)
            return nullPointer;

        if (assetUnit->getClassUID() == ClassInfo<AssetUnitScriptableObject>::UID())
            return assetUnit->castRaw<AssetUnitScriptableObject>()->loadScriptableObject();

        return nullPointer;
    }

    //////////////////////////////////////////
    ScriptableObjectPtr const& ScriptableObjectManager::getOrLoadScriptableObject(
        AssetFilePtr const& _assetFile,
        bool _syncLoad)
    {
        static ScriptableObjectPtr const nullPointer;

        AssetUnitScriptableObjectPtr assetUnit = _assetFile->getAssetUnit<AssetUnitScriptableObject>();
        if (assetUnit)
            return getOrLoadScriptableObject(assetUnit->getAssetUnitId(), _syncLoad);

        return nullPointer;
    }

    //////////////////////////////////////////
    ScriptableObjectLibraryData* ScriptableObjectManager::addScriptableObjectToLibrary(
        ScriptableObjectPtr const& _scriptableObject,
        ScriptableObjectLibraryDataCallbacks const& _callbacks,
        DataBlock const& _info)
    {
        SharedPtr<ScriptableObjectLibraryData> data = MakeShared<ScriptableObjectLibraryData>(_scriptableObject, _callbacks, _info);

        auto it = m_scriptableObjectsLibrary.insert(_scriptableObject->getName(), data);

        AssetUnitId auid = _info.getU32(MAZE_HCS("auid"), c_invalidAssetUnitId);
        if (auid != c_invalidAssetUnitId)
            m_scriptableObjectsByAssetUnitId.emplace(auid, data);

        return it->get();
    }


} // namespace Maze
//////////////////////////////////////////
