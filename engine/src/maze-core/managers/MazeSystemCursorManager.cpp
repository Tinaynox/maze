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
#include "MazeCoreHeader.hpp"
#include "maze-core/managers/MazeSystemCursorManager.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/system/MazeSystemCursor.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/assets/MazeAssetUnit.hpp"
#include "maze-core/assets/MazeAssetUnitSystemCursor.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeSystemCursorManager)


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(BuiltinSystemCursorType);


    //////////////////////////////////////////
    // Class SystemCursorManager
    //
    //////////////////////////////////////////
    SystemCursorManager* SystemCursorManager::s_instance = nullptr;

    //////////////////////////////////////////
    SystemCursorManager::SystemCursorManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    SystemCursorManager::~SystemCursorManager()
    {
        s_instance = nullptr;

        AssetManager::s_eventAssetManagerInitialized.unsubscribe(this);
    }

    //////////////////////////////////////////
    void SystemCursorManager::Initialize(SystemCursorManagerPtr& _systemCursorManager, DataBlock const& _config)
    {
        MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR(SystemCursorManager, _systemCursorManager, init(_config));
    }

    //////////////////////////////////////////
    bool SystemCursorManager::init(DataBlock const& _config)
    {    
        if (AssetManager::GetInstancePtr())
            initializeCursorAssets();
        else
            AssetManager::s_eventAssetManagerInitialized.subscribe(this, &SystemCursorManager::notifyAssetManagerInitialized);

        return true;
    }

    //////////////////////////////////////////
    void SystemCursorManager::initializeCursorAssets()
    {
        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                AssetUnitSystemCursor::GetDataBlockId(),
                [](AssetFilePtr const& _file, DataBlock const& _data)
            {
                return AssetUnitSystemCursor::Create(_file, _data);
            });

            AssetUnitManager::GetInstancePtr()->eventAssetUnitAdded.subscribe(
                [](AssetUnitPtr const& _assetUnit)
            {
                if (_assetUnit->getClassUID() == ClassInfo<AssetUnitSystemCursor>::UID())
                    _assetUnit->castRaw<AssetUnitSystemCursor>()->initCursor();
            });
        }

        if (AssetManager::GetInstancePtr())
        {
            AssetManager::GetInstancePtr()->eventAssetFileAdded.subscribe(
                [](AssetFilePtr const& _assetFile, HashedString const& _extension)
                {
                    if (_extension == MAZE_HCS("mzsyscur"))
                    {
                        if (!_assetFile->getAssetUnit<AssetUnitSystemCursor>())
                            _assetFile->addAssetUnit(AssetUnitSystemCursor::Create(_assetFile));
                    }
                });

            AssetManager::GetInstancePtr()->eventAssetFileMoved.subscribe(
                [](AssetFilePtr const& _assetFile, Path const& _prevPath)
                {
                    if (_assetFile->getExtension() == Path("mzsyscur"))
                    {
                        if (!SystemCursorManager::GetInstancePtr())
                            return;

                        StringKeyMap<SharedPtr<SystemCursorLibraryData>>& cursorsLibrary = SystemCursorManager::GetInstancePtr()->m_systemCursorsLibrary;
                        String prevMaterialName = FileHelper::GetFileNameInPath(_prevPath).toUTF8();
                        StringKeyMap<SharedPtr<SystemCursorLibraryData>>::iterator it = cursorsLibrary.find(prevMaterialName);
                        if (it != cursorsLibrary.end())
                        {
                            String newAssetName = _assetFile->getFileName().toUTF8();
                            it->second->cursor->setName(HashedString(newAssetName));
                            cursorsLibrary.insert(newAssetName, it->second);
                            cursorsLibrary.erase(it);
                        }
                    }
                });
        }
    }

    //////////////////////////////////////////
    void SystemCursorManager::notifyAssetManagerInitialized()
    {
        initializeCursorAssets();
    }

    //////////////////////////////////////////
    SystemCursorLibraryData const* SystemCursorManager::getSystemCursorLibraryData(HashedCString _name)
    {
        StringKeyMap<SharedPtr<SystemCursorLibraryData>>::const_iterator it = m_systemCursorsLibrary.find(_name);
        if (it != m_systemCursorsLibrary.end())
            return it->second.get();
        return nullptr;
    }

    //////////////////////////////////////////
    SystemCursorLibraryData const* SystemCursorManager::getSystemCursorLibraryData(AssetUnitId _auid)
    {
        auto it = m_systemCursorsByAssetUnitId.find(_auid);
        if (it != m_systemCursorsByAssetUnitId.end())
            return it->second.get();
        return nullptr;
    }

    //////////////////////////////////////////
    SystemCursorPtr const& SystemCursorManager::getOrLoadSystemCursor(
        HashedCString _name,
        bool _syncLoad)
    {
        static SystemCursorPtr const nullPointer;

        SystemCursorLibraryData const* libraryData = getSystemCursorLibraryData(_name);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(true);

            return libraryData->cursor;
        }

        AssetFilePtr assetFile = AssetManager::GetInstancePtr()->getAssetFile(Path(_name));
        if (!assetFile)
            return nullPointer;

        return getOrLoadSystemCursor(assetFile);
    }

    //////////////////////////////////////////
    SystemCursorPtr const& SystemCursorManager::getOrLoadSystemCursor(
        AssetFilePtr const& _assetFile)
    {
        static SystemCursorPtr const nullPointer;

        SystemCursorLibraryData const* libraryData = getSystemCursorLibraryData(_assetFile->getFileName());
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(true);

            return libraryData->cursor;
        }

        HashedString name(_assetFile->getFileName().toUTF8());

        SystemCursorPtr cursor = createSystemCursor();
        cursor->setName(name);

        SystemCursorLibraryData* data = addSystemCursorToLibrary(name, cursor);
        if (data)
        {
            data->callbacks.requestLoad =
                [
                    assetFileWeak = (AssetFileWPtr)_assetFile,
                    cursorWeak = (SystemCursorWPtr)cursor
                ](bool _immediate)
                {
                    AssetFilePtr assetFile = assetFileWeak.lock();
                    SystemCursorPtr cursor = cursorWeak.lock();
                    if (assetFile && cursor)
                        cursor->loadFromAssetFile(assetFile);
                };
            data->callbacks.requestReload =
                [
                    assetFileWeak = (AssetFileWPtr)_assetFile,
                    cursorWeak = (SystemCursorWPtr)cursor
                ](bool _immediate)
                {
                    AssetFilePtr assetFile = assetFileWeak.lock();
                    SystemCursorPtr cursor = cursorWeak.lock();
                    if (assetFile && cursor)
                        cursor->loadFromAssetFile(assetFile);
                };
            data->callbacks.hasAnyOfTags =
                [assetFileWeak = (AssetFileWPtr)_assetFile](Set<String> const& _tags)
                {
                    if (AssetFilePtr assetFile = assetFileWeak.lock())
                        return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            data->callbacks.requestLoad(true);
            return data->cursor;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    SystemCursorPtr const& SystemCursorManager::getOrLoadSystemCursor(
        AssetUnitId _auid,
        bool _syncLoad)
    {
        static SystemCursorPtr const nullPointer;

        SystemCursorLibraryData const* libraryData = getSystemCursorLibraryData(_auid);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->cursor;
        }

        AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(_auid);
        if (!assetUnit)
            return nullPointer;

        if (assetUnit->getClassUID() == ClassInfo<AssetUnitSystemCursor>::UID())
            return assetUnit->castRaw<AssetUnitSystemCursor>()->loadCursor();

        return nullPointer;
    }

    //////////////////////////////////////////
    SystemCursorLibraryData* SystemCursorManager::addSystemCursorToLibrary(
        HashedCString _name,
        SystemCursorPtr const& _cursor,
        SystemCursorLibraryDataCallbacks const& _callbacks,
        DataBlock const& _info)
    {
        SharedPtr<SystemCursorLibraryData> data = MakeShared<SystemCursorLibraryData>(_cursor, _callbacks, _info);

        auto it = m_systemCursorsLibrary.insert(_name, data);

        AssetUnitId auid = _info.getU32(MAZE_HCS("auid"), c_invalidAssetUnitId);
        if (auid != c_invalidAssetUnitId)
            m_systemCursorsByAssetUnitId.emplace(auid, data);

        return it->get();
    }

    //////////////////////////////////////////
    void SystemCursorManager::removeSystemCursorFromLibrary(HashedCString _name)
    {
        auto it = m_systemCursorsLibrary.find(_name);
        if (it != m_systemCursorsLibrary.end())
        {
            AssetUnitId auid = it->second->data.getU32(MAZE_HCS("auid"), c_invalidAssetUnitId);
            if (auid != c_invalidAssetUnitId)
                m_systemCursorsByAssetUnitId.erase(auid);

            m_systemCursorsLibrary.erase(it);
        }
    }

    //////////////////////////////////////////
    void SystemCursorManager::removeSystemCursorFromLibrary(AssetUnitId _auid)
    {
        auto it = m_systemCursorsByAssetUnitId.find(_auid);
        if (it != m_systemCursorsByAssetUnitId.end())
        {
            for (auto it2 = m_systemCursorsLibrary.begin(), end2 = m_systemCursorsLibrary.end(); it2 != end2; ++it2)
            {
                if (it2.value() == it->second)
                {
                    m_systemCursorsLibrary.erase(it2);
                    break;
                }
            }
            m_systemCursorsByAssetUnitId.erase(it);
        }
    }

    //////////////////////////////////////////
    SystemCursorPtr SystemCursorManager::createSystemCursor(AssetFilePtr const& _assetFile)
    {
        SystemCursorPtr cursor = createSystemCursor();
        if (!cursor)
            return nullptr;

        cursor->loadFromAssetFile(_assetFile);

        return cursor;
    }

    //////////////////////////////////////////
    void SystemCursorManager::createBuiltinSystemCursors()
    {
        MAZE_PROFILE_EVENT("SystemCursorManager::createBuiltinSystemCursors");

        for (BuiltinSystemCursorType t = BuiltinSystemCursorType(1); t < BuiltinSystemCursorType::MAX; ++t)
            ensureBuiltinSystemCursor(t);
    }

    //////////////////////////////////////////
    SystemCursorPtr const& SystemCursorManager::ensureBuiltinSystemCursor(BuiltinSystemCursorType _systemCursorType)
    {
        SystemCursorPtr const& systemCursor = getBuiltinSystemCursor(_systemCursorType);
        if (systemCursor)
            return systemCursor;

        return createBuiltinSystemCursor(_systemCursorType);
    }


} // namespace Maze
//////////////////////////////////////////
