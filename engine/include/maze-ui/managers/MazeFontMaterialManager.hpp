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
#if (!defined(_MazeFontMaterialManager_hpp_))
#define _MazeFontMaterialManager_hpp_


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
    MAZE_USING_SHARED_PTR(FontMaterialManager);
    MAZE_USING_SHARED_PTR(FontMaterial);
    

    //////////////////////////////////////////
    // Struct FontMaterialLibraryDataCallbacks
    //
    //////////////////////////////////////////
    struct MAZE_UI_API FontMaterialLibraryDataCallbacks
    {
        std::function<void(bool)> requestLoad;
        std::function<void(bool)> requestUnload;
        std::function<void(bool)> requestReload;
        std::function<bool(Set<String> const&)> hasAnyOfTags;
    };


    //////////////////////////////////////////
    // Struct FontMaterialLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_UI_API FontMaterialLibraryData
    {
        //////////////////////////////////////////
        FontMaterialLibraryData(
            FontMaterialPtr const& _fontMaterial = nullptr,
            FontMaterialLibraryDataCallbacks const& _callbacks = FontMaterialLibraryDataCallbacks(),
            DataBlock const& _data = DataBlock::c_empty)
            : fontMaterial(_fontMaterial)
            , callbacks(_callbacks)
            , data(_data)
        {}

        FontMaterialPtr fontMaterial;
        FontMaterialLibraryDataCallbacks callbacks;
        DataBlock data;
    };


    //////////////////////////////////////////
    // Class FontMaterialManager
    //
    //////////////////////////////////////////
    class MAZE_UI_API FontMaterialManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~FontMaterialManager();

        //////////////////////////////////////////
        static void Initialize(FontMaterialManagerPtr& _manager);


        //////////////////////////////////////////
        static inline FontMaterialManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline FontMaterialManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        FontMaterialLibraryData const* getFontMaterialLibraryData(HashedCString _fontMaterialName);

        //////////////////////////////////////////
        FontMaterialLibraryData const* getFontMaterialLibraryData(String const& _assetFileName) { return getFontMaterialLibraryData(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        FontMaterialLibraryData const* getFontMaterialLibraryData(CString _assetFileName) { return getFontMaterialLibraryData(MAZE_HASHED_CSTRING(_assetFileName)); }


        //////////////////////////////////////////
        FontMaterialPtr const& getOrLoadFontMaterial(
            HashedCString _assetFileName,
            bool _syncLoad = true);

        //////////////////////////////////////////
        inline FontMaterialPtr const& getOrLoadFontMaterial(String const& _assetFileName, bool _syncLoad = true) { return getOrLoadFontMaterial(MAZE_HASHED_CSTRING(_assetFileName.c_str()), _syncLoad); }

        //////////////////////////////////////////
        inline FontMaterialPtr const& getOrLoadFontMaterial(CString _assetFileName, bool _syncLoad = true) { return getOrLoadFontMaterial(MAZE_HASHED_CSTRING(_assetFileName), _syncLoad); }


        //////////////////////////////////////////
        FontMaterialLibraryData* addFontMaterialToLibrary(
            FontMaterialPtr const& _trueTypeFont,
            FontMaterialLibraryDataCallbacks const& _callbacks = FontMaterialLibraryDataCallbacks(),
            DataBlock const& _info = DataBlock::c_empty);

        //////////////////////////////////////////
        void removeFontMaterialFromLibrary(HashedCString _fontMaterialName);

        //////////////////////////////////////////
        inline void removeFontMaterialFromLibrary(CString _fontMaterialName) { removeFontMaterialFromLibrary(HashedCString(_fontMaterialName)); }

        //////////////////////////////////////////
        inline void removeFontMaterialFromLibrary(String const& _fontMaterialName) { removeFontMaterialFromLibrary(HashedCString(_fontMaterialName.c_str())); }


        //////////////////////////////////////////
        String const& getFontMaterialName(FontMaterial const* _font);


        //////////////////////////////////////////
        inline FontMaterialPtr const& getDefaultFontMaterial() const { return m_defaultFontMaterial; }

        //////////////////////////////////////////
        inline void setDefaultFontMaterial(FontMaterialPtr const& _value) { m_defaultFontMaterial = _value; }


        //////////////////////////////////////////
        void unloadAssetFontMaterials(Set<String> const& _tags);

    protected:

        //////////////////////////////////////////
        FontMaterialManager();

        //////////////////////////////////////////
        bool init();


    protected:
        static FontMaterialManager* s_instance;

        StringKeyMap<FontMaterialLibraryData> m_fontMaterialsLibrary;

        FontMaterialPtr m_defaultFontMaterial;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFontMaterialManager_hpp_
//////////////////////////////////////////
