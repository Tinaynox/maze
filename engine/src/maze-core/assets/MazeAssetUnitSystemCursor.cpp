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
#include "maze-core/assets/MazeAssetUnitSystemCursor.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/managers/MazeSystemCursorManager.hpp"
#include "maze-core/ecs/components/MazePrefabInstance.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitSystemCursor
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitSystemCursor, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitSystemCursor::AssetUnitSystemCursor()
    {

    }

    //////////////////////////////////////////
    AssetUnitSystemCursor::~AssetUnitSystemCursor()
    {
    }

    //////////////////////////////////////////
    AssetUnitSystemCursorPtr AssetUnitSystemCursor::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitSystemCursorPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(
            AssetUnitSystemCursor,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitSystemCursor::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    SystemCursorPtr const& AssetUnitSystemCursor::loadCursor()
    {
        if (!isLoaded())
            loadNow();

        return m_cursor;
    }

    //////////////////////////////////////////
    bool AssetUnitSystemCursor::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initCursor();
        if (!m_cursor)
            return false;

        return m_cursor->loadFromAssetFile(assetFile);
    }

    //////////////////////////////////////////
    bool AssetUnitSystemCursor::unloadNowImpl()
    {
        if (m_cursor)
        {
            m_cursor.reset();
        }

        return true;
    }

    //////////////////////////////////////////
    SystemCursorPtr const& AssetUnitSystemCursor::initCursor()
    {
        if (m_cursor)
            return m_cursor;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_cursor;

        
        m_cursor = SystemCursorManager::GetInstancePtr()->createSystemCursor();
                
        if (SystemCursorManager::GetInstancePtr())
        {
            SystemCursorLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitSystemCursorWPtr)cast<AssetUnitSystemCursor>()](bool _syncLoad)
                {
                    if (AssetUnitSystemCursorPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitSystemCursorWPtr)cast<AssetUnitSystemCursor>()] (bool _syncLoad)
                {
                    if (AssetUnitSystemCursorPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitSystemCursorWPtr)cast<AssetUnitSystemCursor>()](bool _syncLoad)
                {
                    if (AssetUnitSystemCursorPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitSystemCursorWPtr)cast<AssetUnitSystemCursor>()](Set<String> const& _tags)
                {
                    if (AssetUnitSystemCursorPtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            DataBlock info;
            info.setU32(MAZE_HCS("auid"), getAssetUnitId());

            SystemCursorManager::GetInstancePtr()->addSystemCursorToLibrary(
                getName(),
                m_cursor,
                callbacks,
                info);
        }
        

        return m_cursor;
    }


} // namespace Maze
//////////////////////////////////////////
