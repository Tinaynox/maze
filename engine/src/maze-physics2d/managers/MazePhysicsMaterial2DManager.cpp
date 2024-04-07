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
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include "maze-physics2d/physics/MazePhysicsMaterial2D.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-physics2d/physics/MazePhysicsMaterial2D.hpp"


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
    PhysicsMaterial2DPtr const& PhysicsMaterial2DManager::getMaterial(HashedCString _materialName)
    {
        PhysicsMaterial2DLibraryData const* libraryData = getPhysicsMaterial2DLibraryData(_materialName);
        if (libraryData)
            return libraryData->physicsMaterial2D;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_materialName);
        if (!assetFile)
        {
            MAZE_ERROR("Undefined material: %s!", _materialName.str);
            return m_defaultMaterial;
        }

        PhysicsMaterial2DPtr material = PhysicsMaterial2D::Create(assetFile);
        auto it2 = m_materialsLibrary.insert(
            _materialName,
            material);
        return it2->physicsMaterial2D;
    }

    //////////////////////////////////////////
    PhysicsMaterial2DPtr const& PhysicsMaterial2DManager::getMaterial(AssetFilePtr const& _assetFile)
    {
        static PhysicsMaterial2DPtr const nullPointer;

        PhysicsMaterial2DLibraryData const* libraryData = getPhysicsMaterial2DLibraryData(_assetFile->getFileName());
        if (libraryData)
            return libraryData->physicsMaterial2D;

        PhysicsMaterial2DPtr material = PhysicsMaterial2D::Create(_assetFile);
        PhysicsMaterial2DLibraryData* data = addMaterialToLibrary(
            HashedCString(_assetFile->getFileName().toUTF8().c_str()),
            material);

        if (data)
        {
            data->assetFile = _assetFile;
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
        HashedCString _physicsMaterial2DName,
        PhysicsMaterial2DPtr const& _physicsMaterial2D)
    {
        auto it2 = m_materialsLibrary.insert(
            _physicsMaterial2DName,
            { _physicsMaterial2D, nullptr });
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
        StringKeyMap<PhysicsMaterial2DLibraryData>::iterator it = m_materialsLibrary.begin();
        StringKeyMap<PhysicsMaterial2DLibraryData>::iterator end = m_materialsLibrary.end();
        for (; it != end; )
        {
            if (it->second.assetFile && it->second.assetFile->hasAnyOfTags(_tags))
            {
                it = m_materialsLibrary.erase(it);
                end = m_materialsLibrary.end();
            }
            else
            {
                ++it;
            }
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
