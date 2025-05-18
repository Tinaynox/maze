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
#if (!defined(_MazeScriptableObjectManager_hpp_))
#define _MazeScriptableObjectManager_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-plugin-csharp/mono/MazeScriptInstance.hpp"
#include "maze-plugin-csharp/mono/MazeScriptProperty.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/events/MazeEvent.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/data/MazeHashedCString.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ScriptableObjectManager);
    MAZE_USING_MANAGED_SHARED_PTR(ScriptableObject);


    //////////////////////////////////////////
    // Struct ScriptableObjectLibraryDataCallbacks
    //
    //////////////////////////////////////////
    struct MAZE_PLUGIN_CSHARP_API ScriptableObjectLibraryDataCallbacks
    {
        std::function<void(bool)> requestLoad;
        std::function<void(bool)> requestUnload;
        std::function<void(bool)> requestReload;
        std::function<bool(Set<String> const&)> hasAnyOfTags;
    };


    //////////////////////////////////////////
    // Struct ScriptableObjectLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_PLUGIN_CSHARP_API ScriptableObjectLibraryData
    {
        //////////////////////////////////////////
        ScriptableObjectLibraryData(
            ScriptableObjectPtr const& _scriptableObject = nullptr,
            ScriptableObjectLibraryDataCallbacks const& _callbacks = ScriptableObjectLibraryDataCallbacks(),
            DataBlock const& _data = DataBlock::c_empty)
            : scriptableObject(_scriptableObject)
            , callbacks(_callbacks)
            , data(_data)
        {}

        ScriptableObjectPtr scriptableObject;
        ScriptableObjectLibraryDataCallbacks callbacks;
        DataBlock data;
    };


    //////////////////////////////////////////
    // Class ScriptableObjectManager
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API ScriptableObjectManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~ScriptableObjectManager();

        //////////////////////////////////////////
        static void Initialize(
            ScriptableObjectManagerPtr& _manager);


        //////////////////////////////////////////
        static inline ScriptableObjectManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline ScriptableObjectManager& GetInstance() { return *s_instance; }



        //////////////////////////////////////////
        inline StringKeyMap<SharedPtr<ScriptableObjectLibraryData>> const& getScriptableObjectsLibrary() const { return m_scriptableObjectsLibrary; }


        //////////////////////////////////////////
        ScriptableObjectLibraryData const* getScriptableObjectLibraryData(HashedCString _scriptableObjectName);

        //////////////////////////////////////////
        ScriptableObjectLibraryData const* getScriptableObjectLibraryData(String const& _assetFileName) { return getScriptableObjectLibraryData(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        ScriptableObjectLibraryData const* getScriptableObjectLibraryData(CString _assetFileName) { return getScriptableObjectLibraryData(MAZE_HASHED_CSTRING(_assetFileName)); }

        //////////////////////////////////////////
        ScriptableObjectLibraryData const* getScriptableObjectLibraryData(AssetUnitId _auid);



        //////////////////////////////////////////
        ScriptableObjectPtr const& getOrLoadScriptableObject(
            HashedCString _scriptableObjectName,
            bool _syncLoad);

        //////////////////////////////////////////
        ScriptableObjectPtr const& getOrLoadScriptableObject(AssetUnitId _auid, bool _syncLoad);


        //////////////////////////////////////////
        ScriptableObjectLibraryData* addScriptableObjectToLibrary(
            ScriptableObjectPtr const& _scriptableObject,
            ScriptableObjectLibraryDataCallbacks const& _callbacks,
            DataBlock const& _info);

    protected:

        //////////////////////////////////////////
        ScriptableObjectManager();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);


    protected:
        static ScriptableObjectManager* s_instance;

        StringKeyMap<SharedPtr<ScriptableObjectLibraryData>> m_scriptableObjectsLibrary;
        UnorderedMap<AssetUnitId, SharedPtr<ScriptableObjectLibraryData>> m_scriptableObjectsByAssetUnitId;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeScriptableObjectManager_hpp_
//////////////////////////////////////////
