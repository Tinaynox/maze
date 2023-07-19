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
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-ui/fonts/MazeFontMaterial.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class FontMaterialManager
    //
    //////////////////////////////////////////
    FontMaterialManager* FontMaterialManager::s_instance = nullptr;

    //////////////////////////////////////////
    FontMaterialManager::FontMaterialManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    FontMaterialManager::~FontMaterialManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void FontMaterialManager::Initialize(FontMaterialManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(FontMaterialManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool FontMaterialManager::init()
    {
        
        return true;
    }

    //////////////////////////////////////////
    FontMaterialLibraryData const* FontMaterialManager::getFontMaterialLibraryData(HashedCString _fontMaterialName)
    {
        StringKeyMap<FontMaterialLibraryData>::const_iterator it = m_fontMaterialsLibrary.find(_fontMaterialName);
        if (it != m_fontMaterialsLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    FontMaterialPtr const& FontMaterialManager::getFontMaterial(HashedCString _trueTypeFont)
    {
        static FontMaterialPtr nullPointer;

        FontMaterialLibraryData const* libraryData = getFontMaterialLibraryData(_trueTypeFont);
        if (libraryData)
            return libraryData->fontMaterial;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_trueTypeFont);
        if (!assetFile)
            return nullPointer;

        FontMaterialPtr font = FontMaterial::Create(assetFile);
        if (!font)
            return nullPointer;

        font->setName(_trueTypeFont.str);
        FontMaterialLibraryData* data = addFontMaterialToLibrary(font);
        data->assetFile = assetFile;
        return data->fontMaterial;
    }

    //////////////////////////////////////////
    FontMaterialLibraryData* FontMaterialManager::addFontMaterialToLibrary(FontMaterialPtr const& _trueTypeFont)
    {
        auto it2 = m_fontMaterialsLibrary.insert(
            _trueTypeFont->getName(),
            _trueTypeFont);
        return it2;
    }

    //////////////////////////////////////////
    void FontMaterialManager::removeFontMaterialFromLibrary(HashedCString _fontMaterialName)
    {
        m_fontMaterialsLibrary.erase(_fontMaterialName);
    }

    //////////////////////////////////////////
    String const& FontMaterialManager::getFontMaterialName(FontMaterial const* _font)
    {
        static String nullPointer;

        for (auto const& fontData : m_fontMaterialsLibrary)
        {
            if (fontData.second.fontMaterial.get() == _font)
                return fontData.first;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    void FontMaterialManager::unloadAssetFontMaterials(Set<String> const& _tags)
    {
        StringKeyMap<FontMaterialLibraryData>::iterator it = m_fontMaterialsLibrary.begin();
        StringKeyMap<FontMaterialLibraryData>::iterator end = m_fontMaterialsLibrary.end();
        for (; it != end; )
        {
            if (it->second.assetFile && it->second.assetFile->hasAnyOfTags(_tags))
            {
                it = m_fontMaterialsLibrary.erase(it);
                end = m_fontMaterialsLibrary.end();
            }
            else
            {
                ++it;
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
