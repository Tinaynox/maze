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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/assets/MazeAssetUnitMaterial.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitMaterial
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitMaterial, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitMaterial::AssetUnitMaterial()
    {

    }

    //////////////////////////////////////////
    AssetUnitMaterial::~AssetUnitMaterial()
    {
    }

    //////////////////////////////////////////
    AssetUnitMaterialPtr AssetUnitMaterial::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitMaterialPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(
            AssetUnitMaterial,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitMaterial::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    MaterialPtr const& AssetUnitMaterial::loadMaterial(bool _syncLoad)
    {
        if (!isLoaded())
        {
            initMaterial();
            _syncLoad ? loadNow() : load();
        }

        return m_material;
    }

    //////////////////////////////////////////
    bool AssetUnitMaterial::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initMaterial();
        if (!m_material)
            return false;

        m_material->loadFromAssetFile(assetFile);
        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitMaterial::unloadNowImpl()
    {
        if (m_material)
        {
            
        }

        return true;
    }

    //////////////////////////////////////////
    MaterialPtr const& AssetUnitMaterial::initMaterial()
    {
        if (m_material)
            return m_material;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_material;

        m_material = Material::Create();
        m_material->setName(m_name);        

        if (MaterialManager::GetCurrentInstance())
        {
            MaterialLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitMaterialWPtr)cast<AssetUnitMaterial>()](bool _syncLoad)
                {
                    if (AssetUnitMaterialPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitMaterialWPtr)cast<AssetUnitMaterial>()] (bool _syncLoad)
                {
                    if (AssetUnitMaterialPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitMaterialWPtr)cast<AssetUnitMaterial>()](bool _syncLoad)
                {
                    if (AssetUnitMaterialPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitMaterialWPtr)cast<AssetUnitMaterial>()](Set<String> const& _tags)
                {
                    if (AssetUnitMaterialPtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            DataBlock info;
            info.setU32(MAZE_HCS("auid"), getAssetUnitId());

            MaterialManager::GetCurrentInstance()->addMaterialToLibrary(
                m_material,
                callbacks,
                info);
        }

        return m_material;
    }


} // namespace Maze
//////////////////////////////////////////
