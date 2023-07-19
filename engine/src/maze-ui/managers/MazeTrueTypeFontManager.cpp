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
#include "MazeUIHeader.hpp"
#include "maze-ui/managers/MazeTrueTypeFontManager.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-ui/fonts/MazeTrueTypeFont.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TrueTypeFontManager
    //
    //////////////////////////////////////////
    TrueTypeFontManager* TrueTypeFontManager::s_instance = nullptr;

    //////////////////////////////////////////
    TrueTypeFontManager::TrueTypeFontManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    TrueTypeFontManager::~TrueTypeFontManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void TrueTypeFontManager::Initialize(TrueTypeFontManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(TrueTypeFontManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool TrueTypeFontManager::init()
    {
        
        return true;
    }

    //////////////////////////////////////////
    TrueTypeFontLibraryData const* TrueTypeFontManager::getTrueTypeFontLibraryData(HashedCString _fontName)
    {
        StringKeyMap<TrueTypeFontLibraryData>::const_iterator it = m_trueTypeFontsLibrary.find(_fontName);
        if (it != m_trueTypeFontsLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    TrueTypeFontPtr const& TrueTypeFontManager::getTrueTypeFont(HashedCString _trueTypeFont)
    {
        static TrueTypeFontPtr nullPointer;

        TrueTypeFontLibraryData const* libraryData = getTrueTypeFontLibraryData(_trueTypeFont);
        if (libraryData)
            return libraryData->trueTypeFont;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_trueTypeFont);
        if (!assetFile)
            return nullPointer;

        TrueTypeFontPtr font = createTrueTypeFont(assetFile);
        if (!font)
            return nullPointer;

        font->setName(_trueTypeFont.str);

        TrueTypeFontLibraryData* data = addTrueTypeFontToLibrary(font);
        data->assetFile = assetFile;
        return data->trueTypeFont;
    }

    //////////////////////////////////////////
    TrueTypeFontLibraryData* TrueTypeFontManager::addTrueTypeFontToLibrary(TrueTypeFontPtr const& _trueTypeFont)
    {
        auto it2 = m_trueTypeFontsLibrary.insert(
            _trueTypeFont->getName(),
            _trueTypeFont);
        return it2;
    }

    //////////////////////////////////////////
    void TrueTypeFontManager::removeTrueTypeFontFromLibrary(HashedCString _fontName)
    {
        m_trueTypeFontsLibrary.erase(_fontName);
    }

    //////////////////////////////////////////
    TrueTypeFontPtr TrueTypeFontManager::createTrueTypeFont(
        AssetFilePtr const& _assetFile)
    {
        if (!m_trueTypeFontLoader.loadTrueTypeFontAssetFileFunc)
            return nullptr;

        TrueTypeFontPtr font;
        m_trueTypeFontLoader.loadTrueTypeFontAssetFileFunc(_assetFile, font);
        return font;
    }

    //////////////////////////////////////////
    void TrueTypeFontManager::unloadAssetTrueTypeFonts(Set<String> const& _tags)
    {
        StringKeyMap<TrueTypeFontLibraryData>::iterator it = m_trueTypeFontsLibrary.begin();
        StringKeyMap<TrueTypeFontLibraryData>::iterator end = m_trueTypeFontsLibrary.end();
        for (; it != end; )
        {
            if (it->second.assetFile && it->second.assetFile->hasAnyOfTags(_tags))
            {
                it = m_trueTypeFontsLibrary.erase(it);
                end = m_trueTypeFontsLibrary.end();
            }
            else
            {
                ++it;
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
