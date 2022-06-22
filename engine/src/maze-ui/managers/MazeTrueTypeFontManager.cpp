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
    TrueTypeFontPtr const& TrueTypeFontManager::getTrueTypeFont(HashedCString _trueTypeFont)
    {
        static TrueTypeFontPtr nullPointer;

        StringKeyMap<TrueTypeFontPtr>::const_iterator it = m_trueTypeFontsByName.find(_trueTypeFont);
        if (it != m_trueTypeFontsByName.end())
            return it->second;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_trueTypeFont);
        if (!assetFile)
            return nullPointer;

        TrueTypeFontPtr renderMesh = createTrueTypeFont(assetFile);
        renderMesh->setName(_trueTypeFont.str);
        return addTrueTypeFont(renderMesh);
    }

    //////////////////////////////////////////
    TrueTypeFontPtr const& TrueTypeFontManager::addTrueTypeFont(TrueTypeFontPtr const& _trueTypeFont)
    {
        auto it2 = m_trueTypeFontsByName.insert(
            _trueTypeFont->getName(),
            _trueTypeFont);
        return *it2;
    }

    //////////////////////////////////////////
    TrueTypeFontPtr TrueTypeFontManager::createTrueTypeFont(
        AssetFilePtr const& _assetFile)
    {
        // #TODO: loaders here
        return nullptr;
    }


} // namespace Maze
//////////////////////////////////////////
