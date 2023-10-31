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
#if (!defined(_MazeFontManager_hpp_))
#define _MazeFontManager_hpp_


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
    MAZE_USING_SHARED_PTR(FontManager);
    MAZE_USING_SHARED_PTR(FontMaterialManager);
    MAZE_USING_SHARED_PTR(TrueTypeFontManager);
    MAZE_USING_SHARED_PTR(TrueTypeFont);
    MAZE_USING_SHARED_PTR(Font);
    

    //////////////////////////////////////////
    // Struct FontLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_UI_API FontLibraryData
    {
        //////////////////////////////////////////
        FontLibraryData(
            FontPtr const& _font = nullptr,
            AssetFilePtr const& _assetFile = nullptr)
            : font(_font)
            , assetFile(_assetFile)
        {}

        FontPtr font;
        AssetFilePtr assetFile;
    };


    //////////////////////////////////////////
    // Class FontManager
    //
    //////////////////////////////////////////
    class MAZE_UI_API FontManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~FontManager();

        //////////////////////////////////////////
        static void Initialize(FontManagerPtr& _manager);


        //////////////////////////////////////////
        static inline FontManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline FontManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        FontLibraryData const* getFontLibraryData(HashedCString _fontName);

        //////////////////////////////////////////
        FontLibraryData const* getFontLibraryData(String const& _assetFileName) { return getFontLibraryData(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        FontLibraryData const* getFontLibraryData(CString _assetFileName) { return getFontLibraryData(MAZE_HASHED_CSTRING(_assetFileName)); }


        //////////////////////////////////////////
        FontPtr const& getFont(HashedCString _assetFileName);

        //////////////////////////////////////////
        inline FontPtr const& getFont(String const& _assetFileName) { return getFont(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        inline FontPtr const& getFont(CString _assetFileName) { return getFont(MAZE_HASHED_CSTRING(_assetFileName)); }


        //////////////////////////////////////////
        FontLibraryData* addFontToLibrary(FontPtr const& _font);

        //////////////////////////////////////////
        void removeFontFromLibrary(HashedCString _fontName);

        //////////////////////////////////////////
        inline void removeFontFromLibrary(CString _fontMaterialName) { removeFontFromLibrary(HashedCString(_fontMaterialName)); }

        //////////////////////////////////////////
        inline void removeFontFromLibrary(String const& _fontMaterialName) { removeFontFromLibrary(HashedCString(_fontMaterialName.c_str())); }


        //////////////////////////////////////////
        inline FontPtr const& getDefaultFont() const { return m_defaultFont; }

        //////////////////////////////////////////
        inline void setDefaultFont(FontPtr const& _font) { m_defaultFont = _font; }


        //////////////////////////////////////////
        String const& getFontName(Font const* _font);


        //////////////////////////////////////////
        void unloadAssetFontMaterials(Set<String> const& _tags);

    protected:

        //////////////////////////////////////////
        FontManager();

        //////////////////////////////////////////
        bool init();


    protected:
        static FontManager* s_instance;

        TrueTypeFontManagerPtr m_trueTypeFontManager;

        StringKeyMap<FontLibraryData> m_fontsLibrary;

        FontMaterialManagerPtr m_fontMaterialManager;

        FontPtr m_defaultFont;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFontManager_hpp_
//////////////////////////////////////////
