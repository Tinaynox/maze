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
#if (!defined(_MazeSystemCursorManager_hpp_))
#define _MazeSystemCursorManager_hpp_


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
    MAZE_USING_SHARED_PTR(SystemCursorManager);
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);
    MAZE_USING_MANAGED_SHARED_PTR(SystemCursor);
    MAZE_USING_SHARED_PTR(Event);


    //////////////////////////////////////////
    // Enum BuiltinSystemCursorType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_13_API(MAZE_CORE_API, BuiltinSystemCursorType,
        Arrow,
        EditText,
        Wait,
        Cross,
        ArrowUp,
        SizeNWSE,
        SizeNESW,
        SizeWE,
        SizeNS,
        SizeAll,
        No,
        HandPress,
        Help);


    //////////////////////////////////////////
    // Struct SystemCursorLibraryDataCallbacks
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API SystemCursorLibraryDataCallbacks
    {
        std::function<void(bool)> requestLoad;
        std::function<void(bool)> requestUnload;
        std::function<void(bool)> requestReload;
        std::function<bool(Set<String> const&)> hasAnyOfTags;
    };


    //////////////////////////////////////////
    // Struct SystemCursorLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API SystemCursorLibraryData
    {
        //////////////////////////////////////////
        SystemCursorLibraryData(
            SystemCursorPtr const& _cursor = nullptr,
            SystemCursorLibraryDataCallbacks const& _callbacks = SystemCursorLibraryDataCallbacks(),
            DataBlock const& _data = DataBlock::c_empty)
            : cursor(_cursor)
            , callbacks(_callbacks)
            , data(_data)
        {}

        SystemCursorPtr cursor;
        SystemCursorLibraryDataCallbacks callbacks;
        DataBlock data;
    };


    //////////////////////////////////////////
    // Class SystemCursorManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SystemCursorManager
        : public SharedObject<SystemCursorManager>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~SystemCursorManager();

        //////////////////////////////////////////
        static void Initialize(SystemCursorManagerPtr& _systemCursorManager, DataBlock const& _config = DataBlock::c_empty);


        //////////////////////////////////////////
        static inline SystemCursorManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline SystemCursorManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual SystemCursorPtr createSystemCursor() { return nullptr; }

        //////////////////////////////////////////
        SystemCursorPtr createSystemCursor(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        virtual void showSystemCursor() {}

        //////////////////////////////////////////
        virtual void hideSystemCursor() {}

        //////////////////////////////////////////
        virtual void setSystemCursorPosition(Vec2F _posSystemScreen) {}

        //////////////////////////////////////////
        virtual Vec2F getSystemCursorPosition() { return Vec2F::c_zero; }



        //////////////////////////////////////////
        SystemCursorLibraryData const* getSystemCursorLibraryData(HashedCString _name);

        //////////////////////////////////////////
        SystemCursorLibraryData const* getSystemCursorLibraryData(String const& _assetFileName) { return getSystemCursorLibraryData(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        SystemCursorLibraryData const* getSystemCursorLibraryData(CString _assetFileName) { return getSystemCursorLibraryData(MAZE_HASHED_CSTRING(_assetFileName)); }

        //////////////////////////////////////////
        SystemCursorLibraryData const* getSystemCursorLibraryData(AssetUnitId _auid);


        //////////////////////////////////////////
        SystemCursorPtr const& getOrLoadSystemCursor(
            HashedCString _name,
            bool _syncLoad = true);

        //////////////////////////////////////////
        SystemCursorPtr const& getOrLoadSystemCursor(
            String const& _name,
            bool _syncLoad = true) { return getOrLoadSystemCursor(MAZE_HASHED_CSTRING(_name.c_str()), _syncLoad); }

        //////////////////////////////////////////
        SystemCursorPtr const& getOrLoadSystemCursor(
            CString _name,
            bool _syncLoad = true) { return getOrLoadSystemCursor(MAZE_HASHED_CSTRING(_name), _syncLoad); }

        //////////////////////////////////////////
        SystemCursorPtr const& getOrLoadSystemCursor(
            AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        SystemCursorPtr const& getOrLoadSystemCursor(
            AssetUnitId _auid,
            bool _syncLoad = true);

        //////////////////////////////////////////
        SystemCursorLibraryData* addSystemCursorToLibrary(
            HashedCString _name,
            SystemCursorPtr const& _cursor,
            SystemCursorLibraryDataCallbacks const& _callbacks = SystemCursorLibraryDataCallbacks(),
            DataBlock const& _info = DataBlock::c_empty);

        //////////////////////////////////////////
        void removeSystemCursorFromLibrary(HashedCString _name);

        //////////////////////////////////////////
        inline void removeSystemCursorFromLibrary(CString _name) { removeSystemCursorFromLibrary(HashedCString(_name)); }

        //////////////////////////////////////////
        inline void removeSystemCursorFromLibrary(String const& _name) { removeSystemCursorFromLibrary(HashedCString(_name.c_str())); }

        //////////////////////////////////////////
        void removeSystemCursorFromLibrary(AssetUnitId _auid);


        //////////////////////////////////////////
        void createBuiltinSystemCursors();

        //////////////////////////////////////////
        SystemCursorPtr const& ensureBuiltinSystemCursor(BuiltinSystemCursorType _systemCursorType);

        //////////////////////////////////////////
        inline SystemCursorPtr const& getBuiltinSystemCursor(BuiltinSystemCursorType _systemCursorType) { return m_builtinSystemCursors[_systemCursorType]; }


    protected:

        //////////////////////////////////////////
        SystemCursorManager();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _config);

        //////////////////////////////////////////
        virtual SystemCursorPtr const& createBuiltinSystemCursor(BuiltinSystemCursorType _systemCursorType) MAZE_ABSTRACT;

        //////////////////////////////////////////
        void initializeCursorAssets();

        //////////////////////////////////////////
        void notifyAssetManagerInitialized();

    protected:
        static SystemCursorManager* s_instance;
        
    protected:
        SystemCursorPtr m_builtinSystemCursors[BuiltinSystemCursorType::MAX];

        StringKeyMap<SharedPtr<SystemCursorLibraryData>> m_systemCursorsLibrary;
        UnorderedMap<AssetUnitId, SharedPtr<SystemCursorLibraryData>> m_systemCursorsByAssetUnitId;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemCursorManager_hpp_
//////////////////////////////////////////
