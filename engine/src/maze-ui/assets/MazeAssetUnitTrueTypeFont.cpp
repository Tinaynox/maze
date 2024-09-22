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
#include "maze-ui/assets/MazeAssetUnitTrueTypeFont.hpp"
#include "maze-ui/fonts/MazeTrueTypeFont.hpp"
#include "maze-ui/managers/MazeTrueTypeFontManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"



//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitTrueTypeFont
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitTrueTypeFont, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitTrueTypeFont::AssetUnitTrueTypeFont()
    {

    }

    //////////////////////////////////////////
    AssetUnitTrueTypeFont::~AssetUnitTrueTypeFont()
    {
    }

    //////////////////////////////////////////
    AssetUnitTrueTypeFontPtr AssetUnitTrueTypeFont::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitTrueTypeFontPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(
            AssetUnitTrueTypeFont,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitTrueTypeFont::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    TrueTypeFontPtr const& AssetUnitTrueTypeFont::loadTrueTypeFont(bool _syncLoad)
    {
        if (!isLoaded())
        {
            initTrueTypeFont();
            _syncLoad ? loadNow() : load();
        }

        return m_trueTypeFont;
    }

    //////////////////////////////////////////
    bool AssetUnitTrueTypeFont::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initTrueTypeFont();
        if (!m_trueTypeFont)
            return false;

        m_trueTypeFont->loadFromAssetFile(assetFile);

        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitTrueTypeFont::unloadNowImpl()
    {
        if (m_trueTypeFont)
        {
            
        }

        return true;
    }

    //////////////////////////////////////////
    TrueTypeFontPtr const& AssetUnitTrueTypeFont::initTrueTypeFont()
    {
        if (m_trueTypeFont)
            return m_trueTypeFont;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_trueTypeFont;

        m_trueTypeFont = TrueTypeFont::Create();
        if (!m_trueTypeFont)
            return m_trueTypeFont;

        m_trueTypeFont->setName(HashedString(m_data.getString(MAZE_HCS("name"), assetFile->getFileName().toUTF8())));

        if (TrueTypeFontManager::GetInstancePtr())
        {
            TrueTypeFontLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitTrueTypeFontWPtr)cast<AssetUnitTrueTypeFont>()](bool _syncLoad)
                {
                    if (AssetUnitTrueTypeFontPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitTrueTypeFontWPtr)cast<AssetUnitTrueTypeFont>()] (bool _syncLoad)
                {
                    if (AssetUnitTrueTypeFontPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitTrueTypeFontWPtr)cast<AssetUnitTrueTypeFont>()](bool _syncLoad)
                {
                    if (AssetUnitTrueTypeFontPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitTrueTypeFontWPtr)cast<AssetUnitTrueTypeFont>()](Set<String> const& _tags)
                {
                    if (AssetUnitTrueTypeFontPtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            DataBlock info;
            info.setU32(MAZE_HCS("auid"), getAssetUnitId());

            TrueTypeFontManager::GetInstancePtr()->addTrueTypeFontToLibrary(
                m_trueTypeFont,
                callbacks,
                info);
        }

        return m_trueTypeFont;
    }


} // namespace Maze
//////////////////////////////////////////
