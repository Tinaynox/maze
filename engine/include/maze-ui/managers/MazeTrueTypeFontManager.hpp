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
#if (!defined(_MazeTrueTypeFontManager_hpp_))
#define _MazeTrueTypeFontManager_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(TrueTypeFontManager);
    MAZE_USING_SHARED_PTR(TrueTypeFont);
    

    //////////////////////////////////////////
    using LoadTrueTypeFontAssetFileFunction = bool(*)(AssetFilePtr const& _file, TrueTypeFontPtr& _font);
    using LoadTrueTypeFontByteBufferFunction = bool(*)(ByteBufferPtr const& _fileData, TrueTypeFontPtr& _font);
    using IsTrueTypeFontAssetFileFunction = bool(*)(AssetFilePtr const& _file);
    using IsTrueTypeFontByteBufferFunction = bool(*)(ByteBufferPtr const& _fileData);


    //////////////////////////////////////////
    // Struct TrueTypeFontLibraryDataCallbacks
    //
    //////////////////////////////////////////
    struct MAZE_UI_API TrueTypeFontLibraryDataCallbacks
    {
        std::function<void(bool)> requestLoad;
        std::function<void(bool)> requestUnload;
        std::function<void(bool)> requestReload;
        std::function<bool(Set<String> const&)> hasAnyOfTags;
    };


    //////////////////////////////////////////
    // Struct TrueTypeFontLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_UI_API TrueTypeFontLibraryData
    {
        //////////////////////////////////////////
        TrueTypeFontLibraryData(
            TrueTypeFontPtr const& _trueTypeFont = nullptr,
            TrueTypeFontLibraryDataCallbacks const& _callbacks = TrueTypeFontLibraryDataCallbacks())
            : trueTypeFont(_trueTypeFont)
            , callbacks(_callbacks)
        {}

        TrueTypeFontPtr trueTypeFont;
        TrueTypeFontLibraryDataCallbacks callbacks;
    };


    //////////////////////////////////////////
    // Struct TrueTypeFontLoaderData
    //
    //////////////////////////////////////////
    struct MAZE_UI_API TrueTypeFontLoaderData
    {
        //////////////////////////////////////////
        TrueTypeFontLoaderData() = default;

        //////////////////////////////////////////
        TrueTypeFontLoaderData(
            LoadTrueTypeFontAssetFileFunction _loadTrueTypeFontAssetFileFunc,
            LoadTrueTypeFontByteBufferFunction _loadTrueTypeFontByteBufferFunc,
            IsTrueTypeFontAssetFileFunction _isTrueTypeFontAssetFileFunc,
            IsTrueTypeFontByteBufferFunction _isTrueTypeFontByteBufferFunc)
            : loadTrueTypeFontAssetFileFunc(_loadTrueTypeFontAssetFileFunc)
            , loadTrueTypeFontByteBufferFunc(_loadTrueTypeFontByteBufferFunc)
            , isTrueTypeFontAssetFileFunc(_isTrueTypeFontAssetFileFunc)
            , isTrueTypeFontByteBufferFunc(_isTrueTypeFontByteBufferFunc)
        {}

        LoadTrueTypeFontAssetFileFunction loadTrueTypeFontAssetFileFunc = nullptr;
        LoadTrueTypeFontByteBufferFunction loadTrueTypeFontByteBufferFunc = nullptr;
        IsTrueTypeFontAssetFileFunction isTrueTypeFontAssetFileFunc = nullptr;
        IsTrueTypeFontByteBufferFunction isTrueTypeFontByteBufferFunc = nullptr;
    };


    //////////////////////////////////////////
    // Class TrueTypeFontManager
    //
    //////////////////////////////////////////
    class MAZE_UI_API TrueTypeFontManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~TrueTypeFontManager();

        //////////////////////////////////////////
        static void Initialize(TrueTypeFontManagerPtr& _manager);


        //////////////////////////////////////////
        static inline TrueTypeFontManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline TrueTypeFontManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        TrueTypeFontLibraryData const* getTrueTypeFontLibraryData(HashedCString _fontName);

        //////////////////////////////////////////
        TrueTypeFontLibraryData const* getTrueTypeFontLibraryData(String const& _assetFileName) { return getTrueTypeFontLibraryData(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        TrueTypeFontLibraryData const* getTrueTypeFontLibraryData(CString _assetFileName) { return getTrueTypeFontLibraryData(MAZE_HASHED_CSTRING(_assetFileName)); }


        //////////////////////////////////////////
        TrueTypeFontPtr const& getOrLoadTrueTypeFont(HashedCString _assetFileName, bool _syncLoad = true);

        //////////////////////////////////////////
        inline TrueTypeFontPtr const& getOrLoadTrueTypeFont(String const& _assetFileName, bool _syncLoad = true) { return getOrLoadTrueTypeFont(MAZE_HASHED_CSTRING(_assetFileName.c_str()), _syncLoad); }

        //////////////////////////////////////////
        inline TrueTypeFontPtr const& getOrLoadTrueTypeFont(CString _assetFileName, bool _syncLoad = true) { return getOrLoadTrueTypeFont(MAZE_HASHED_CSTRING(_assetFileName), _syncLoad); }


        //////////////////////////////////////////
        TrueTypeFontLibraryData* addTrueTypeFontToLibrary(
            TrueTypeFontPtr const& _trueTypeFont,
            TrueTypeFontLibraryDataCallbacks const& _callbacks = TrueTypeFontLibraryDataCallbacks());

        //////////////////////////////////////////
        void removeTrueTypeFontFromLibrary(HashedCString _fontName);

        //////////////////////////////////////////
        inline void removeTrueTypeFontFromLibrary(CString _fontName) { removeTrueTypeFontFromLibrary(HashedCString(_fontName)); }

        //////////////////////////////////////////
        inline void removeTrueTypeFontFromLibrary(String const& _fontName) { removeTrueTypeFontFromLibrary(HashedCString(_fontName.c_str())); }


        //////////////////////////////////////////
        TrueTypeFontPtr createTrueTypeFont(
            AssetFilePtr const& _assetFile);


        //////////////////////////////////////////
        TrueTypeFontLoaderData const& getTrueTypeFontLoader() const { return m_trueTypeFontLoader; }

        //////////////////////////////////////////
        void setTrueTypeFontLoader(TrueTypeFontLoaderData const& _value) { m_trueTypeFontLoader = _value; }


        //////////////////////////////////////////
        void unloadAssetTrueTypeFonts(Set<String> const& _tags);

    protected:

        //////////////////////////////////////////
        TrueTypeFontManager();

        //////////////////////////////////////////
        bool init();


    protected:
        static TrueTypeFontManager* s_instance;

        StringKeyMap<TrueTypeFontLibraryData> m_trueTypeFontsLibrary;

        TrueTypeFontLoaderData m_trueTypeFontLoader;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTrueTypeFontManager_hpp_
//////////////////////////////////////////
