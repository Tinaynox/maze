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
#include "maze-ui/managers/MazeFontManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-ui/managers/MazeTrueTypeFontManager.hpp"
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-ui/fonts/MazeFont.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class FontManager
    //
    //////////////////////////////////////////
    FontManager* FontManager::s_instance = nullptr;

    //////////////////////////////////////////
    FontManager::FontManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    FontManager::~FontManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void FontManager::Initialize(FontManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(FontManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool FontManager::init()
    {
        TrueTypeFontManager::Initialize(m_trueTypeFontManager);
        if (!m_trueTypeFontManager)
            return false;

        FontMaterialManager::Initialize(m_fontMaterialManager);
        if (!m_fontMaterialManager)
            return false;
        
        return true;
    }

    //////////////////////////////////////////
    FontPtr const& FontManager::getFont(HashedCString _font)
    {
        static FontPtr nullPointer;

        StringKeyMap<FontPtr>::const_iterator it = m_fontsByName.find(_font);
        if (it != m_fontsByName.end())
            return it->second;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_font);
        if (!assetFile)
            return nullPointer;

        FontPtr font = Font::Create(assetFile);
        if (!font)
            return nullPointer;

        font->setName(_font.str);
        return addFont(font);
    }

    //////////////////////////////////////////
    FontPtr const& FontManager::addFont(FontPtr const& _font)
    {
        auto it2 = m_fontsByName.insert(
            _font->getName(),
            _font);
        return *it2;
    }

    //////////////////////////////////////////
    String const& FontManager::getFontName(Font const* _font)
    {
        static String nullPointer;

        for (auto const& fontData : m_fontsByName)
        {
            if (fontData.second.get() == _font)
                return fontData.first;
        }

        return nullPointer;
    }


} // namespace Maze
//////////////////////////////////////////
