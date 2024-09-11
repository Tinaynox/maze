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
#pragma once
#if (!defined(_MazeAssetUnit_hpp_))
#define _MazeAssetUnit_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/system/MazeFileStats.hpp"
#include "maze-core/system/MazePath.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/data/MazeHashedString.hpp"
#include "maze-core/MazeObject.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AssetUnit);   
    MAZE_USING_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    enum class MAZE_CORE_API AssetUnitLoadingState
    {
        None,
        Loading,
        Loaded,
        Error,
        Unloading,
    };


    //////////////////////////////////////////
    // Class AssetUnit
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AssetUnit
        : public SharedObject<AssetUnit>
        , public Object
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetUnit, Object);
        
    public:

        //////////////////////////////////////////
        virtual ~AssetUnit();


        //////////////////////////////////////////
        inline AssetUnitId getAssetUnitId() const { return m_auid; }

        //////////////////////////////////////////
        inline void setAssetUnitId(AssetUnitId _auid) { m_auid = _auid; }


        //////////////////////////////////////////
        inline AssetUnitLoadingState getLoadingState() const { return m_loadingState; }

        //////////////////////////////////////////
        inline bool isLoaded() const { return m_loadingState == AssetUnitLoadingState::Loaded; }

        //////////////////////////////////////////
        inline bool isLoading() const { return m_loadingState == AssetUnitLoadingState::Loading; }

        //////////////////////////////////////////
        inline bool isUnloading() const { return m_loadingState == AssetUnitLoadingState::Unloading; }


        //////////////////////////////////////////
        inline AssetFilePtr const& getAssetFile() const { return m_assetFile.lock(); }


        //////////////////////////////////////////
        void load();

        //////////////////////////////////////////
        void loadNow();


        //////////////////////////////////////////
        void unload();

        //////////////////////////////////////////
        void unloadNow();

    protected:

        //////////////////////////////////////////
        AssetUnit();

        //////////////////////////////////////////
        virtual bool init(
            AssetFilePtr const& _assetFile,
            DataBlock const& _data);


        //////////////////////////////////////////
        virtual bool loadNowImpl() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool unloadNowImpl() MAZE_ABSTRACT;
    
    protected:
        AssetUnitId m_auid = 0u;
        AssetUnitLoadingState m_loadingState = AssetUnitLoadingState::None;

        AssetFileWPtr m_assetFile;
        DataBlock m_data;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetUnit_hpp_
//////////////////////////////////////////
