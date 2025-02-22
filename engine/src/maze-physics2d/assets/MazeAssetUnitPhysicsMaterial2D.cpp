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
#include "MazePhysics2DHeader.hpp"
#include "maze-physics2d/assets/MazeAssetUnitPhysicsMaterial2D.hpp"
#include "maze-physics2d/physics/MazePhysicsMaterial2D.hpp"
#include "maze-physics2d/managers/MazePhysicsMaterial2DManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitPhysicsMaterial2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitPhysicsMaterial2D, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitPhysicsMaterial2D::AssetUnitPhysicsMaterial2D()
    {

    }

    //////////////////////////////////////////
    AssetUnitPhysicsMaterial2D::~AssetUnitPhysicsMaterial2D()
    {
    }

    //////////////////////////////////////////
    AssetUnitPhysicsMaterial2DPtr AssetUnitPhysicsMaterial2D::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitPhysicsMaterial2DPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(
            AssetUnitPhysicsMaterial2D,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitPhysicsMaterial2D::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DPtr const& AssetUnitPhysicsMaterial2D::loadPhysicsMaterial2D(bool _syncLoad)
    {
        if (!isLoaded())
        {
            initPhysicsMaterial2D();
            _syncLoad ? loadNow() : load();
        }

        return m_physicsMaterial2D;
    }

    //////////////////////////////////////////
    bool AssetUnitPhysicsMaterial2D::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initPhysicsMaterial2D();
        if (!m_physicsMaterial2D)
            return false;

        m_physicsMaterial2D->loadFromAssetFile(assetFile);
        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitPhysicsMaterial2D::unloadNowImpl()
    {
        if (m_physicsMaterial2D)
        {
            
        }

        return true;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DPtr const& AssetUnitPhysicsMaterial2D::initPhysicsMaterial2D()
    {
        if (m_physicsMaterial2D)
            return m_physicsMaterial2D;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_physicsMaterial2D;

        m_physicsMaterial2D = PhysicsMaterial2D::Create();
        m_physicsMaterial2D->setName(HashedString(m_data.getString(MAZE_HCS("name"), assetFile->getFileName().toUTF8())));

        if (PhysicsMaterial2DManager::GetInstancePtr())
        {
            PhysicsMaterial2DLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitPhysicsMaterial2DWPtr)cast<AssetUnitPhysicsMaterial2D>()](bool _syncLoad)
                {
                    if (AssetUnitPhysicsMaterial2DPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitPhysicsMaterial2DWPtr)cast<AssetUnitPhysicsMaterial2D>()] (bool _syncLoad)
                {
                    if (AssetUnitPhysicsMaterial2DPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitPhysicsMaterial2DWPtr)cast<AssetUnitPhysicsMaterial2D>()](bool _syncLoad)
                {
                    if (AssetUnitPhysicsMaterial2DPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitPhysicsMaterial2DWPtr)cast<AssetUnitPhysicsMaterial2D>()](Set<String> const& _tags)
                {
                    if (AssetUnitPhysicsMaterial2DPtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            DataBlock info;
            info.setU32(MAZE_HCS("auid"), getAssetUnitId());

            PhysicsMaterial2DManager::GetInstancePtr()->addMaterialToLibrary(
                m_physicsMaterial2D,
                callbacks,
                info);
        }

        return m_physicsMaterial2D;
    }


} // namespace Maze
//////////////////////////////////////////
