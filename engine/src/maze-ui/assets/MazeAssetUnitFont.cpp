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
#include "maze-ui/assets/MazeAssetUnitFont.hpp"
#include "maze-ui/fonts/MazeFont.hpp"
#include "maze-ui/managers/MazeFontManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"



//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitFont
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitFont, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitFont::AssetUnitFont()
    {

    }

    //////////////////////////////////////////
    AssetUnitFont::~AssetUnitFont()
    {
    }

    //////////////////////////////////////////
    AssetUnitFontPtr AssetUnitFont::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitFontPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(
            AssetUnitFont,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitFont::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    FontPtr const& AssetUnitFont::loadFont(bool _syncLoad)
    {
        if (!isLoaded())
        {
            initFont();
            _syncLoad ? loadNow() : load();
        }

        return m_font;
    }

    //////////////////////////////////////////
    bool AssetUnitFont::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initFont();
        if (!m_font)
            return false;

        m_font->loadFromAssetFile(assetFile);
        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitFont::unloadNowImpl()
    {
        if (m_font)
        {
            
        }

        return true;
    }

    //////////////////////////////////////////
    FontPtr const& AssetUnitFont::initFont()
    {
        if (m_font)
            return m_font;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_font;

        m_font = Font::Create();
        m_font->setName(m_data.getString(MAZE_HCS("name"), assetFile->getFileName()));

        if (FontManager::GetInstancePtr())
        {
            FontLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitFontWPtr)cast<AssetUnitFont>()](bool _syncLoad)
                {
                    if (AssetUnitFontPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitFontWPtr)cast<AssetUnitFont>()] (bool _syncLoad)
                {
                    if (AssetUnitFontPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitFontWPtr)cast<AssetUnitFont>()](bool _syncLoad)
                {
                    if (AssetUnitFontPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitFontWPtr)cast<AssetUnitFont>()](Set<String> const& _tags)
                {
                    if (AssetUnitFontPtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            DataBlock info;
            info.setU32(MAZE_HCS("auid"), getAssetUnitId());

            FontManager::GetInstancePtr()->addFontToLibrary(
                m_font,
                callbacks,
                info);
        }

        return m_font;
    }


} // namespace Maze
//////////////////////////////////////////
