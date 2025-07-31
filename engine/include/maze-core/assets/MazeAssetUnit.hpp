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
#include "maze-core/utils/MazeManagedSharedObject.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/data/MazeHashedString.hpp"
#include "maze-core/events/MazeEvent.hpp"
#include "maze-core/MazeObject.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(AssetUnit);   
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);


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
    // Class AssetUnitIdChangedEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AssetUnitIdChangedEvent
        : public GenericEvent<AssetUnitIdChangedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetUnitIdChangedEvent, Event);

    public:

        //////////////////////////////////////////
        AssetUnitIdChangedEvent(
            AssetUnit* _assetUnit = nullptr,
            AssetUnitId _prevAssetUnitId = c_invalidAssetUnitId,
            AssetUnitId _newAssetUnitId = c_invalidAssetUnitId)
            : m_assetUnit(_assetUnit)
            , m_prevAssetUnitId(_prevAssetUnitId)
            , m_newAssetUnitId(_newAssetUnitId)
        {}

        //////////////////////////////////////////
        inline AssetUnit* getAssetUnit() const { return m_assetUnit; }

        //////////////////////////////////////////
        inline AssetUnitId getPrevAssetUnitId() const { return m_prevAssetUnitId; }

        //////////////////////////////////////////
        inline AssetUnitId getNewAssetUnitId() const { return m_newAssetUnitId; }

    private:
        AssetUnit* m_assetUnit = nullptr;
        AssetUnitId m_prevAssetUnitId = c_invalidAssetUnitId;
        AssetUnitId m_newAssetUnitId = c_invalidAssetUnitId;
    };


    //////////////////////////////////////////
    // Class AssetUnitNameChangedEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AssetUnitNameChangedEvent
        : public GenericEvent<AssetUnitNameChangedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetUnitNameChangedEvent, Event);

    public:

        //////////////////////////////////////////
        AssetUnitNameChangedEvent(
            AssetUnit* _assetUnit = nullptr,
            HashedString const& _prevName = HashedString(),
            HashedString const& _newName = HashedString())
            : m_assetUnit(_assetUnit)
            , m_prevName(_prevName)
            , m_newName(_newName)
        {}

        //////////////////////////////////////////
        inline AssetUnit* getAssetUnit() const { return m_assetUnit; }

        //////////////////////////////////////////
        inline HashedString const& getPrevName() const { return m_prevName; }

        //////////////////////////////////////////
        inline HashedString const& getNewName() const { return m_newName; }

    private:
        AssetUnit* m_assetUnit = nullptr;
        HashedString m_prevName;
        HashedString m_newName;
    };


    //////////////////////////////////////////
    // Class AssetUnit
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AssetUnit
        : public ManagedSharedObject<AssetUnit>
        , public Object
    {
    public:
        
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetUnit, Object);
        
    public:

        //////////////////////////////////////////
        virtual ~AssetUnit();

        //////////////////////////////////////////
        virtual HashedCString getDataBlockId() const MAZE_ABSTRACT;


        //////////////////////////////////////////
        inline AssetUnitId getAssetUnitId() const { return m_auid; }

        //////////////////////////////////////////
        void setAssetUnitId(AssetUnitId _auid);


        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }

        //////////////////////////////////////////
        void setName(HashedString const& _name);


        //////////////////////////////////////////
        inline AssetUnitLoadingState getLoadingState() const { return m_loadingState; }

        //////////////////////////////////////////
        inline bool isLoaded() const { return m_loadingState == AssetUnitLoadingState::Loaded; }

        //////////////////////////////////////////
        inline bool isLoading() const { return m_loadingState == AssetUnitLoadingState::Loading; }

        //////////////////////////////////////////
        inline bool isUnloading() const { return m_loadingState == AssetUnitLoadingState::Unloading; }


        //////////////////////////////////////////
        inline AssetFilePtr getAssetFile() const { return m_assetFile.lock(); }


        //////////////////////////////////////////
        virtual void updateDataFromAssetUnit(DataBlock& _metaData) const;


        //////////////////////////////////////////
        inline DataBlock const& getData() const { return m_data; }


        //////////////////////////////////////////
        void load();

        //////////////////////////////////////////
        void loadNow();


        //////////////////////////////////////////
        void unload();

        //////////////////////////////////////////
        void unloadNow();


        //////////////////////////////////////////
        virtual bool isForceSetAssetFileName() const { return false; }

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
        HashedString m_name;
        AssetUnitLoadingState m_loadingState = AssetUnitLoadingState::None;

        AssetFileWPtr m_assetFile;
        DataBlock m_data;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetUnit_hpp_
//////////////////////////////////////////
