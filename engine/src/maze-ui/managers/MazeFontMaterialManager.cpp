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
    FontMaterialPtr const& FontMaterialManager::getFontMaterial(HashedCString _trueTypeFont)
    {
        static FontMaterialPtr nullPointer;

        StringKeyMap<FontMaterialPtr>::const_iterator it = m_fontMaterialsByName.find(_trueTypeFont);
        if (it != m_fontMaterialsByName.end())
            return it->second;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_trueTypeFont);
        if (!assetFile)
            return nullPointer;

        FontMaterialPtr font = FontMaterial::Create(assetFile);
        if (!font)
            return nullPointer;

        font->setName(_trueTypeFont.str);
        return addFontMaterial(font);
    }

    //////////////////////////////////////////
    FontMaterialPtr const& FontMaterialManager::addFontMaterial(FontMaterialPtr const& _trueTypeFont)
    {
        auto it2 = m_fontMaterialsByName.insert(
            _trueTypeFont->getName(),
            _trueTypeFont);
        return *it2;
    }

    //////////////////////////////////////////
    String const& FontMaterialManager::getFontMaterialName(FontMaterial const* _font)
    {
        static String nullPointer;

        for (auto const& fontData : m_fontMaterialsByName)
        {
            if (fontData.second.get() == _font)
                return fontData.first;
        }

        return nullPointer;
    }


} // namespace Maze
//////////////////////////////////////////
