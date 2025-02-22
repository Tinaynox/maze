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
#if (!defined(_MazeAssetUnitManager_hpp_))
#define _MazeAssetUnitManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AssetUnitManager);
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);
    MAZE_USING_MANAGED_SHARED_PTR(AssetUnit);
    MAZE_USING_SHARED_PTR(Event);


    //////////////////////////////////////////
    using AssetUnitProcessor = std::function<AssetUnitPtr(AssetFilePtr const&, DataBlock const&)>;


    //////////////////////////////////////////
    // Class AssetUnitManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AssetUnitManager
        : public SharedObject<AssetUnitManager>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~AssetUnitManager();

        //////////////////////////////////////////
        static void Initialize(AssetUnitManagerPtr& _AssetUnitManager, DataBlock const& _config = DataBlock::c_empty);


        //////////////////////////////////////////
        AssetUnitPtr createAssetUnit(AssetFilePtr const& _assetFile, DataBlock const& _data);


        //////////////////////////////////////////
        void registerAssetUnitProcessor(HashedCString _name, AssetUnitProcessor const& _processor);

        //////////////////////////////////////////
        void clearAssetUnitProcessor(HashedCString _name);


        //////////////////////////////////////////
        AssetUnitId generateAssetUnitId() const;

        //////////////////////////////////////////
        void addAssetUnit(AssetUnitPtr const& _assetFile);

        //////////////////////////////////////////
        void removeAssetUnit(AssetUnitId _assetFileId);

        //////////////////////////////////////////
        AssetUnitPtr const& getAssetUnit(AssetUnitId _assetFileId) const;

        //////////////////////////////////////////
        AssetUnitPtr const& getAssetUnit(HashedCString _name) const;


        //////////////////////////////////////////
        HashedString const& getAssetUnitName(AssetUnitId _assetFileId) const;

        //////////////////////////////////////////
        AssetUnitId getAssetUnitId(HashedCString _name) const;

        
        //////////////////////////////////////////
        static inline AssetUnitManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline AssetUnitManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        inline bool getGenerateIdsForNewAssetUnits() const { return m_generateIdsForNewAssetUnits; }

    public:
        MultiDelegate<AssetUnitPtr const&> eventAssetUnitAdded;
        MultiDelegate<AssetUnitId, AssetUnitPtr> eventAssetUnitWillBeRemoved;

    protected:

        //////////////////////////////////////////
        AssetUnitManager();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _config);


        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);

    protected:
        static AssetUnitManager* s_instance;
        
    protected:
        StringKeyMap<AssetUnitProcessor> m_assetUnitProcessors;

        UnorderedMap<AssetUnitId, AssetUnitPtr> m_assetUnitsById;
        StringKeyMap<AssetUnitPtr> m_assetUnitsByName;

        bool m_generateIdsForNewAssetUnits = true;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetUnitManager_hpp_
//////////////////////////////////////////
