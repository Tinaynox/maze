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
#include "maze-ui/assets/MazeAssetUnitFontMaterial.hpp"
#include "maze-ui/fonts/MazeFontMaterial.hpp"
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"



//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitFontMaterial
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitFontMaterial, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitFontMaterial::AssetUnitFontMaterial()
    {

    }

    //////////////////////////////////////////
    AssetUnitFontMaterial::~AssetUnitFontMaterial()
    {
    }

    //////////////////////////////////////////
    AssetUnitFontMaterialPtr AssetUnitFontMaterial::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitFontMaterialPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(
            AssetUnitFontMaterial,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitFontMaterial::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    FontMaterialPtr const& AssetUnitFontMaterial::loadFontMaterial(bool _syncLoad)
    {
        if (!isLoaded())
        {
            initFontMaterial();
            _syncLoad ? loadNow() : load();
        }

        return m_fontMaterial;
    }

    //////////////////////////////////////////
    bool AssetUnitFontMaterial::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initFontMaterial();
        if (!m_fontMaterial)
            return false;

        m_fontMaterial->loadFromAssetFile(assetFile);
        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitFontMaterial::unloadNowImpl()
    {
        if (m_fontMaterial)
        {
            
        }

        return true;
    }

    //////////////////////////////////////////
    FontMaterialPtr const& AssetUnitFontMaterial::initFontMaterial()
    {
        if (m_fontMaterial)
            return m_fontMaterial;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_fontMaterial;

        m_fontMaterial = FontMaterial::Create();
        m_fontMaterial->setName(HashedString(m_data.getString(MAZE_HCS("name"), assetFile->getFileName().toUTF8())));

        if (FontMaterialManager::GetInstancePtr())
        {
            FontMaterialLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitFontMaterialWPtr)cast<AssetUnitFontMaterial>()](bool _syncLoad)
                {
                    if (AssetUnitFontMaterialPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitFontMaterialWPtr)cast<AssetUnitFontMaterial>()] (bool _syncLoad)
                {
                    if (AssetUnitFontMaterialPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitFontMaterialWPtr)cast<AssetUnitFontMaterial>()](bool _syncLoad)
                {
                    if (AssetUnitFontMaterialPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitFontMaterialWPtr)cast<AssetUnitFontMaterial>()](Set<String> const& _tags)
                {
                    if (AssetUnitFontMaterialPtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            DataBlock info;
            info.setU32(MAZE_HCS("auid"), getAssetUnitId());

            FontMaterialManager::GetInstancePtr()->addFontMaterialToLibrary(
                m_fontMaterial,
                callbacks,
                info);
        }

        return m_fontMaterial;
    }


} // namespace Maze
//////////////////////////////////////////
