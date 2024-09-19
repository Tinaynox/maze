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
#include "maze-physics2d/managers/MazePhysicsMaterial2DManager.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include "maze-physics2d/physics/MazePhysicsMaterial2D.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-physics2d/physics/MazePhysicsMaterial2D.hpp"
#include "maze-physics2d/assets/MazeAssetUnitPhysicsMaterial2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PhysicsMaterial2DManager
    //
    //////////////////////////////////////////
    PhysicsMaterial2DManager* PhysicsMaterial2DManager::s_instance = nullptr;
    
    //////////////////////////////////////////
    PhysicsMaterial2DManager::PhysicsMaterial2DManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DManager::~PhysicsMaterial2DManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void PhysicsMaterial2DManager::Initialize(PhysicsMaterial2DManagerPtr& _physics2DManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(PhysicsMaterial2DManager, _physics2DManager, init());
    }

    //////////////////////////////////////////
    bool PhysicsMaterial2DManager::init()
    {
        m_defaultMaterial = PhysicsMaterial2D::Create(0.25f, 0.35f);

        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                AssetUnitPhysicsMaterial2D::GetDataBlockId(),
                [](AssetFilePtr const& _file, DataBlock const& _data)
                {
                    return AssetUnitPhysicsMaterial2D::Create(_file, _data);
                });

            AssetUnitManager::GetInstancePtr()->eventAssetUnitAdded.subscribe(
                [](AssetUnitPtr const& _assetUnit)
                {
                    if (_assetUnit->getClassUID() == ClassInfo<AssetUnitPhysicsMaterial2D>::UID())
                        _assetUnit->castRaw<AssetUnitPhysicsMaterial2D>()->initPhysicsMaterial2D();
                });
        }

        if (AssetManager::GetInstancePtr())
        {
            AssetManager::GetInstancePtr()->eventAssetFileAdded.subscribe(
                [](AssetFilePtr const& _assetFile, HashedString const& _extension)
                {
                    if (_extension == MAZE_HCS("mzphysicsMaterial2D"))
                    {
                        if (!_assetFile->getAssetUnit<AssetUnitPhysicsMaterial2D>())
                            _assetFile->addAssetUnit(AssetUnitPhysicsMaterial2D::Create(_assetFile));
                    }
                });
        }

        return true;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DLibraryData const* PhysicsMaterial2DManager::getPhysicsMaterial2DLibraryData(HashedCString _physicsMaterial2DName)
    {
        StringKeyMap<PhysicsMaterial2DLibraryData>::const_iterator it = m_materialsLibrary.find(_physicsMaterial2DName);
        if (it != m_materialsLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DPtr const& PhysicsMaterial2DManager::getOrLoadMaterial(
        HashedCString _materialName,
        bool _syncLoad)
    {
        PhysicsMaterial2DLibraryData const* libraryData = getPhysicsMaterial2DLibraryData(_materialName);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->physicsMaterial2D;
        }

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_materialName);
        if (!assetFile)
        {
            MAZE_ERROR("Undefined material: %s!", _materialName.str);
            return m_defaultMaterial;
        }

        return getOrLoadMaterial(assetFile, _syncLoad);
    }

    //////////////////////////////////////////
    PhysicsMaterial2DPtr const& PhysicsMaterial2DManager::getOrLoadMaterial(
        AssetFilePtr const& _assetFile,
        bool _syncLoad)
    {
        static PhysicsMaterial2DPtr const nullPointer;

        PhysicsMaterial2DLibraryData const* libraryData = getPhysicsMaterial2DLibraryData(_assetFile->getFileName());
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->physicsMaterial2D;
        }

        PhysicsMaterial2DPtr material = PhysicsMaterial2D::Create(_assetFile);
        material->setName(_assetFile->getFileName().toUTF8());
        PhysicsMaterial2DLibraryData* data = addMaterialToLibrary(material);

        if (data)
        {
            data->callbacks.requestReload =
                [
                    assetFileWeak = (AssetFileWPtr)_assetFile,
                    materialWeak = (PhysicsMaterial2DWPtr)material
                ](bool _immediate)
                {
                    AssetFilePtr assetFile = assetFileWeak.lock();
                    PhysicsMaterial2DPtr material = materialWeak.lock();
                    if (assetFile && material)
                        material->loadFromAssetFile(assetFile);
                };
            data->callbacks.hasAnyOfTags =
                [assetFileWeak = (AssetFileWPtr)_assetFile](Set<String> const& _tags)
                {
                    if (AssetFilePtr assetFile = assetFileWeak.lock())
                        return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            return data->physicsMaterial2D;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    String const& PhysicsMaterial2DManager::getMaterialName(PhysicsMaterial2D const* _material)
    {
        static String nullPointer;

        for (auto const& materialData : m_materialsLibrary)
        {
            if (materialData.second.physicsMaterial2D.get() == _material)
                return materialData.first;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DLibraryData* PhysicsMaterial2DManager::addMaterialToLibrary(
        PhysicsMaterial2DPtr const& _physicsMaterial2D,
        PhysicsMaterial2DLibraryDataCallbacks const& _callbacks,
        DataBlock const& _info)
    {
        auto it2 = m_materialsLibrary.insert(
            _physicsMaterial2D->getName(),
            { _physicsMaterial2D, _callbacks, _info });
        return it2;
    }

    //////////////////////////////////////////
    void PhysicsMaterial2DManager::removeMaterialFromLibrary(HashedCString _physicsMaterial2DName)
    {
        m_materialsLibrary.erase(_physicsMaterial2DName);
    }

    //////////////////////////////////////////
    void PhysicsMaterial2DManager::unloadAssetPhysicsMaterial2Ds(Set<String> const& _tags)
    {
        Vector<std::function<void(bool)>> unloadCallbacks;

        m_materialsLibrary.iterate(
            [&](HashedCString _name, PhysicsMaterial2DLibraryData const& _data)
            {
                if (_data.callbacks.hasAnyOfTags && _data.callbacks.hasAnyOfTags(_tags) && _data.callbacks.requestUnload)
                    unloadCallbacks.push_back(_data.callbacks.requestUnload);

                return true;
            });

        for (std::function<void(bool)> const& unloadCallback : unloadCallbacks)
            unloadCallback(true);
    }
    
} // namespace Maze
//////////////////////////////////////////
