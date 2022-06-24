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
#include "maze-core/math/MazeVec2D.hpp"
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
    // Struct TrueTypeFontLoaderData
    //
    //////////////////////////////////////////
    struct TrueTypeFontLoaderData
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

        LoadTrueTypeFontAssetFileFunction loadTrueTypeFontAssetFileFunc;
        LoadTrueTypeFontByteBufferFunction loadTrueTypeFontByteBufferFunc;
        IsTrueTypeFontAssetFileFunction isTrueTypeFontAssetFileFunc;
        IsTrueTypeFontByteBufferFunction isTrueTypeFontByteBufferFunc;
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
        TrueTypeFontPtr const& getTrueTypeFont(HashedCString _assetFileName);

        //////////////////////////////////////////
        inline TrueTypeFontPtr const& getTrueTypeFont(String const& _assetFileName) { return getTrueTypeFont(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        inline TrueTypeFontPtr const& getTrueTypeFont(CString _assetFileName) { return getTrueTypeFont(MAZE_HASHED_CSTRING(_assetFileName)); }

        //////////////////////////////////////////
        TrueTypeFontPtr const& addTrueTypeFont(TrueTypeFontPtr const& _trueTypeFont);

        //////////////////////////////////////////
        TrueTypeFontPtr createTrueTypeFont(
            AssetFilePtr const& _assetFile);


        //////////////////////////////////////////
        TrueTypeFontLoaderData const& getTrueTypeFontLoader() const { return m_trueTypeFontLoader; }

        //////////////////////////////////////////
        void setTrueTypeFontLoader(TrueTypeFontLoaderData const& _value) { m_trueTypeFontLoader = _value; }

    protected:

        //////////////////////////////////////////
        TrueTypeFontManager();

        //////////////////////////////////////////
        bool init();


    protected:
        static TrueTypeFontManager* s_instance;

        StringKeyMap<TrueTypeFontPtr> m_trueTypeFontsByName;

        TrueTypeFontLoaderData m_trueTypeFontLoader;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTrueTypeFontManager_hpp_
//////////////////////////////////////////
